#include "nr-rlc-sdu-tag.h"
namespace ns3 {
NrRlcSduTag::NrRlcSduTag ()
{
}
TypeId
NrRlcSduTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcSduTag")
                          .SetParent<Tag> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcSduTag> ();
  return tid;
}
uint16_t
NrRlcSduTag::GetSudId (void)
{
  static uint16_t sduId = 0;
  return sduId++;
}

TypeId
NrRlcSduTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcSduTag::GetSerializedSize (void) const
{
  return 2;
}
void
NrRlcSduTag::Serialize (TagBuffer i) const
{
  i.WriteU16 (m_sduId);
}
void
NrRlcSduTag::Deserialize (TagBuffer i)
{
  m_sduId = i.ReadU16 ();
}
void
NrRlcSduTag::Print (std::ostream &os) const
{
  os << m_sduId;
}
void
NrRlcSduTag::GenSduId (void)
{
  m_sduId = GetSudId ();
}
} // namespace ns3
