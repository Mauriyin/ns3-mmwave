#ifndef NR_RLC_SAP_H
#define NR_RLC_SAP_H

#include "ns3/packet.h"

namespace ns3
{

/**
 * Service Access Point (SAP) offered by the UM-RLC and AM-RLC entities to the PDCP entity
 * See 3GPP 38.322 Radio Link Control (RLC) protocol specification
 *
 * This is the RLC SAP Provider
 * (i.e. the part of the SAP that contains the RLC methods called by the PDCP)
 */
class NrRlcSapProvider
{
public:
	virtual ~NrRlcSapProvider();

	/**
     * Parameters for NrRlcSapProvider::TransmitPdcpPdu
     */
	struct TransmitPdcpPduParameters
	{
		Ptr<Packet> pdcpPdu; /**< the PDCP PDU */
		uint16_t rnti;			 /**< the C-RNTI identifying the UE */
		uint8_t lcid;				 /**< the logical channel id corresponding to the sending RLC instance */
	};

	/**
     * Send a PDCP PDU to the RLC for transmission
     * This method is to be called
     * when upper PDCP entity has a PDCP PDU ready to send
     */
	virtual void TransmitPdcpPdu(TransmitPdcpPduParameters params) = 0;
};

/**
 * Service Access Point (SAP) offered by the UM-RLC and AM-RLC entities to the PDCP entity
 * See 3GPP 38.322 Radio Link Control (RLC) protocol specification
 *
 * This is the RLC SAP User
 * (i.e. the part of the SAP that contains the PDCP methos called by the RLC)
 */
class NrRlcSapUser
{
public:
	virtual ~NrRlcSapUser();

	/**
    * Called by the RLC entity to notify the PDCP entity of the reception of a new PDCP PDU
    *
    * \param p the PDCP PDU
    */
	virtual void ReceivePdcpPdu(Ptr<Packet> p) = 0;
};

///////////////////////////////////////

template <class C>
class NrRlcSpecificNrRlcSapProvider : public NrRlcSapProvider
{
public:
	NrRlcSpecificNrRlcSapProvider(C *rlc);

	// Interface implemented from NrRlcSapProvider
	virtual void TransmitPdcpPdu(TransmitPdcpPduParameters params);

private:
	NrRlcSpecificNrRlcSapProvider();
	C *m_rlc;
};

template <class C>
NrRlcSpecificNrRlcSapProvider<C>::NrRlcSpecificNrRlcSapProvider(C *rlc)
		: m_rlc(rlc)
{
}

template <class C>
NrRlcSpecificNrRlcSapProvider<C>::NrRlcSpecificNrRlcSapProvider()
{
}

template <class C>
void NrRlcSpecificNrRlcSapProvider<C>::TransmitPdcpPdu(TransmitPdcpPduParameters params)
{
	m_rlc->DoTransmitPdcpPdu(params.pdcpPdu);
}

///////////////////////////////////////

template <class C>
class NrRlcSpecificNrRlcSapUser : public NrRlcSapUser
{
public:
	NrRlcSpecificNrRlcSapUser(C *pdcp);

	// Interface implemented from NrRlcSapUser
	virtual void ReceivePdcpPdu(Ptr<Packet> p);

private:
	NrRlcSpecificNrRlcSapUser();
	C *m_pdcp;
};

template <class C>
NrRlcSpecificNrRlcSapUser<C>::NrRlcSpecificNrRlcSapUser(C *pdcp)
		: m_pdcp(pdcp)
{
}

template <class C>
NrRlcSpecificNrRlcSapUser<C>::NrRlcSpecificNrRlcSapUser()
{
}

template <class C>
void NrRlcSpecificNrRlcSapUser<C>::ReceivePdcpPdu(Ptr<Packet> p)
{
	m_pdcp->DoReceivePdcpPdu(p);
}

} // namespace ns3

#endif // Nr_RLC_SAP_H
