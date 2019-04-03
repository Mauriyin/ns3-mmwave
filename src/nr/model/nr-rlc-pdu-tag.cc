#include "nr-rlc-pdu-tag.h"
namespace ns3 {
NrRlcPduTag::NrRlcPduTag ()
{
}
TypeId
NrRlcPduTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcPduTag")
                          .SetParent<Tag> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcPduTag> ();
  return tid;
}
TypeId
NrRlcPduTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcPduTag::GetSerializedSize (void) const
{
  return 0;
}
void
NrRlcPduTag::Serialize (TagBuffer i) const
{
}
void
NrRlcPduTag::Deserialize (TagBuffer i)
{
}
void
NrRlcPduTag::Print (std::ostream &os) const
{
}
} // namespace ns3
