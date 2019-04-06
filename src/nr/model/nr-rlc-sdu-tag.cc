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

#include "nr-rlc-sdu-tag.h"
namespace ns3 {
NrRlcSduTag::NrRlcSduTag ()
{
}
TypeId
NrRlcSduTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcSduTag")
                          .SetParent<Tag> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcSduTag> ();
  return tid;
}
uint16_t
NrRlcSduTag::GetSudId (void)
{
  static uint16_t sduId = 0;
  return sduId++;
}

TypeId
NrRlcSduTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcSduTag::GetSerializedSize (void) const
{
  return 2;
}
void
NrRlcSduTag::Serialize (TagBuffer i) const
{
  i.WriteU16 (m_sduId);
}
void
NrRlcSduTag::Deserialize (TagBuffer i)
{
  m_sduId = i.ReadU16 ();
}
void
NrRlcSduTag::Print (std::ostream &os) const
{
  os << m_sduId;
}
void
NrRlcSduTag::GenSduId (void)
{
  m_sduId = GetSudId ();
}
} // namespace ns3
