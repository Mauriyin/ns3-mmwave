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

#include "ns3/log.h"
#include "nr-rlc-um.h"
#include "nr-rlc-pdu-tag.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcAm");

NS_OBJECT_ENSURE_REGISTERED (NrRlcAm);
NrRlcAm::NrRlcAm ()
{
  NS_LOG_FUNCTION (this);
}
NrRlcAm::~NrRlcAm ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
NrRlcAm::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::NrRlcAm").SetParent<NrRlc> ().SetGroupName ("Nr").AddConstructor<NrRlcAm> ();
  return tid;
}
void
NrRlcAm::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}

///////////////////////////////
void
NrRlcAm::DoTransmitPdcpPdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
}
void
NrRlcAm::DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId)
{
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << bytes);
}
void
NrRlcAm::DoNotifyHarqDeliveryFailure ()
{
  NS_LOG_FUNCTION (this);
}
void
NrRlcAm::DoReceivePdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << (uint32_t) m_rnti << (uint32_t) m_lcid << p->GetSize ());
}
} // namespace ns3
