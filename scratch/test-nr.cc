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
  virtual std::string GenPacket (uint16_t);
  void checkRecv ();
  Ptr<NrTestPdcp> txPdcp;
  Ptr<NrRlc> txRlc;
  Ptr<NrTestMac> txMac;
  std::string m_recv;
  uint32_t m_sendLen;
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
  // LogComponentEnable ("NrRlcUm", LOG_LEVEL_ALL);
  // LogComponentEnable ("TESTMAC", LOG_LEVEL_ALL);
  // LogComponentEnable ("TESTPDCP", LOG_LEVEL_ALL);

  srand (time (0));

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

  // txPdcp->SendData (Seconds (0), "abcdefghijklmnopqrstuvwxyz");
  // txPdcp->SendData (Seconds (0.1), "0123456789");
  // txMac->SendTxOpportunity (Seconds (0.11), 20);
  // txMac->DoSend (Seconds (0.2));
  // txMac->SendTxOpportunity (Seconds (0.21), 10);
  // txMac->SendTxOpportunity (Seconds (0.25), 8);
  // txMac->SendTxOpportunity (Seconds (0.3), 7);
  // txMac->DoSend (Seconds (2));
  // txMac->SendTxOpportunity (Seconds (2.5), 7);
  // //txMac->SendTxOpportunity (Seconds (0.55), 7);
  // txMac->SendTxOpportunity (Seconds (2.7), 7);
  // txMac->DoSend (Seconds (3));

  m_sendLen = 0;
  for (int i = 0; i < 100000; ++i)
    {
      int tmp = 128 + (rand () % 20 - 14);
      m_sendLen += tmp;
      txPdcp->SendData (Seconds (i / 100.0), GenPacket (tmp));
      if (i % 3 == 2)
        {
          txMac->SendTxOpportunity (Seconds (i / 100.0 + 0.0050), 128);
          txMac->SendTxOpportunity (Seconds (i / 100.0 + 0.0051), 64);
          txMac->SendTxOpportunity (Seconds (i / 100.0 + 0.0052), 128 * 2);
        }
    }
  txMac->SendTxOpportunity (Seconds (9997), 10240);
  txMac->DoSend (Seconds (9998));
  Simulator::Schedule (Seconds (9999), &NrTestCase1::checkRecv, this);
  Simulator::Run ();
  Simulator::Stop (Seconds (10000));
  Simulator::Destroy ();
}

std::string
NrTestCase1::GenPacket (uint16_t len)
{
  static int index;
  std::string ret;
  while (len > 0)
    {
      --len;
      ret += (char) ('A' + index++ % 26);
    }
  return ret;
}
void
NrTestCase1::checkRecv ()
{
  m_recv = txPdcp->GetDataReceived ();
  std::cout << "Send Length: " << m_sendLen << std::endl;
  std::cout << "Recv Length: " << m_recv.length () << std::endl;
  for (std::__cxx11::basic_string<char>::size_type i = 0; i < m_recv.length (); i += 26 * 100 + 1)
    std::cout << m_recv[i];
  int index = 0;
  for (std::__cxx11::basic_string<char>::size_type i = 0; i < m_recv.length (); ++i)
    {
      if (m_recv[i] != (char) ('A' + index++ % 26))
        {
          std::cout << std::endl << i << std::endl;
          puts ("ERROR");
          break;
        }
    }
  puts ("");
}

int
main (int argc, char **argv)
{
  NrTestCase1 t;
  t.DoRun ();
  return 0;
}