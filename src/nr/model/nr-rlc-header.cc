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
#include "nr-rlc-header.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("NrRlcHeader");

NS_OBJECT_ENSURE_REGISTERED (NrRlcUmHeader);

NrRlcUmHeader::NrRlcUmHeader () : m_SN (0), m_SO (0), m_SI (SI_UNKNOW), m_type (PDU_UNKNOW)
{
}
NrRlcUmHeader::~NrRlcUmHeader (){

};
TypeId
NrRlcUmHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcUmHeader")
                          .SetParent<Header> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcUmHeader> ();
  return tid;
}
TypeId
NrRlcUmHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcUmHeader::GetSerializedSize (void) const
{
  uint8_t headerLen;
  switch (m_type)
    {
    case PDU_COMPLETE:
    case PDU_SN6:
      headerLen = 1;
      break;
    case PDU_SN12:
      headerLen = 2;
      break;
    case PDU_SN6SO:
      headerLen = 3;
      break;
    case PDU_SN12SO:
      headerLen = 4;
      break;
    default:
      NS_ABORT_MSG ("Header has not been init");
    }
  return headerLen;
}
void
NrRlcUmHeader::Serialize (Buffer::Iterator iter) const
{
  switch (m_type)
    {
    case PDU_COMPLETE:
      iter.WriteU8 ((uint8_t) m_SI);
      break;
    case PDU_SN6:
      iter.WriteU8 ((uint8_t) m_SI | (uint8_t) m_SN);
      break;
    case PDU_SN12:
      iter.WriteU8 ((uint8_t) m_SI | (uint8_t) (m_SN >> 8));
      iter.WriteU8 ((uint8_t) m_SN);
      break;
    case PDU_SN6SO:
      iter.WriteU8 ((uint8_t) m_SI | (uint8_t) m_SN);
      iter.WriteU16 (m_SO);
      break;
    case PDU_SN12SO:
      iter.WriteU8 ((uint8_t) m_SI | (uint8_t) (m_SN >> 8));
      iter.WriteU8 ((uint8_t) m_SN);
      iter.WriteU16 (m_SO);
      break;

    default:
      break;
    }
}
uint32_t
NrRlcUmHeader::Deserialize (Buffer::Iterator iter)
{
  uint8_t tmp;
  switch (m_type)
    {
    case PDU_COMPLETE:
      m_SI = (SIType_t) (iter.ReadU8 () & SI_MASK);
      return 1;
    case PDU_SN6:
      tmp = iter.ReadU8 ();
      m_SI = (SIType_t) (tmp & SI_MASK);
      m_SN = (uint16_t) (tmp & SN6_MASK);
      return 1;
    case PDU_SN12:
      tmp = iter.ReadU8 ();
      m_SI = (SIType_t) (tmp & SI_MASK);
      m_SN = (uint16_t) (tmp & SN6_MASK) << 8 | iter.ReadU8 ();
      return 2;
    case PDU_SN6SO:
      tmp = iter.ReadU8 ();
      m_SI = (SIType_t) (tmp & SI_MASK);
      m_SN = (uint16_t) (tmp & SN6_MASK);
      m_SO = iter.ReadU16 ();
      return 3;
    case PDU_SN12SO:
      tmp = iter.ReadU8 ();
      m_SI = (SIType_t) (tmp & SI_MASK);
      m_SN = (uint16_t) (tmp & SN6_MASK) << 8 | iter.ReadU8 ();
      m_SO = iter.ReadU16 ();
      return 4;

    default:
      return 0;
    }
}
void
NrRlcUmHeader::Print (std::ostream &os) const
{
  os << "Type: " << m_type << " ";
  os << "SI: " << m_SI << " ";
  switch (m_type)
    {
    case PDU_SN6SO:
    case PDU_SN12SO:
      os << "SO: " << m_SO << " ";
    case PDU_SN6:
    case PDU_SN12:
      os << "SN: " << m_SN << " ";
    default:
      break;
    }
}

