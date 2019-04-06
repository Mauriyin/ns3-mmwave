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

#ifndef NR_PDCP_H
#define NR_PDCP_H
#include "ns3/simple-ref-count.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
namespace ns3 {
class NrPdcp : public Object
{
public:
  NrRlc ();
  virtual ~NrRlc ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

private:
  uint32_t m_windowSize;
  uint32_t m_maxTxBufferSize;
  uint32_t m_mtuSize;

  SequenceNumber m_txNext;
  SequenceNumber m_rxNext;
  SequenceNumber m_rxDeliv;
  SequenceNumber m_rxReOrd;

  Timer m_discardTimer;
  Timer m_tReordering;
};
} // namespace ns3

#endif