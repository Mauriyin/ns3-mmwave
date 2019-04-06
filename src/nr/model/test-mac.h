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

  virtual void DoSend (Time tm);

private:
  virtual void SendAll ();
  // forwarded from NrMacSapProvider
  void DoTransmitPdu (NrMacSapProvider::TransmitPduParameters);
  void DoReportBufferStatus (NrMacSapProvider::ReportBufferStatusParameters);

  NrMacSapProvider *m_macSapProvider;
  NrMacSapUser *m_macSapUser;
  std::vector<Ptr<Packet>> m_buffer;
};
} // namespace ns3
#endif