#ifndef NR_RLC_SDU_TAG_H
#define NR_RLC_SDU_TAG_H
#include "ns3/tag.h"
namespace ns3 {
class NrRlcSduTag : public Tag
{
public:
  NrRlcSduTag ();
  static TypeId GetTypeId (void);
  static uint16_t GetSudId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;
  virtual void GenSduId(void);

private:
  uint16_t m_sduId;
};
} // namespace ns3

#endif