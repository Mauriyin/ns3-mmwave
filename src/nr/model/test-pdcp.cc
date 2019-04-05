#include "test-pdcp.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TESTPDCP");

TypeId
NrTestPdcp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrTestPdcp").SetParent<Object> ().AddConstructor<NrTestPdcp> ();

  return tid;
}

NrTestPdcp::NrTestPdcp ()
{
  NS_LOG_FUNCTION (this);
  m_rlcSapUser = new NrRlcSpecificNrRlcSapUser<NrTestPdcp> (this);
  Simulator::ScheduleNow (&NrTestPdcp::Start, this);
}

NrTestPdcp::~NrTestPdcp ()
{
  NS_LOG_FUNCTION (this);
}

void
NrTestPdcp::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rlcSapUser;
}

void
NrTestPdcp::SetNrRlcSapProvider (NrRlcSapProvider *s)
{
  m_rlcSapProvider = s;
}

NrRlcSapUser *
NrTestPdcp::GetNrRlcSapUser (void)
{
  return m_rlcSapUser;
}

std::string
NrTestPdcp::GetDataReceived (void)
{
  NS_LOG_FUNCTION (this);

  return m_receivedData;
}

/**
 * RLC SAP
 */

void
NrTestPdcp::DoReceivePdcpPdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p->GetSize ());
  NS_LOG_LOGIC ("Data = " << (*p));

  uint32_t dataLen = p->GetSize ();
  uint8_t *buf = new uint8_t[dataLen];
  p->CopyData (buf, dataLen);
  m_receivedData += std::string ((char *) buf, dataLen);

  NS_LOG_LOGIC ("@@@ Recv: " + std::string ((char *) buf, dataLen));

  delete[] buf;
}

/**
 * START
 */

void
NrTestPdcp::Start ()
{
  NS_LOG_FUNCTION (this);
}

void
NrTestPdcp::SendData (Time time, std::string dataToSend)
{
  NS_LOG_FUNCTION (this << time << dataToSend.length () << dataToSend);

  NrRlcSapProvider::TransmitPdcpPduParameters p;
  p.rnti = 1111;
  p.lcid = 222;

  NS_LOG_LOGIC ("Data(" << dataToSend.length () << ") = " << dataToSend.data ());
  p.pdcpPdu = Create<Packet> ((uint8_t *) dataToSend.data (), dataToSend.length ());

  NS_LOG_LOGIC ("Packet(" << p.pdcpPdu->GetSize () << ")");
  Simulator::Schedule (time, &NrRlcSapProvider::TransmitPdcpPdu, m_rlcSapProvider, p);
}
} // namespace ns3