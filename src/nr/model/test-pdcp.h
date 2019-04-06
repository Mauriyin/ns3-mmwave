/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2019 Huazhong University of Science and Technology (HUST)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Pengyu Liu <eicliupengyu@gmail.com>
 */

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