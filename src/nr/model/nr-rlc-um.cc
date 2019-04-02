#include"ns3/log.h"
#include "nr-rlc-um.h"
namespace ns3
{
NS_LOG_COMPONENT_DEFINE("NrRlcUm");
NrRlcUm::NrRlcUm()
{
    NS_LOG_FUNCTION(this);
}
NrRlcUm::~NrRlcUm()
{
    NS_LOG_FUNCTION(this);
}
TypeId NrRlcUm::GetTypeId(void)
{
    static TypeId tid =
        TypeId("ns3::NrRlcUm")
            .SetParent<NrRlc>()
            .SetGroupName("Nr")
            .AddConstructor<NrRlcUm>();
    return tid;
}
void NrRlcUm::DoDispose()
{
    NS_LOG_FUNCTION(this);
}

void NrRlcUm::DoTransmitPdcpPdu(Ptr<Packet> p)
{
    NS_LOG_FUNCTION(this << (uint32_t)m_rnti << (uint32_t)m_lcid << p->GetSize());
}
void NrRlcUm::DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer, uint8_t harqId)
{
    NS_LOG_FUNCTION(this << (uint32_t)m_rnti << (uint32_t)m_lcid << bytes);
    Ptr<Packet> packet = Create<Packet>();
}
void NrRlcUm::DoNotifyHarqDeliveryFailure()
{
    NS_LOG_FUNCTION(this);
}
void NrRlcUm::DoReceivePdu(Ptr<Packet> p)
{
    NS_LOG_FUNCTION(this << (uint32_t)m_rnti << (uint32_t)m_lcid << p->GetSize());
}
} // namespace ns3
