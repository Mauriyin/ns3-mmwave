/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/test-pdcp.h"
#include "ns3/test-mac.h"
#include "ns3/nr-rlc-um.h"

// An essential include is test.h
#include "ns3/test.h"
#include "ns3/log.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// This is an example TestCase.
class NrTestCase1 : public TestCase
{
public:
  NrTestCase1 ();
  virtual ~NrTestCase1 ();

  virtual void DoRun (void);
  virtual void Recv (void);
  Ptr<NrTestPdcp> txPdcp;
  Ptr<NrRlc> txRlc;
  Ptr<NrTestMac> txMac;
};

// Add some help text to this case to describe what it is intended to test
NrTestCase1::NrTestCase1 () : TestCase ("Test RLC")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
NrTestCase1::~NrTestCase1 ()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
NrTestCase1::DoRun (void)
{
  LogComponentEnable ("NrRlcUm", LOG_LEVEL_ALL);
  LogComponentEnable ("TESTMAC", LOG_LEVEL_ALL);
  LogComponentEnable ("TESTPDCP", LOG_LEVEL_ALL);

  uint16_t rnti = 1111;
  uint8_t lcid = 222;

  Packet::EnablePrinting ();

  txPdcp = CreateObject<NrTestPdcp> ();

  txRlc = CreateObject<NrRlcUm> ();
  txRlc->SetRnti (rnti);
  txRlc->SetLcId (lcid);

  txMac = CreateObject<NrTestMac> ();

  txPdcp->SetNrRlcSapProvider (txRlc->GetNrRlcSapProvider ());
  txRlc->SetNrRlcSapUser (txPdcp->GetNrRlcSapUser ());

  txRlc->SetNrMacSapProvider (txMac->GetNrMacSapProvider ());
  txMac->SetNrMacSapUser (txRlc->GetNrMacSapUser ());
  txPdcp->SendData (Seconds (0), "abcdefghijklmnopqrstuvwxyz");
  txMac->SendTxOpportunity (Seconds (0.1), 26);
  txMac->SendTxOpportunity (Seconds (0.15), 6);
  txPdcp->SendData (Seconds (0.1), "0123456789");
  txMac->DoSend (Seconds (0.2));
  txMac->SendTxOpportunity (Seconds (0.3), 7);
  txMac->DoSend (Seconds (0.4));
  txMac->SendTxOpportunity (Seconds (0.5), 7);
  txMac->SendTxOpportunity (Seconds (0.55), 7);
  txMac->DoSend (Seconds (0.6));

  Simulator::Run ();
  Simulator::Stop (Seconds (5));
  Simulator::Destroy ();
}
void
NrTestCase1::Recv (void)
{
  std::string ret = txPdcp->GetDataReceived ();
  std::cout << ret << std::endl;
}

int
main (int argc, char **argv)
{
  NrTestCase1 t;
  t.DoRun ();
  return 0;
}