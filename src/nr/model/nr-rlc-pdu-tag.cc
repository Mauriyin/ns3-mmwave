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
  return 1;
}
void
NrRlcPduTag::Serialize (TagBuffer iter) const
{
  iter.WriteU8 (m_type);
}
void
NrRlcPduTag::Deserialize (TagBuffer iter)
{
  m_type = iter.ReadU8 ();
}
void
NrRlcPduTag::Print (std::ostream &os) const
{
  os << m_type;
}
void
NrRlcPduTag::SetPduType (uint8_t type)
{
  m_type = type;
}
uint8_t
NrRlcPduTag::GetPduType () const
{
  return m_type;
}
} // namespace ns3
