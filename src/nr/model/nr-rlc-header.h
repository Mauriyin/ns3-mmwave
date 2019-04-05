#ifndef NR_RLC_HEADER_H
#define NR_RLC_HEADER_H
#include <ns3/packet.h>
#include "nr-rlc-sequence-number.h"

namespace ns3 {
const uint8_t SI_MASK = 0xc0;
const uint8_t SN6_MASK = 0x3f;
class NrRlcUmHeader : public Header
{
public:
  typedef enum {
    PDU_COMPLETE, //UMD PDU containing a complete RLC SDU
    PDU_SN6, //UMD PDU with 6 bit SN (NO SO)
    PDU_SN12, //UMD PDU with 12 bit SN (NO SO)
    PDU_SN6SO, //UMD PDU with 6 bit SN and with SO
    PDU_SN12SO, //UMD PDU with 12 bit SN and with SO
    PDU_UNKNOW
  } PduType_t;

  typedef enum {
    SI_ALL = 0x0, //Data field contains all bytes of an RLC SDU
    SI_FIRST_SEG = 0x40, //Data field contains the first segment of an RLC SDU
    SI_LAST_SEG = 0x80, //Data field contains the last segment of an RLC SDU
    SI_OTHER =
        0xc0, //Data field contains neither the first segment nor the last segment of an RLC SDU
    SI_UNKNOW = 0xff
  } SIType_t;

  NrRlcUmHeader ();
  virtual ~NrRlcUmHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator iter) const;
  virtual uint32_t Deserialize (Buffer::Iterator iter);
  virtual void Print (std::ostream &os) const;

  virtual void SetHeaderType (PduType_t type);
  virtual PduType_t GetHeaderType () const;
  virtual void SetSequenceNumber (SequenceNumber sn);
  virtual SequenceNumber GetSequenceNumber () const;
  virtual void SetSegmentationInfo (SIType_t si);
  virtual SIType_t GetSegmentationInfo () const;
  virtual void SetSegmentOffset (uint32_t so);
  virtual uint32_t GetSegmentOffset () const;

private:
  uint16_t m_SN;
  uint16_t m_SO;
  SIType_t m_SI;
  PduType_t m_type;
};
} // namespace ns3
#endif