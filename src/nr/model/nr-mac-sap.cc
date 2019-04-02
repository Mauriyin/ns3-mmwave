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

template <class C>
EnbMacMemberNrMacSapProvider<C>::EnbMacMemberNrMacSapProvider(C *mac)
	: m_mac(mac)
{
}

template <class C>
void EnbMacMemberNrMacSapProvider<C>::TransmitPdu(TransmitPduParameters params)
{
	m_mac->DoTransmitPdu(params);
}

template <class C>
void EnbMacMemberNrMacSapProvider<C>::ReportBufferStatus(ReportBufferStatusParameters params)
{
	m_mac->DoReportBufferStatus(params);
}

} // namespace ns3
