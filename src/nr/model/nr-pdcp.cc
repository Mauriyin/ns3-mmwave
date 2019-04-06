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

#include "nr-pdcp.h"

namespace ns3 {
NrPdcp::NrPdcp ()
{
  NS_LOG_FUNCTION (this);
}
NrPdcp::~NrPdcp ()
{
  NS_LOG_FUNCTION (this);
}
TypeId
NrPdcp::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::NrPdcp")
          .SetParent<Object> ()
          .SetGroupName ("Nr")
          .AddConstructor<NrPdcp> ()
          .AddAttribute ("MaxTxBufferSize", "Maximum Size of the Transmission Buffer (in Bytes)",
                         UintegerValue (20480), MakeUintegerAccessor (&NrPdcp::m_maxTxBufferSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("WindowSize", "Pdcp Window Size", UintegerValue (20480),
                         MakeUintegerAccessor (&NrPdcp::m_windowSize),
                         MakeUintegerChecker<uint32_t> ())
          .AddAttribute ("Mtu", "The Maximum Supported Size of a PDCP SDU", UintegerValue (9000),
                         MakeUintegerAccessor (&NrPdcp::m_mtuSize),
                         MakeUintegerChecker<uint32_t> ());
  return tid;
}
void
NrPdcp::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_txNext = 0;
  m_rxNext = 0;
  m_rxDeliv = 0;
  m_rxReOrd = 0;
}
} // namespace ns3
