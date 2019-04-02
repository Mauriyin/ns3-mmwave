#include "ns3/log.h"
#include "nr-rlc-header.h"
namespace ns3
{
NS_LOG_COMPONENT_DEFINE("NrRlcHeader");

NS_OBJECT_ENSURE_REGISTERED(NrRlcHeader);

NrRlcHeader::NrRlcHeader()
{
}
NrRlcHeader::~NrRlcHeader(){

};
TypeId NrRlcHeader::GetTypeId(void)
{
    static TypeId tid =
        TypeId("ns3::NrRlcHeader")
            .SetParent<Header>()
            .SetGroupName("Nr")
            .AddConstructor<NrRlcHeader>();
    return tid;
}
TypeId NrRlcHeader::GetInstanceTypeId(void) const
{
    return GetTypeId();
}
uint32_t NrRlcHeader::GetSerializedSize(void) const
{
    return 0;
}
void NrRlcHeader::Serialize(Buffer::Iterator iter) const
{
}
uint32_t NrRlcHeader::Deserialize(Buffer::Iterator iter)
{
    return 0;
}
void NrRlcHeader::Print(std::ostream &os) const
{
}
} // namespace ns3
