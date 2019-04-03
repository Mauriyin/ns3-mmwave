#include "ns3/log.h"
#include "nr-rlc-um.h"
#include "nr-rlc-sdu-tag.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcUm");
NS_OBJECT_ENSURE_REGISTERED (NrRlcUm);
NrRlcUm::NrRlcUm () : m_TxBufferSize (0)
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
                         MakeUintegerChecker<uint32_t> ());
  return tid;
}
void
NrRlcUm::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_txBuffer.clear ();
  m_rxBuffer.clear ();
}

void
NrRlcUm::SetSnBitLength (uint8_t length)
{
  NS_LOG_FUNCTION (this << length);
  m_TxNext.SetMod (length);
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
  if (m_TxBufferSize + p->GetSize () <= m_maxTxBufferSize)
    {
      NrRlcSduTag tag;
      tag.GenSduId ();
      p->AddPacketTag (tag);
      m_txBuffer.push_back (p);
      m_TxBufferSize += p->GetSize ();
    }
}
void
NrRlcUm::DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId)
{
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << bytes);
  Ptr<Packet> packet = m_txBuffer.front ()->Copy ();
  m_txBuffer.pop_front ();
  NrMacSapProvider::TransmitPduParameters params;
  params.pdu = packet;
  params.rnti = m_rnti;
  params.lcid = m_lcid;
  params.layer = layer;
  params.harqProcessId = harqId;
  m_macSapProvider->TransmitPdu (params);
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