void
NrRlcUmHeader::SetHeaderType (PduType_t type)
{
  NS_LOG_FUNCTION (this << type);
  m_type = type;
}
NrRlcUmHeader::PduType_t
NrRlcUmHeader::GetHeaderType () const
{
  return m_type;
}
void
NrRlcUmHeader::SetSequenceNumber (SequenceNumber sn)
{
  NS_LOG_FUNCTION (this << sn);
  m_SN = sn.GetValue ();
}
SequenceNumber
NrRlcUmHeader::GetSequenceNumber () const
{
  switch (m_type)
    {
    case PDU_SN6:
    case PDU_SN6SO:
      return SequenceNumber (m_SN, 6);
    case PDU_SN12:
    case PDU_SN12SO:
      return SequenceNumber (m_SN, 12);

    default:
      NS_ABORT_MSG ("Try to get sequence number failed");
      return SequenceNumber ();
    }
}
void
NrRlcUmHeader::SetSegmentationInfo (SIType_t si)
{
  NS_LOG_FUNCTION (this << si);
  m_SI = si;
}
NrRlcUmHeader::SIType_t
NrRlcUmHeader::GetSegmentationInfo () const
{
  return m_SI;
}
void
NrRlcUmHeader::SetSegmentOffset (uint32_t so)
{
  NS_LOG_FUNCTION (this << so);
  m_SO = so;
}
uint32_t
NrRlcUmHeader::GetSegmentOffset () const
{
  return m_SO;
}

///////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (NrRlcAmHeader);

NrRlcAmHeader::NrRlcAmHeader ()
    : m_ACK_SN (0), m_SO (0), m_DC (0), m_P (0), m_SI (SI_UNKNOW), m_type (PDU_UNKNOW)
{
}
NrRlcAmHeader::~NrRlcAmHeader (){

};
TypeId
NrRlcAmHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrRlcAmHeader")
                          .SetParent<Header> ()
                          .SetGroupName ("Nr")
                          .AddConstructor<NrRlcAmHeader> ();
  return tid;
}
TypeId
NrRlcAmHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
NrRlcAmHeader::GetSerializedSize (void) const
{
  switch (m_type)
    {
    case PDU_SN12:
      return 2;

    case PDU_SN18:
      return 3;

    case PDU_SN12SO:
      return 4;

    case PDU_SN18SO:
      return 5;

    case PDU_STATUS_SN12:
      return 2;

    case PDU_STATUS_SN18:
      return 2;

    default:
      return 0;
    }
  return 0;
}
void
NrRlcAmHeader::Serialize (Buffer::Iterator iter) const
{
}
uint32_t
NrRlcAmHeader::Deserialize (Buffer::Iterator iter)
{
  return 0;
}
void
NrRlcAmHeader::Print (std::ostream &os) const
{
}

void
NrRlcAmHeader::SetHeaderType (PduType_t type)
{
  NS_LOG_FUNCTION (this << type);
  m_type = type;
}
NrRlcAmHeader::PduType_t
NrRlcAmHeader::GetHeaderType () const
{
  return m_type;
}
void
NrRlcAmHeader::SetSequenceNumber (SequenceNumber sn)
{
  NS_LOG_FUNCTION (this << sn);
  m_ACK_SN = sn.GetValue ();
}
SequenceNumber
NrRlcAmHeader::GetSequenceNumber () const
{
  switch (m_type)
    {
    case PDU_SN12:
    case PDU_SN12SO:
      return SequenceNumber (m_ACK_SN, 12);
    case PDU_SN18:
    case PDU_SN18SO:
      return SequenceNumber (m_ACK_SN, 18);

    default:
      NS_ABORT_MSG ("Try to get sequence number failed");
      return SequenceNumber ();
    }
}
void
NrRlcAmHeader::SetSegmentationInfo (SIType_t si)
{
  NS_LOG_FUNCTION (this << si);
  m_SI = si;
}
NrRlcAmHeader::SIType_t
NrRlcAmHeader::GetSegmentationInfo () const
{
  return m_SI;
}
void
NrRlcAmHeader::SetSegmentOffset (uint32_t so)
{
  NS_LOG_FUNCTION (this << so);
  m_SO = so;
}
uint32_t
NrRlcAmHeader::GetSegmentOffset () const
{
  return m_SO;
}
} // namespace ns3
