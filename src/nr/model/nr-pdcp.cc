#include "nr-pdcp.h"

namespace ns3 {
NrPdcp::NrPdcp ()
{
  NS_LOG_FUNCTION (this);
}
NrPdcp::~NrPdcp ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
NrPdcp::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::NrPdcp")
          .SetParent<Object> ()
          .SetGroupName ("Nr")
          .AddConstructor<NrPdcp> ()
          .AddAttribute ("MaxTxBufferSize", "Maximum Size of the Transmission Buffer (in Bytes)",
                         UintegerValue (20480), MakeUintegerAccessor (&NrPdcp::m_maxTxBufferSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("WindowSize", "Pdcp Window Size", UintegerValue (20480),
                         MakeUintegerAccessor (&NrPdcp::m_windowSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("Mtu", "The Maximum Supported Size of a PDCP SDU", UintegerValue (9000),
                         MakeUintegerAccessor (&NrPdcp::m_mtuSize),
                         MakeUintegerChecker<uint32_t> ());
  return tid;
}
void
NrPdcp::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_txNext = 0;
  m_rxNext = 0;
  m_rxDeliv = 0;
  m_rxReOrd = 0;
}
} // namespace ns3
