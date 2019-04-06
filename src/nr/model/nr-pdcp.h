#ifndef NR_PDCP_H
#define NR_PDCP_H
#include "ns3/simple-ref-count.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
namespace ns3 {
class NrPdcp : public Object
{
public:
  NrRlc ();
  virtual ~NrRlc ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

private:
  uint32_t m_windowSize;
  uint32_t m_maxTxBufferSize;
  uint32_t m_mtuSize;

  SequenceNumber m_txNext;
  SequenceNumber m_rxNext;
  SequenceNumber m_rxDeliv;
  SequenceNumber m_rxReOrd;

  Timer m_discardTimer;
  Timer m_tReordering;
};
} // namespace ns3

#endif