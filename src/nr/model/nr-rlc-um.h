#ifndef NR_RLC_UM_H
#define NR_RLC_UM_H
#include "nr-rlc.h"
#include "nr-rlc-sequence-number.h"
#include "ns3/timer.h"
#include "ns3/packet.h"
#include <map>
namespace ns3 {
class NrRlcUm : public NrRlc
{
public:
  NrRlcUm ();
  virtual ~NrRlcUm ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * RLC SAP
   */
  virtual void DoTransmitPdcpPdu (Ptr<Packet> p);

  /**
   * MAC SAP
   */
  virtual void DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId);
  virtual void DoNotifyHarqDeliveryFailure ();
  virtual void DoReceivePdu (Ptr<Packet> p);

  virtual void SetSnBitLength (uint8_t length);
  virtual void SetUmWindowSize (uint16_t size);
  virtual void SetTxBufferSize (uint32_t size);

private:
  void ExpireTimer (void);
  uint16_t m_windowSize;
  uint32_t m_maxTxBufferSize;
  uint32_t m_TxBufferSize;

  std::list<Ptr<Packet>> m_txBuffer;
  std::map<uint16_t, Ptr<Packet>> m_rxBuffer;

  SequenceNumber m_TxNext;
  SequenceNumber m_RxNextHighest;
  SequenceNumber m_RxNextReassembly;
  SequenceNumber m_RxTimerTrigger;

  Timer m_tReassembly;
};
} // namespace ns3

#endif
