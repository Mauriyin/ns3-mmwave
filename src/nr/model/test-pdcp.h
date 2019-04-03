#ifndef TEST_PDCP_H
#define TEST_PDCP_H
#include "ns3/simulator.h"
#include "nr-rlc-sap.h"
namespace ns3 {

class NrTestPdcp : public Object
{
  friend class NrRlcSpecificNrRlcSapUser<NrTestPdcp>;

public:
  static TypeId GetTypeId (void);

  NrTestPdcp (void);
  virtual ~NrTestPdcp (void);
  virtual void DoDispose (void);

  /**
    * \brief Set the RLC SAP provider
    * \param s a pointer to the RLC SAP provider
    */
  void SetNrRlcSapProvider (NrRlcSapProvider *s);
  /**
    * \brief Get the RLC SAP user
    * \return a pointer to the SAP user of the RLC
    */
  NrRlcSapUser *GetNrRlcSapUser (void);

  void Start ();

  void SendData (Time time, std::string dataToSend);
  std::string GetDataReceived (void);

private:
  // Interface forwarded by NrRlcSapUser
  virtual void DoReceivePdcpPdu (Ptr<Packet> p);

  NrRlcSapUser *m_rlcSapUser;
  NrRlcSapProvider *m_rlcSapProvider;

  std::string m_receivedData;
};
} // namespace ns3
#endif