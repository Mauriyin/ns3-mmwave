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

#ifndef NR_RLC_UM_H
#define NR_RLC_UM_H
#include "nr-rlc.h"
#include "nr-rlc-sequence-number.h"
#include "nr-rlc-header.h"
#include "ns3/timer.h"
#include "ns3/packet.h"
#include <map>
namespace ns3 {
class NrRlcUmRxBuffer
{
  std::map<uint16_t, Ptr<Packet>> m_buffer;
  uint16_t m_lengthWithoutLastSeq;
  uint16_t m_currentLength;

public:
  NrRlcUmRxBuffer ();
  ~NrRlcUmRxBuffer ();
  bool AddPacket (Ptr<Packet> p, uint16_t so);
  bool AddLastPacket (Ptr<Packet> p, uint16_t so);
  void Clear ();
  bool IsAll ();
  bool IsContinuous();
  Ptr<Packet> GetPacket ();
};

class NrRlcUm : public NrRlc
{
public:
  NrRlcUm ();
  virtual ~NrRlcUm ();
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
  uint8_t GetSnBitLength (void) const;
  void SetUmWindowSize (uint16_t size);
  void SetTxBufferSize (uint32_t size);

private:
  void SetupTimer (void);
  void ExpireTimer (void);
  void CheckAndOpenTimer (void);
  bool InWindow (SequenceNumber sn);
  bool InDiscardWindow (SequenceNumber sn);
  void DiscardNFrom (SequenceNumber iter, uint16_t n);
  SequenceNumber FindNext (SequenceNumber sn);
  uint16_t m_windowSize;
  uint32_t m_maxTxBufferSize;
  uint32_t m_txBufferSize;
  uint8_t m_snBitLen;

  std::list<Ptr<Packet>> m_txBuffer;
  // std::map<uint16_t, NrRlcUmRxBuffer> m_rxBuffer;
  std::vector<NrRlcUmRxBuffer> m_rxBuffer;

  SequenceNumber m_txNext;
  SequenceNumber m_rxNextHighest;
  SequenceNumber m_rxNextReassembly;
  SequenceNumber m_rxTimerTrigger;

  Timer m_tReassembly;
  Time m_reassemblyDelay;

  NrRlcUmHeader::PduType_t m_nextPduType;
  NrRlcUmHeader::PduType_t m_PduTypeSN;
  NrRlcUmHeader::PduType_t m_PduTypeSNSO;
};

} // namespace ns3

#endif
