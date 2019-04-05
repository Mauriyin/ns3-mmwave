#ifndef TEST_MAC_H
#define TEST_MAC_H
#include "ns3/simulator.h"
#include "nr-mac-sap.h"
namespace ns3 {
class NrTestMac : public Object
{
  friend class EnbMacMemberNrMacSapProvider<NrTestMac>;

public:
  static TypeId GetTypeId (void);

  NrTestMac (void);
  virtual ~NrTestMac (void);
  virtual void DoDispose (void);

  void SendTxOpportunity (Time, uint32_t);

  /**
     * \brief Set the MAC SAP user
     * \param s a pointer to the MAC SAP user
     */
  void SetNrMacSapUser (NrMacSapUser *s);
  /**
     * \brief Get the MAC SAP provider
     * \return a pointer to the SAP provider of the MAC
     */
  NrMacSapProvider *GetNrMacSapProvider (void);

  virtual void DoSend(Time tm);

private:
  virtual void SendAll();
  // forwarded from NrMacSapProvider
  void DoTransmitPdu (NrMacSapProvider::TransmitPduParameters);
  void DoReportBufferStatus (NrMacSapProvider::ReportBufferStatusParameters);

  NrMacSapProvider *m_macSapProvider;
  NrMacSapUser *m_macSapUser;
  std::vector<Ptr<Packet>> m_buffer;
};
} // namespace ns3
#endif