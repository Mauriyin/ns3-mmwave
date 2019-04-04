#include "ns3/log.h"
#include "nr-rlc-um.h"
#include "nr-rlc-sdu-tag.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcUm");
NS_OBJECT_ENSURE_REGISTERED (NrRlcUm);
NrRlcUm::NrRlcUm () : m_txBufferSize (0)
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
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
  if (m_txBufferSize + p->GetSize () <= m_maxTxBufferSize)
    {
      // NrRlcSduTag tag;
      // tag.GenSduId ();
      // p->AddPacketTag (tag);
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
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << bytes);
  /*Ptr<Packet> packet = m_txBuffer.front ()->Copy ();
  m_txBuffer.pop_front ();
  NrMacSapProvider::TransmitPduParameters params;
  params.pdu = packet;
  params.rnti = m_rnti;
  params.lcid = m_lcid;
  params.layer = layer;
  params.harqProcessId = harqId;
  m_macSapProvider->TransmitPdu (params);*/
  // Ptr<Packet> packet = Create<Packet> ();
  NrMacSapProvider::TransmitPduParameters params;
  params.rnti = m_rnti;
  params.lcid = m_lcid;
  params.layer = layer;
  params.harqProcessId = harqId;
  uint32_t lastBytes = bytes;
  while (lastBytes > 0 && !m_txBuffer.empty ())
    {
      Ptr<Packet> tmpPacket = m_txBuffer.front ();
      m_txBuffer.pop_front ();
      if (tmpPacket->GetSize () <= lastBytes)
        {
          params.pdu = tmpPacket;
          m_macSapProvider->TransmitPdu (params);
          lastBytes -= tmpPacket->GetSize ();
          continue;
        }
      else
        {
          NrRlcUmHeader header;
          header.SetHeaderType (NrRlcUmHeader::PDU_COMPLETE);
          tmpPacket->RemoveHeader (header);
          tmpPacket->CreateFragment (0, lastBytes);
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
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
  m_rlcSapUser->ReceivePdcpPdu (p);
}
} // namespace ns3
