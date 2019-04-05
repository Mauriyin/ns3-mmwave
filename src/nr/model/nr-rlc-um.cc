#include "ns3/log.h"
#include "nr-rlc-um.h"
#include "nr-rlc-pdu-tag.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcUm");

NrRlcUmRxBuffer::NrRlcUmRxBuffer ()
{
  Clear ();
}
NrRlcUmRxBuffer::~NrRlcUmRxBuffer ()
{
}
bool
NrRlcUmRxBuffer::AddPacket (Ptr<Packet> p, uint16_t so)
{
  if (m_buffer.find (so) != m_buffer.end ())
    {
      return false;
    }
  m_buffer[so] = p;
  m_currentLength += p->GetSize ();
  return true;
}
bool
NrRlcUmRxBuffer::AddLastPacket (Ptr<Packet> p, uint16_t so)
{
  if (m_buffer.find (so) != m_buffer.end ())
    {
      return false;
    }
  m_buffer[so] = p;
  m_lengthWithoutLastSeq = so;
  return true;
}
void
NrRlcUmRxBuffer::Clear ()
{
  m_buffer.clear ();
  m_lengthWithoutLastSeq = 0xffff;
  m_currentLength = 0;
}
bool
NrRlcUmRxBuffer::IsAll ()
{
  return (m_currentLength == m_lengthWithoutLastSeq);
}
Ptr<Packet>
NrRlcUmRxBuffer::GetPacket ()
{
  Ptr<Packet> packet = Create<Packet> ();
  for (auto iter = m_buffer.begin (); iter != m_buffer.end (); ++iter)
    {
      packet->AddAtEnd (iter->second);
    }
  return packet;
}
//////////////////////////////////////////////////////////////////////
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
                         MakeUintegerChecker<uint16_t> ())
          .AddAttribute ("ReassemblyTime", "Timer Reassembly Time", TimeValue (Seconds (1)),
                         MakeTimeAccessor (&NrRlcUm::m_reassemblyDelay), MakeTimeChecker ());
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
  m_rxBuffer.resize (1 << length);
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
      m_txBufferSize -= packet->GetSize ();
      if (packet->GetSize () <= lastBytes)
        {
          NS_LOG_LOGIC ("CASE 1");
          params.pdu = packet;
          m_macSapProvider->TransmitPdu (params);
          lastBytes -= packet->GetSize ();
          m_nextPduType = NrRlcUmHeader::PDU_COMPLETE;
          continue;
        }
      else
        {
          NS_LOG_LOGIC ("CASE 2");
          NrRlcUmHeader header;
          header.SetHeaderType (m_nextPduType);
          packet->RemoveHeader (header);
          NrRlcUmHeader NewHeader;
          Ptr<Packet> tmpPacket;
          if (m_nextPduType == NrRlcUmHeader::PDU_COMPLETE)
            {
              NS_LOG_LOGIC ("CASE 2.1");
              NewHeader.SetHeaderType (m_PduTypeSN);
              NewHeader.SetSegmentationInfo (NrRlcUmHeader::SI_FIRST_SEG);
              NewHeader.SetSequenceNumber (m_txNext);
              if (lastBytes <= NewHeader.GetSerializedSize ())
                {
                  packet->AddHeader (header);
                  m_txBuffer.push_front (packet);
                  m_txBufferSize += packet->GetSize ();
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
              NS_LOG_LOGIC ("CASE 2.2");
              NewHeader.SetHeaderType (m_PduTypeSNSO);
              NewHeader.SetSegmentationInfo (NrRlcUmHeader::SI_OTHER);
              NewHeader.SetSequenceNumber (header.GetSequenceNumber ());
              NewHeader.SetSegmentOffset (header.GetSegmentOffset ());
              if (lastBytes <= NewHeader.GetSerializedSize ())
                {
                  packet->AddHeader (header);
                  m_txBuffer.push_front (packet);
                  m_txBufferSize += packet->GetSize ();
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
          m_txBufferSize += packet->GetSize ();
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
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
  NrRlcUmHeader header;
  header.SetHeaderType (NrRlcUmHeader::PDU_COMPLETE);
  p->PeekHeader (header);
  NrRlcUmHeader::SIType_t si = header.GetSegmentationInfo ();
  switch (si)
    {
    case NrRlcUmHeader::SI_ALL:
      p->RemoveHeader (header);
      m_rlcSapUser->ReceivePdcpPdu (p);
      return;
    case NrRlcUmHeader::SI_FIRST_SEG:
      header.SetHeaderType (m_PduTypeSN);
      break;
    default:
      header.SetHeaderType (m_PduTypeSNSO);
      break;
    }
  p->RemoveHeader (header);
  SequenceNumber sn (header.GetSequenceNumber ());
  if (InDiscardWindow (sn))
    {
      return;
    }
  uint16_t snValue = sn.GetValue ();
  uint16_t so = header.GetSegmentOffset ();
  if (si != NrRlcUmHeader::SI_LAST_SEG)
    {
      m_rxBuffer[snValue].AddPacket (p, so);
    }
  else
    {
      m_rxBuffer[snValue].AddLastPacket (p, so);
    }
  if (m_rxBuffer[snValue].IsAll ())
    {
      m_rlcSapUser->ReceivePdcpPdu (m_rxBuffer[snValue].GetPacket ());
      // m_rxBuffer[snValue].Clear ();
      if (sn == m_rxNextReassembly)
        {
          m_rxNextReassembly = FindNext (m_rxNextReassembly + 1);
        }
    }
  else if (!InWindow (sn))
    {
      uint32_t delta = sn + 1 - m_rxNextHighest;
      m_rxNextHighest = sn + 1;
      DiscardNFrom (m_rxNextHighest - m_windowSize, delta);
      if (!InWindow (m_rxNextReassembly))
        {
          m_rxNextReassembly = FindNext (m_rxNextHighest - m_windowSize);
        }
    }
  if (m_tReassembly.IsRunning ())
    {
      if (m_rxNextReassembly - m_rxTimerTrigger < m_windowSize ||
          !InWindow (m_rxTimerTrigger) && m_rxTimerTrigger != m_rxNextHighest ||
          m_rxNextReassembly + 1 == m_rxNextHighest &&
              m_rxBuffer[m_rxNextReassembly.GetValue ()].IsAll ())
        {
          m_tReassembly.Cancel ();
        }
    }
  else
    {
      CheckAndOpenTimer ();
    }
}
void
NrRlcUm::SetupTimer (void)
{
  m_tReassembly.Cancel ();
  m_tReassembly = Timer (Timer::CANCEL_ON_DESTROY);
  m_tReassembly.SetFunction (&NrRlcUm::ExpireTimer, this);
  m_tReassembly.SetDelay (m_reassemblyDelay);
}
void
NrRlcUm::ExpireTimer (void)
{
  NS_LOG_FUNCTION (this);
  SequenceNumber old = m_rxNextReassembly;
  m_rxNextReassembly = FindNext (m_rxTimerTrigger);
  DiscardNFrom (m_rxNextReassembly, m_rxNextReassembly - old);
  CheckAndOpenTimer ();
}
void
NrRlcUm::CheckAndOpenTimer (void)
{
  if (m_rxNextReassembly + 1 - m_rxNextHighest > 0 &&
          m_rxNextReassembly + 1 - m_rxNextHighest < m_windowSize ||
      m_rxNextReassembly + 1 == m_rxNextHighest &&
          !m_rxBuffer[m_rxNextReassembly.GetValue ()].IsAll ())
    {
      SetupTimer ();
      m_tReassembly.Schedule ();
      m_rxTimerTrigger = m_rxNextHighest;
    }
}
bool
NrRlcUm::InWindow (SequenceNumber sn)
{
  NS_ASSERT_MSG (sn.GetMod () == m_snBitLen, "SN Type does not meet the requirement");
  return (sn - (m_rxNextHighest - m_windowSize) < m_windowSize && sn != m_rxNextHighest &&
          m_rxNextHighest - sn < m_windowSize);
}
bool
NrRlcUm::InDiscardWindow (SequenceNumber sn)
{
  NS_ASSERT_MSG (sn.GetMod () == m_snBitLen, "SN Type does not meet the requirement");
  return (sn - (m_rxNextHighest - m_windowSize) < m_windowSize && sn != m_rxNextReassembly &&
          m_rxNextReassembly - sn < m_windowSize);
}
void
NrRlcUm::DiscardNFrom (SequenceNumber iter, uint16_t n)
{
  while (n > 0)
    {
      --iter;
      --n;
      m_rxBuffer[iter.GetValue ()].Clear ();
    }
}
SequenceNumber
NrRlcUm::FindNext (SequenceNumber sn)
{
  while (true)
    {
      if (!m_rxBuffer[sn.GetValue ()].IsAll ())
        {
          return sn;
        }
      ++sn;
    }
}
} // namespace ns3
