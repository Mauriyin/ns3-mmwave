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

#ifndef NR_RLC_SEQUENCE_NUMBER_H
#define NR_RLC_SEQUENCE_NUMBER_H

#include <limits>
#include <iostream>
#include <stdint.h>
#include "ns3/assert.h"

namespace ns3 {
class SequenceNumber
{
public:
  SequenceNumber ()
  {
    m_modBitLen = 18;
    m_modulusMask = (1 << m_modBitLen) - 1;
    m_value = 0;
  }

  explicit SequenceNumber (uint32_t value)
  {
    m_modBitLen = 18;
    m_modulusMask = (1 << m_modBitLen) - 1;
    m_value = value & m_modulusMask;
  }

  SequenceNumber (uint32_t value, uint8_t modBitLen)
  {
    m_modBitLen = modBitLen;
    m_modulusMask = (1 << modBitLen) - 1;
    m_value = value & m_modulusMask;
  }

  SequenceNumber (SequenceNumber const &oth)
      : m_value (oth.m_value), m_modBitLen (oth.m_modBitLen), m_modulusMask (oth.m_modulusMask)
  {
  }

  SequenceNumber &
  operator= (uint32_t value)
  {
    m_value = value & m_modulusMask;
    return *this;
  }

  void
  SetMod (uint8_t modBitLen)
  {
    NS_ASSERT_MSG (modBitLen == 6 || modBitLen == 12 || modBitLen == 18,
                   "Sequence Number must be 6 or 12 or 18");
    m_modBitLen = modBitLen;
    m_modulusMask = (1 << modBitLen) - 1;
    m_value = m_value & m_modulusMask;
  }

  uint8_t
  GetMod ()
  {
    return m_modBitLen;
  }

  uint32_t
  GetValue () const
  {
    return m_value;
  }

  SequenceNumber
  operator++ (int)
  {
    SequenceNumber retval (*this);
    m_value = (m_value + 1) & m_modulusMask;
    return retval;
  }

  SequenceNumber
  operator++ ()
  {
    m_value = (m_value + 1) & m_modulusMask;
    return *this;
  }

  SequenceNumber
  operator-- (int)
  {
    SequenceNumber retval (*this);
    m_value = (m_value + m_modulusMask) & m_modulusMask;
    return retval;
  }

  SequenceNumber
  operator-- ()
  {
    m_value = (m_value + m_modulusMask) & m_modulusMask;
    return *this;
  }

  SequenceNumber
  operator+ (uint32_t delta) const
  {
    return SequenceNumber (m_value + delta, m_modBitLen);
  }

  SequenceNumber
  operator- (uint32_t delta) const
  {
    return SequenceNumber ((m_value - (delta & m_modulusMask) + m_modulusMask + 1) & m_modulusMask,
                           m_modBitLen);
  }

  uint32_t
  operator- (const SequenceNumber &other) const
  {
    NS_ASSERT_MSG (m_modBitLen == other.m_modBitLen,
                   "Sequence Number of different modulus bit can not calculate");
    return (m_value - other.m_value + m_modulusMask + 1) & m_modulusMask;
  }

  bool
  operator> (const SequenceNumber &other) const
  {
    return m_value > other.m_value;
  }

  bool
  operator== (const SequenceNumber &other) const
  {
    return (m_value == other.m_value);
  }

  bool
  operator!= (const SequenceNumber &other) const
  {
    return (m_value != other.m_value);
  }

  bool
  operator<= (const SequenceNumber &other) const
  {
    return (!this->operator> (other));
  }

  bool
  operator>= (const SequenceNumber &other) const
  {
    return (this->operator> (other) || this->operator== (other));
  }

  bool
  operator< (const SequenceNumber &other) const
  {
    return !this->operator> (other) && m_value != other.m_value;
  }

  friend std::ostream &operator<< (std::ostream &os, const SequenceNumber &val);

private:
  uint32_t m_value;
  uint8_t m_modBitLen;
  uint32_t m_modulusMask;
};

/////////////////////////////////////////////////////////////
class SequenceNumber12
{
public:
  SequenceNumber12 ()
  {
    m_modulusMask = (1 << 12) - 1;
    m_value = m_modulusMask + 1;
  }

  explicit SequenceNumber12 (uint16_t value)
  {
    m_modulusMask = (1 << 12) - 1;
    if (value < m_modulusMask + 1)
      m_value = value + m_modulusMask + 1;
    else
      m_value = value;
  }

  SequenceNumber12 (SequenceNumber12 const &oth)
      : m_value (oth.m_value), m_modulusMask (oth.m_modulusMask)
  {
  }

  SequenceNumber12 &
  operator= (uint16_t value)
  {
    if (value < m_modulusMask + 1)
      m_value = value + m_modulusMask + 1;
    else
      m_value = value;
    return *this;
  }

  uint16_t
  GetValue () const
  {
    return m_value & m_modulusMask;
  }

  SequenceNumber12
  operator++ (int)
  {
    SequenceNumber12 retval (*this);
    ++m_value;
    return retval;
  }

  SequenceNumber12
  operator++ ()
  {
    ++m_value;
    return *this;
  }

  SequenceNumber12
  operator-- (int)
  {
    SequenceNumber12 retval (*this);
    --m_value;
    return retval;
  }

  SequenceNumber12
  operator-- ()
  {
    --m_value;
    return *this;
  }

  SequenceNumber12
  operator+ (uint16_t delta) const
  {
    return SequenceNumber12 (m_value + delta);
  }

  SequenceNumber12
  operator- (uint16_t delta) const
  {
    return SequenceNumber12 (m_value - delta);
  }

  uint16_t
  operator- (const SequenceNumber12 &other) const
  {
    NS_ASSERT_MSG (m_value >= other.m_value, "Sequence number diff < 0");
    return (m_value - other.m_value) & m_modulusMask;
  }

  bool
  operator> (const SequenceNumber12 &other) const
  {
    return (m_value > other.m_value);
  }

  bool
  operator== (const SequenceNumber12 &other) const
  {
    return (m_value == other.m_value);
  }

  bool
  operator!= (const SequenceNumber12 &other) const
  {
    return (m_value != other.m_value);
  }

  bool
  operator<= (const SequenceNumber12 &other) const
  {
    return (m_value <= other.m_value);
  }

  bool
  operator>= (const SequenceNumber12 &other) const
  {
    return (m_value >= other.m_value);
  }

  bool
  operator< (const SequenceNumber12 &other) const
  {
    return (m_value < other.m_value);
  }

  friend std::ostream &operator<< (std::ostream &os, const SequenceNumber12 &val);

private:
  uint32_t m_value;
  uint32_t m_modulusMask;
};
} // namespace ns3

#endif // NR_RLC_SEQUENCE_NUMBER_H
