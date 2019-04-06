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

#include "nr-rlc-pdu-tag.h"
namespace ns3 {
NrRlcPduTag::NrRlcPduTag ()
{
}
TypeId
NrRlcPduTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcPduTag")
                          .SetParent<Tag> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcPduTag> ();
  return tid;
}
TypeId
NrRlcPduTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcPduTag::GetSerializedSize (void) const
{
  return 1;
}
void
NrRlcPduTag::Serialize (TagBuffer iter) const
{
  iter.WriteU8 (m_type);
}
void
NrRlcPduTag::Deserialize (TagBuffer iter)
{
  m_type = iter.ReadU8 ();
}
void
NrRlcPduTag::Print (std::ostream &os) const
{
  os << m_type;
}
void
NrRlcPduTag::SetPduType (uint8_t type)
{
  m_type = type;
}
uint8_t
NrRlcPduTag::GetPduType () const
{
  return m_type;
}
} // namespace ns3
