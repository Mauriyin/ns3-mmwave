#ifndef NR_RLC_H
#define NR_RLC_H
#include <ns3/simple-ref-count.h>
#include <ns3/packet.h>
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/nstime.h"
#include "ns3/object.h"

#include "nr-rlc-sap.h"
#include "nr-mac-sap.h"
namespace ns3
{
class NrRlc : public Object
{
    friend class NrRlcSpecificLteMacSapUser;
    friend class NrRlcSpecificNrRlcSapProvider<NrRlc>;

  public:
    NrRlc();
    virtual ~NrRlc();
    static TypeId GetTypeId(void);
    virtual void DoDispose();

    void SetRnti(uint16_t rnti);
    void SetLcId(uint8_t lcId);
    void SetNrRlcSapUser(NrRlcSapUser *s);
    NrRlcSapProvider *GetNrRlcSapProvider();
    void SetNrMacSapProvider(NrMacSapProvider *s);
    NrMacSapUser *GetNrMacSapUser();
    typedef void (*NotifyTxTracedCallback)(uint16_t rnti, uint8_t lcid, uint32_t bytes);
    typedef void (*ReceiveTracedCallback)(uint16_t rnti, uint8_t lcid, uint32_t bytes, uint64_t delay);

//   protected:
    virtual void DoTransmitPdcpPdu(Ptr<Packet> p) = 0;

    NrRlcSapUser *m_rlcSapUser;
    NrRlcSapProvider *m_rlcSapProvider;

    // Interface forwarded by NrMacSapUser
    virtual void DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer, uint8_t harqId) = 0;
    virtual void DoNotifyHarqDeliveryFailure() = 0;
    virtual void DoNotifyHarqDeliveryFailure(uint8_t harqId);
    virtual void DoReceivePdu(Ptr<Packet> p) = 0;

    NrMacSapUser *m_macSapUser;
    NrMacSapProvider *m_macSapProvider;

    uint16_t m_rnti;
    uint8_t m_lcid;

    /**
     * Used to inform of a PDU delivery to the MAC SAP provider
     */
    TracedCallback<uint16_t, uint8_t, uint32_t> m_txPdu;
    /**
     * Used to inform of a PDU reception from the MAC SAP user
     */
    TracedCallback<uint16_t, uint8_t, uint32_t, uint64_t> m_rxPdu;
};

} // namespace ns3

#endif