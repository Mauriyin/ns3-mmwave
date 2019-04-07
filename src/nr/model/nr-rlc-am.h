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

private:
};

} // namespace ns3

#endif
