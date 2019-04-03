#ifndef NR_RLC_PDU_TAG_H
#define NR_RLC_PDU_TAG_H
#include "ns3/tag.h"
namespace ns3 {
class NrRlcPduTag : public Tag
{
public:
  NrRlcPduTag ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

private:
};
} // namespace ns3

#endif