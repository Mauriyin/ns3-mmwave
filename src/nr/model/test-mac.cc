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

#include "test-mac.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "nr-mac-sap.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("TESTMAC");
TypeId
NrTestMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrTestMac").SetParent<Object> ().AddConstructor<NrTestMac> ();

  return tid;
}
NrTestMac::NrTestMac (void)
{
  NS_LOG_FUNCTION (this);
  m_macSapProvider = new EnbMacMemberNrMacSapProvider<NrTestMac> (this);
}
NrTestMac::~NrTestMac (void)
{
  NS_LOG_FUNCTION (this);
}
void
NrTestMac::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  delete m_macSapProvider;
}
void
NrTestMac::SendTxOpportunity (Time time, uint32_t bytes)
{
  NS_LOG_FUNCTION (this << time << " " << bytes);
  Simulator::Schedule (time, &NrMacSapUser::NotifyTxOpportunity, m_macSapUser, bytes, 0, 0);
}
void
NrTestMac::SetNrMacSapUser (NrMacSapUser *s)
{
  m_macSapUser = s;
}
NrMacSapProvider *
NrTestMac::GetNrMacSapProvider (void)
{
  return m_macSapProvider;
}
void
NrTestMac::DoTransmitPdu (NrMacSapProvider::TransmitPduParameters params)
{
  NS_LOG_FUNCTION (this);
  m_buffer.push_back (params.pdu->Copy ());
}
void NrTestMac::DoReportBufferStatus (NrMacSapProvider::ReportBufferStatusParameters)
{
  NS_LOG_FUNCTION (this);
}
void
NrTestMac::DoSend (Time tm)
{
  Simulator::Schedule (tm, &NrTestMac::SendAll, this);
}
void
NrTestMac::SendAll ()
{
  NS_LOG_FUNCTION (this);
  for (auto each : m_buffer)
    {
      Simulator::ScheduleNow (&NrMacSapUser::ReceivePdu, m_macSapUser, each);
    }
  m_buffer.clear ();
}
} // namespace ns3
