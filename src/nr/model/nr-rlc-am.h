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

#ifndef NR_RLC_AM_H
#define NR_RLC_AM_H
#include "nr-rlc.h"
#include "nr-rlc-sequence-number.h"
#include "nr-rlc-header.h"
#include "ns3/timer.h"
#include "ns3/packet.h"
#include <map>
namespace ns3 {

class NrRlcAm : public NrRlc
{
public:
  NrRlcAm ();
  virtual ~NrRlcAm ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * RLC SAP
   */
  virtual void DoTransmitPdcpPdu (Ptr<Packet> p);

  /**
   * MAC SAP
   */
  virtual void DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId);
  virtual void DoNotifyHarqDeliveryFailure ();
  virtual void DoReceivePdu (Ptr<Packet> p);

  void SetSnBitLength (uint8_t length);
  uint8_t GetSnBitLength () const;

private:
  uint32_t m_maxTxBufferSize;
  uint8_t m_snBitLen;

  SequenceNumber m_txNextAck;
  SequenceNumber m_txNext;
  SequenceNumber m_PollSn;
  uint32_t m_pduWithoutPoll;
  uint32_t m_byteWithoutPoll;
  SequenceNumber m_rxNext;
  SequenceNumber m_rxNextStatusTrigger;
  SequenceNumber m_rxHighestStatus;
  SequenceNumber m_rxNextHighest;

  // This constant is used by both the transmitting side and the receiving side of each AM RLC entity. AM_Window_Size = 2048 when a 12 bit SN is used, AM_Window_Size = 131072 when an 18 bit SN is used.
  uint32_t m_windowSize;
  //This parameter is used by the transmitting side of each AM RLC entity to limit the number of retransmissions corresponding to an RLC SDU, including its segments
  uint32_t m_maxRetxThreshold;
  //This parameter is used by the transmitting side of each AM RLC entity to trigger a poll for every pollPDU PDUs
  uint32_t m_pollPDU;
  //This parameter is used by the transmitting side of each AM RLC entity to trigger a poll for every pollByte bytes
  uint32_t m_pollByte;

  //This timer is used by the transmitting side of an AM RLC entity in order to retransmit a poll
  Timer m_tPollRetransmit;
  Time m_tPollRetransmitTime;
  // /This timer is used by the receiving side of an AM RLC entity in order to detect loss of RLC PDUs at lower layer.
  Timer m_tReassembly;
  Time m_tReassemblyTime;
  //This timer is used by the receiving side of an AM RLC entity in order to prohibit transmission of a STATUS PDU
  Timer m_tStatusProhibit;
  Time m_tStatusProhibitTime;

  NrRlcAmHeader::PduType_t m_PduTypeSN;
  NrRlcAmHeader::PduType_t m_PduTypeSNSO;
  NrRlcAmHeader::PduType_t m_PduTypeStatus;
};

} // namespace ns3

#endif
