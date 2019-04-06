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

#ifndef NR_RLC_PDU_TAG_H
#define NR_RLC_PDU_TAG_H
#include "ns3/tag.h"
namespace ns3 {
class NrRlcPduTag : public Tag
{
public:
  NrRlcPduTag ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer iter) const;
  virtual void Deserialize (TagBuffer iter);
  virtual void Print (std::ostream &os) const;

  virtual void SetPduType (uint8_t type);
  virtual uint8_t GetPduType () const;

private:
  uint8_t m_type;
};
} // namespace ns3

#endif