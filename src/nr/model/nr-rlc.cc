#include "ns3/log.h"
#include "ns3/simulator.h"
#include "nr-rlc.h"
#include "nr-rlc-sap.h"
#include "nr-mac-sap.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlc");

class NrRlcSpecificNrMacSapUser : public NrMacSapUser
{
public:
  NrRlcSpecificNrMacSapUser (NrRlc *rlc);

  // Interface implemented from NrMacSapUser
  virtual void NotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId);
  virtual void NotifyHarqDeliveryFailure ();
  virtual void NotifyHarqDeliveryFailure (uint8_t harqId);
  virtual void ReceivePdu (Ptr<Packet> p);

private:
  NrRlcSpecificNrMacSapUser ();
  NrRlc *m_rlc;
};

NrRlcSpecificNrMacSapUser::NrRlcSpecificNrMacSapUser (NrRlc *rlc) : m_rlc (rlc)
{
}

NrRlcSpecificNrMacSapUser::NrRlcSpecificNrMacSapUser ()
{
}

void
NrRlcSpecificNrMacSapUser::NotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId)
{
  m_rlc->DoNotifyTxOpportunity (bytes, layer, harqId);
}

void
NrRlcSpecificNrMacSapUser::NotifyHarqDeliveryFailure ()
{
  m_rlc->DoNotifyHarqDeliveryFailure ();
}

void
NrRlcSpecificNrMacSapUser::NotifyHarqDeliveryFailure (uint8_t harqId)
{
  m_rlc->DoNotifyHarqDeliveryFailure (harqId);
}

void
NrRlcSpecificNrMacSapUser::ReceivePdu (Ptr<Packet> p)
{
  m_rlc->DoReceivePdu (p);
}

/////////////////////////////////////////////////////
NS_OBJECT_ENSURE_REGISTERED (NrRlc);

TypeId
NrRlc::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::NrRlc")
          .SetParent<Object> ()
          .SetGroupName ("Nr")
          .AddTraceSource ("TxPDU", "PDU transmission notified to the MAC.",
                           MakeTraceSourceAccessor (&NrRlc::m_txPdu),
                           "ns3::NrRlc::NotifyTxTracedCallback")
          .AddTraceSource ("RxPDU", "PDU received.", MakeTraceSourceAccessor (&NrRlc::m_rxPdu),
                           "ns3::NrRlc::ReceiveTracedCallback");
  return tid;
}
void
NrRlc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete (m_rlcSapProvider);
  delete (m_macSapUser);
}

NrRlc::NrRlc () : m_rlcSapUser (0), m_macSapProvider (0), m_rnti (0), m_lcid (0)
{
  NS_LOG_FUNCTION (this);
  m_rlcSapProvider = new NrRlcSpecificNrRlcSapProvider<NrRlc> (this);
  m_macSapUser = new NrRlcSpecificNrMacSapUser (this);
}

NrRlc::~NrRlc ()
{
  delete m_rlcSapProvider;
  delete m_macSapUser;
}

void
NrRlc::SetRnti (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  m_rnti = rnti;
}
void
NrRlc::SetLcId (uint8_t lcId)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcId);
  m_lcid = lcId;
}
void
NrRlc::SetNrRlcSapUser (NrRlcSapUser *s)
{
  NS_LOG_FUNCTION (this << s);
  m_rlcSapUser = s;
}

NrRlcSapProvider *
NrRlc::GetNrRlcSapProvider ()
{
  NS_LOG_FUNCTION (this);
  return m_rlcSapProvider;
}

void
NrRlc::SetNrMacSapProvider (NrMacSapProvider *s)
{
  NS_LOG_FUNCTION (this << s);
  m_macSapProvider = s;
}

NrMacSapUser *
NrRlc::GetNrMacSapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_macSapUser;
}

void
NrRlc::DoNotifyHarqDeliveryFailure (uint8_t harqId)
{
  NS_LOG_FUNCTION (this);
}
} // namespace ns3
