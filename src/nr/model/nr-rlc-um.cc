#include "ns3/log.h"
#include "nr-rlc-um.h"
#include "nr-rlc-pdu-tag.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcUm");
NS_OBJECT_ENSURE_REGISTERED (NrRlcUm);
NrRlcUm::NrRlcUm () : m_txBufferSize (0), m_nextPduType (NrRlcUmHeader::PDU_COMPLETE)
{
  NS_LOG_FUNCTION (this);
}
NrRlcUm::~NrRlcUm ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
NrRlcUm::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::NrRlcUm")
          .SetParent<NrRlc> ()
          .SetGroupName ("Nr")
          .AddConstructor<NrRlcUm> ()
          .AddAttribute ("MaxTxBufferSize", "Maximum Size of the Transmission Buffer (in Bytes)",
                         UintegerValue (10 * 1024),
                         MakeUintegerAccessor (&NrRlcUm::m_maxTxBufferSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("SnBitLen", "Length of SN (in Bits)", UintegerValue (12),
                         MakeUintegerAccessor (&NrRlcUm::SetSnBitLength, &NrRlcUm::GetSnBitLength),
                         MakeUintegerChecker<uint8_t> (6, 12))
          .AddAttribute ("WindowSize", "UM_Window_Size", UintegerValue (1 << 11),
                         MakeUintegerAccessor (&NrRlcUm::m_windowSize),
                         MakeUintegerChecker<uint16_t> ());
  return tid;
}
void
NrRlcUm::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_txBuffer.clear ();
  m_rxBuffer.clear ();
  m_txNext = 0;
  m_rxNextHighest = 0;
  m_rxNextReassembly = 0;
  m_rxTimerTrigger = 0;
  m_tReassembly.Cancel ();
  m_nextPduType = NrRlcUmHeader::PDU_UNKNOW;
}

void
NrRlcUm::SetSnBitLength (uint8_t length)
{
  NS_LOG_FUNCTION (this << length);

  if (length == 6)
    {
      m_PduTypeSN = NrRlcUmHeader::PDU_SN6;
      m_PduTypeSNSO = NrRlcUmHeader::PDU_SN6SO;
    }
  else if (length == 12)
    {
      m_PduTypeSN = NrRlcUmHeader::PDU_SN12;
      m_PduTypeSNSO = NrRlcUmHeader::PDU_SN12SO;
    }
  else
    {
      NS_ABORT_MSG ("SN length not support");
      return;
    }

  m_snBitLen = length;
  m_txNext.SetMod (length);
  m_rxNextHighest.SetMod (length);
  m_rxNextReassembly.SetMod (length);
  m_rxTimerTrigger.SetMod (length);
}
uint8_t
NrRlcUm::GetSnBitLength (void) const
{
  return m_snBitLen;
}
void
NrRlcUm::SetUmWindowSize (uint16_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_windowSize = size;
}
void
NrRlcUm::SetTxBufferSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_maxTxBufferSize = size;
}

