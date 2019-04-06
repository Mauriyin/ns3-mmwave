#ifndef NR_PDCP_HEADER_H
#define NR_PDCP_HEADER_H
#include "ns3/packet.h"
namespace ns3 {
class NrPdcpHeader : public Header
{
public:
  NrPdcpHeader ();
  virtual ~NrPdcpHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator iter) const;
  virtual uint32_t Deserialize (Buffer::Iterator iter);
  virtual void Print (std::ostream &os) const;

private:
};
} // namespace ns3

#endif