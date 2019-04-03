#include "ns3/log.h"
#include "nr-rlc-header.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcUmHeader");

NS_OBJECT_ENSURE_REGISTERED (NrRlcUmHeader);

NrRlcUmHeader::NrRlcUmHeader ():m_SN(0)
{
}
NrRlcUmHeader::~NrRlcUmHeader (){

};
TypeId
NrRlcUmHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcUmHeader")
                          .SetParent<Header> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcUmHeader> ();
  return tid;
}
TypeId
NrRlcUmHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcUmHeader::GetSerializedSize (void) const
{
  uint8_t headerLen;
  switch (m_type)
    {
    case PDU_COMPLETE:
    case SN6:
      headerLen = 1;
      break;
    case SN12:
      headerLen = 2;
      break;
    case SN6SO:
      headerLen = 3;
      break;
    case SN12SO:
      headerLen = 4;
      break;
    default:
      NS_ABORT_MSG ("Header has not been init");
    }
  return headerLen + m_packetLen;
}
void
NrRlcUmHeader::Serialize (Buffer::Iterator iter) const
{
    
}
uint32_t
NrRlcUmHeader::Deserialize (Buffer::Iterator iter)
{
  return 0;
}
void
NrRlcUmHeader::Print (std::ostream &os) const
{
    os<<"SI: "<<m_SI<<" ";
    os<<"SN: "<<m_SN<<" ";

}

void
NrRlcUmHeader::SetHeaderType (PduType_t type)
{
  NS_LOG_FUNCTION (this << type);
  m_type = type;
}
} // namespace ns3
