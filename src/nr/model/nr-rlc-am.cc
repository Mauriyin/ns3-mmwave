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
#include "nr-rlc-am.h"
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
      TypeId ("ns3::NrRlcAm")
          .SetParent<NrRlc> ()
          .SetGroupName ("Nr")
          .AddConstructor<NrRlcAm> ()
          .AddAttribute ("MaxTxBufferSize", "Maximum Size of the Transmission Buffer (in Bytes)",
                         UintegerValue (10 * 1024),
                         MakeUintegerAccessor (&NrRlcAm::m_maxTxBufferSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("SnBitLen", "Length of SN (in Bits)", UintegerValue (18),
                         MakeUintegerAccessor (&NrRlcAm::SetSnBitLength, &NrRlcAm::GetSnBitLength),
                         MakeUintegerChecker<uint8_t> (12, 18))
          .AddAttribute ("WindowSize",
                         "This constant is used by both the transmitting side and the receiving "
                         "side of each AM RLC entity. AM_Window_Size = 2048 when a 12 bit SN is "
                         "used, AM_Window_Size = 131072 when an 18 bit SN is used.",
                         UintegerValue (1 << 17), MakeUintegerAccessor (&NrRlcAm::m_windowSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("PduWithoutPoll",
                         "This parameter is used by the transmitting side of each AM RLC entity to "
                         "trigger a poll for every pollPDU PDUs",
                         UintegerValue (233), MakeUintegerAccessor (&NrRlcAm::m_pollPDU),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("ByteWithoutPoll",
                         "This parameter is used by the transmitting side of each AM RLC entity to "
                         "trigger a poll for every pollByte bytes",
                         UintegerValue (233), MakeUintegerAccessor (&NrRlcAm::m_pollByte),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute (
              "MaxRetxThreshold",
              "This parameter is used by the transmitting side of each AM RLC entity to limit the "
              "number of retransmissions corresponding to an RLC SDU, including its segments",
              UintegerValue (233), MakeUintegerAccessor (&NrRlcAm::m_maxRetxThreshold),
              MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("PollRetransmit", "Timer t-PollRetransmit",
                         TimeValue (Seconds (1)),
                         MakeTimeAccessor (&NrRlcAm::m_tPollRetransmitTime), MakeTimeChecker ())
          .AddAttribute ("Reassembly", "Timer t-Reassembly",
                         TimeValue (Seconds (1)),
                         MakeTimeAccessor (&NrRlcAm::m_tReassemblyTime), MakeTimeChecker ())
          .AddAttribute ("StatusProhibit", "Timer t-StatusProhibit",
                         TimeValue (Seconds (1)),
                         MakeTimeAccessor (&NrRlcAm::m_tStatusProhibitTime), MakeTimeChecker ());
  return tid;
}
void
NrRlcAm::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}
void
NrRlcAm::SetSnBitLength (uint8_t length)
{
  NS_LOG_FUNCTION (this << length);

  if (length == 12)
    {
      m_PduTypeSN = NrRlcAmHeader::PDU_SN12;
      m_PduTypeSNSO = NrRlcAmHeader::PDU_SN12SO;
      m_PduTypeStatus = NrRlcAmHeader::PDU_STATUS_SN12;
    }
  else if (length == 18)
    {
      m_PduTypeSN = NrRlcAmHeader::PDU_SN18;
      m_PduTypeSNSO = NrRlcAmHeader::PDU_SN18SO;
      m_PduTypeStatus = NrRlcAmHeader::PDU_STATUS_SN18;
    }
  else
    {
      NS_ABORT_MSG ("SN length not support");
      return;
    }

  m_snBitLen = length;
  m_txNextAck.SetMod (length);
  m_txNext.SetMod (length);
  m_PollSn.SetMod (length);
  m_rxNext.SetMod (length);
  m_rxNextStatusTrigger.SetMod (length);
  m_rxHighestStatus.SetMod (length);
  m_rxNextHighest.SetMod (length);
}
uint8_t
NrRlcAm::GetSnBitLength (void) const
{
  return m_snBitLen;
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
