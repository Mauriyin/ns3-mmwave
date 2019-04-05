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
  virtual void Serialize (TagBuffer iter) const;
  virtual void Deserialize (TagBuffer iter);
  virtual void Print (std::ostream &os) const;

  virtual void SetPduType (uint8_t type);
  virtual uint8_t GetPduType () const;

private:
  uint8_t m_type;
};
} // namespace ns3

#endif