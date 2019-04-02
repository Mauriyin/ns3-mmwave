#ifndef NR_RLC_UM_H
#define NR_RLC_UM_H
#include "nr-rlc.h"
namespace ns3
{
class NrRlcUm : public NrRlc
{
  public:
    NrRlcUm();
    virtual ~NrRlcUm();
    static TypeId GetTypeId(void);
    virtual void DoDispose();

    /**
   * RLC SAP
   */
    virtual void DoTransmitPdcpPdu(Ptr<Packet> p);

    /**
   * MAC SAP
   */
    virtual void DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer, uint8_t harqId);
    virtual void DoNotifyHarqDeliveryFailure();
    virtual void DoReceivePdu(Ptr<Packet> p);
};
} // namespace ns3

#endif
