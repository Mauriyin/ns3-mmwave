/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/test-pdcp.h"
#include "ns3/test-mac.h"
#include "ns3/nr-rlc-um.h"

// An essential include is test.h
#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// This is an example TestCase.
class NrTestCase1 : public TestCase
{
public:
  NrTestCase1 ();
  virtual ~NrTestCase1 ();

private:
  virtual void DoRun (void);
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
  uint16_t rnti = 1111;
  uint8_t lcid = 222;

  Packet::EnablePrinting ();

  Ptr<NrTestPdcp> txPdcp = CreateObject<NrTestPdcp> ();

  Ptr<NrRlc> txRlc = CreateObject<NrRlcUm> ();
  txRlc->SetRnti (rnti);
  txRlc->SetLcId (lcid);

  Ptr<NrTestMac> txMac=CreateObject<NrTestMac>();

}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class NrTestSuite : public TestSuite
{
public:
  NrTestSuite ();
};

NrTestSuite::NrTestSuite () : TestSuite ("nr", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new NrTestCase1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static NrTestSuite nrTestSuite;
