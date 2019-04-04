#ifndef NR_RLC_UM_H
#define NR_RLC_UM_H
#include "nr-rlc.h"
#include "nr-rlc-sequence-number.h"
#include "nr-rlc-header.h"
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
  virtual uint8_t GetSnBitLength (void) const;
  virtual void SetUmWindowSize (uint16_t size);
  virtual void SetTxBufferSize (uint32_t size);

private:
  void ExpireTimer (void);
  uint16_t m_windowSize;
  uint32_t m_maxTxBufferSize;
  uint32_t m_txBufferSize;
  uint8_t m_snBitLen;

  std::list<Ptr<Packet>> m_txBuffer;
  std::map<uint16_t, Ptr<Packet>> m_rxBuffer;

  SequenceNumber m_txNext;
  SequenceNumber m_rxNextHighest;
  SequenceNumber m_rxNextReassembly;
  SequenceNumber m_rxTimerTrigger;

  Timer m_tReassembly;

  NrRlcUmHeader::PduType_t m_nextPduType;
};
} // namespace ns3

#endif
