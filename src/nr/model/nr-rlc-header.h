#ifndef NR_RLC_HEADER_H
#define NR_RLC_HEADER_H
#include <ns3/packet.h>

namespace ns3 {

class NrRlcUmHeader : public Header
{
public:
  typedef enum {
    PDU_COMPLETE, //UMD PDU containing a complete RLC SDU
    SN6, //UMD PDU with 6 bit SN (NO SO)
    SN12, //UMD PDU with 12 bit SN (NO SO)
    SN6SO, //UMD PDU with 6 bit SN and with SO
    SN12SO, //UMD PDU with 12 bit SN and with SO
    PDU_UNKNOW
  } PduType_t;

  typedef enum {
    ALL = 0x0, //Data field contains all bytes of an RLC SDU
    FIRST_SEG = 0x1, //Data field contains the first segment of an RLC SDU
    LAST_SEG = 0x2, //Data field contains the last segment of an RLC SDU
    OTHER = 0x3 //Data field contains neither the first segment nor the last segment of an RLC SDU
  } SIType_t;

  NrRlcUmHeader ();
  virtual ~NrRlcUmHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator iter) const;
  virtual uint32_t Deserialize (Buffer::Iterator iter);
  virtual void Print (std::ostream &os) const;

  virtual void SetHeaderType(PduType_t type);

private:
  uint8_t m_SI;
  uint16_t m_SN;
  uint16_t m_SO;
  PduType_t m_type;
  SIType_t m_SIType;
  uint16_t m_packetLen;
};
} // namespace ns3
#endif