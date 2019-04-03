#include "ns3/nr-mac-sap.h"

namespace ns3 {

NrMacSapProvider::~NrMacSapProvider ()
{
}

NrMacSapUser::~NrMacSapUser ()
{
}

void
NrMacSapUser::NotifyDlHarqDeliveryFailure (uint8_t harqId)
{
}

void
NrMacSapUser::NotifyUlHarqDeliveryFailure (uint8_t harqId)
{
}

} // namespace ns3
