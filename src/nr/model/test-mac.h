#ifndef TEST_MAC_H
#define TEST_MAC_H
#include "ns3/simulator.h"
class NrTestMac : public Object
{
	friend class EnbMacMemberNrMacSapProvider<NrTestMac>;

  public:
	static TypeId GetTypeId(void);

	NrTestMac(void);
	virtual ~NrTestMac(void);
	virtual void DoDispose(void);

	void SetDevice(Ptr<NetDevice> device);

	void SendTxOpportunity(Time, uint32_t);
	std::string GetDataReceived(void);

	bool Receive(Ptr<NetDevice> nd, Ptr<const Packet> p, uint16_t protocol, const Address &addr);

	/**
     * \brief Set the MAC SAP user
     * \param s a pointer to the MAC SAP user
     */
	void SetNrMacSapUser(NrMacSapUser *s);
	/**
     * \brief Get the MAC SAP provider
     * \return a pointer to the SAP provider of the MAC
     */
	NrMacSapProvider *GetNrMacSapProvider(void);

	/**
     * \brief Set the other side of the MAC Loopback
     * \param s a pointer to the other side of the MAC loopback
     */
	void SetNrMacLoopback(Ptr<NrTestMac> s);

	/**
     *
     */
	void SetPdcpHeaderPresent(bool present);

	/**
     *
     */
	void SetRlcHeaderType(uint8_t rlcHeaderType);

	typedef enum
	{
		UM_RLC_HEADER = 0,
		AM_RLC_HEADER = 1,
	} RlcHeaderType_t;

	/**
     *
     */
	void SetTxOpportunityMode(uint8_t mode);

	typedef enum
	{
		MANUAL_MODE = 0,
		AUTOMATIC_MODE = 1,
		RANDOM_MODE = 2
	} TxOpportunityMode_t;

	void SetTxOppTime(Time txOppTime);
	void SetTxOppSize(uint32_t txOppSize);

	// Stats
	uint32_t GetTxPdus(void);
	uint32_t GetTxBytes(void);
	uint32_t GetRxPdus(void);
	uint32_t GetRxBytes(void);

  private:
	// forwarded from NrMacSapProvider
	void DoTransmitPdu(NrMacSapProvider::TransmitPduParameters);
	void DoReportBufferStatus(NrMacSapProvider::ReportBufferStatusParameters);

	NrMacSapProvider *m_macSapProvider;
	NrMacSapUser *m_macSapUser;
	Ptr<NrTestMac> m_macLoopback;

	std::string m_receivedData;

	uint8_t m_rlcHeaderType;
	bool m_pdcpHeaderPresent;
	uint8_t m_txOpportunityMode;

	Ptr<NetDevice> m_device;

	// TxOpportunity configuration
	EventId m_nextTxOpp;
	Time m_txOppTime;
	uint32_t m_txOppSize;
	std::list<EventId> m_nextTxOppList;

	// Stats
	uint32_t m_txPdus;
	uint32_t m_txBytes;
	uint32_t m_rxPdus;
	uint32_t m_rxBytes;
};
#endif