///////////////////////////////
void
NrRlcUm::DoTransmitPdcpPdu (Ptr<Packet> p)
{
  puts ("DoTransmitPdcpPdu");
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
  if (m_txBufferSize + p->GetSize () <= m_maxTxBufferSize)
    {
      NrRlcUmHeader header;
      header.SetHeaderType (NrRlcUmHeader::PDU_COMPLETE);
      header.SetSegmentationInfo (NrRlcUmHeader::SI_ALL);
      p->AddHeader (header);
      m_txBuffer.push_back (p);
      m_txBufferSize += p->GetSize ();
    }
}
void
NrRlcUm::DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId)
{
  puts ("DoNotifyTxOpportunity");
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << bytes);
  NrMacSapProvider::TransmitPduParameters params;
  params.rnti = m_rnti;
  params.lcid = m_lcid;
  params.layer = layer;
  params.harqProcessId = harqId;
  uint32_t lastBytes = bytes;
  while (lastBytes > 0 && !m_txBuffer.empty ())
    {
      Ptr<Packet> packet = m_txBuffer.front ();
      m_txBuffer.pop_front ();
      if (packet->GetSize () <= lastBytes)
        {
          puts ("CASE 1");
          params.pdu = packet;
          m_macSapProvider->TransmitPdu (params);
          lastBytes -= packet->GetSize ();
          continue;
        }
      else
        {
          puts ("CASE 2");
          NrRlcUmHeader header;
          header.SetHeaderType (m_nextPduType);
          packet->RemoveHeader (header);
          std::cout << header << " " << packet->GetSize () << std::endl;
          NrRlcUmHeader NewHeader;
          Ptr<Packet> tmpPacket;
          if (m_nextPduType == NrRlcUmHeader::PDU_COMPLETE)
            {
              puts ("CASE 2.1");
              NewHeader.SetHeaderType (m_PduTypeSN);
              NewHeader.SetSegmentationInfo (NrRlcUmHeader::SI_FIRST_SEG);
              NewHeader.SetSequenceNumber (m_txNext);
              if (lastBytes <= NewHeader.GetSerializedSize ())
                {
                  packet->AddHeader (header);
                  m_txBuffer.push_front (packet);
                  break;
                }
              lastBytes -= NewHeader.GetSerializedSize ();

              header.SetHeaderType (m_PduTypeSNSO);
              header.SetSegmentationInfo (NrRlcUmHeader::SI_LAST_SEG);
              header.SetSequenceNumber (m_txNext++);
              header.SetSegmentOffset (lastBytes);
              m_nextPduType = m_PduTypeSNSO;
            }
          else if (m_nextPduType == m_PduTypeSNSO)
            {
              puts ("CASE 2.2");
              NewHeader.SetHeaderType (m_PduTypeSNSO);
              NewHeader.SetSegmentationInfo (NrRlcUmHeader::SI_OTHER);
              NewHeader.SetSequenceNumber (header.GetSequenceNumber ());
              NewHeader.SetSegmentOffset (header.GetSegmentOffset ());
              if (lastBytes <= NewHeader.GetSerializedSize ())
                {
                  packet->AddHeader (header);
                  m_txBuffer.push_front (packet);
                  break;
                }
              lastBytes -= NewHeader.GetSerializedSize ();

              header.SetSegmentOffset (header.GetSegmentOffset () + lastBytes);
            }
          else
            {
              NS_ABORT_MSG ("Next Pdu Type Error");
            }

          tmpPacket = packet->CreateFragment (0, lastBytes);
          tmpPacket->AddHeader (NewHeader);
          packet->RemoveAtStart (lastBytes);
          packet->AddHeader (header);
          m_txBuffer.push_front (packet);
          std::cout << "@@@ " << *tmpPacket << " " << tmpPacket->GetSize () << std::endl;
          tmpPacket->PeekHeader (NewHeader);
          std::cout << "### " << NewHeader << std::endl;
          params.pdu = tmpPacket;
          m_macSapProvider->TransmitPdu (params);
          lastBytes = 0;
        }
    }
}
void
NrRlcUm::DoNotifyHarqDeliveryFailure ()
{
  NS_LOG_FUNCTION (this);
}
void
NrRlcUm::DoReceivePdu (Ptr<Packet> p)
{
  puts ("DoReceivePdu");
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
  NrRlcUmHeader header;
  header.SetHeaderType (NrRlcUmHeader::PDU_COMPLETE);
  p->PeekHeader (header);
  NrRlcUmHeader::SIType_t si = header.GetSegmentationInfo ();
  switch (si)
    {
    case NrRlcUmHeader::SI_ALL:
      break;
    case NrRlcUmHeader::SI_FIRST_SEG:
      header.SetHeaderType (m_PduTypeSN);
      break;
    default:
      header.SetHeaderType (m_PduTypeSNSO);
      break;
    }
  p->RemoveHeader (header);
  m_rlcSapUser->ReceivePdcpPdu (p);
}
} // namespace ns3
