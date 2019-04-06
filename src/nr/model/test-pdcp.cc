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

#include "test-pdcp.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TESTPDCP");

TypeId
NrTestPdcp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrTestPdcp").SetParent<Object> ().AddConstructor<NrTestPdcp> ();

  return tid;
}

NrTestPdcp::NrTestPdcp ()
{
  NS_LOG_FUNCTION (this);
  m_rlcSapUser = new NrRlcSpecificNrRlcSapUser<NrTestPdcp> (this);
  Simulator::ScheduleNow (&NrTestPdcp::Start, this);
}

NrTestPdcp::~NrTestPdcp ()
{
  NS_LOG_FUNCTION (this);
}

void
NrTestPdcp::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rlcSapUser;
}

void
NrTestPdcp::SetNrRlcSapProvider (NrRlcSapProvider *s)
{
  m_rlcSapProvider = s;
}

NrRlcSapUser *
NrTestPdcp::GetNrRlcSapUser (void)
{
  return m_rlcSapUser;
}

std::string
NrTestPdcp::GetDataReceived (void)
{
  NS_LOG_FUNCTION (this);

  return m_receivedData;
}

/**
 * RLC SAP
 */

void
NrTestPdcp::DoReceivePdcpPdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p->GetSize ());
  NS_LOG_LOGIC ("Data = " << (*p));

  uint32_t dataLen = p->GetSize ();
  uint8_t *buf = new uint8_t[dataLen];
  p->CopyData (buf, dataLen);
  m_receivedData += std::string ((char *) buf, dataLen);

  NS_LOG_LOGIC ("@@@ Recv: " + std::string ((char *) buf, dataLen));

  delete[] buf;
}

/**
 * START
 */

void
NrTestPdcp::Start ()
{
  NS_LOG_FUNCTION (this);
}

void
NrTestPdcp::SendData (Time time, std::string dataToSend)
{
  NS_LOG_FUNCTION (this << time << dataToSend.length () << dataToSend);

  NrRlcSapProvider::TransmitPdcpPduParameters p;
  p.rnti = 1111;
  p.lcid = 222;

  NS_LOG_LOGIC ("Data(" << dataToSend.length () << ") = " << dataToSend.data ());
  p.pdcpPdu = Create<Packet> ((uint8_t *) dataToSend.data (), dataToSend.length ());

  NS_LOG_LOGIC ("Packet(" << p.pdcpPdu->GetSize () << ")");
  Simulator::Schedule (time, &NrRlcSapProvider::TransmitPdcpPdu, m_rlcSapProvider, p);
}
} // namespace ns3