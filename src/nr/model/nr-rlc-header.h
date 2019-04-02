#ifndef NR_RLC_HEADER_H
#define NR_RLC_HEADER_H
#include <ns3/packet.h>

namespace ns3
{
class NrRlcHeader : public Header
{
  public:
    NrRlcHeader();
    virtual ~NrRlcHeader();
    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    virtual uint32_t GetSerializedSize(void) const;
    virtual void Serialize(Buffer::Iterator iter) const;
    virtual uint32_t Deserialize(Buffer::Iterator iter);
    virtual void Print(std::ostream &os) const;
};

class NrRlcUmHeader : public Header
{
  public:
    NrRlcUmHeader();
    virtual ~NrRlcUmHeader();
    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    virtual uint32_t GetSerializedSize(void) const;
    virtual void Serialize(Buffer::Iterator iter) const;
    virtual uint32_t Deserialize(Buffer::Iterator iter);
    virtual void Print(std::ostream &os) const;

  private:
  uint8_t m_SI;
};
} // namespace ns3
#endif