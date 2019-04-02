#ifndef NR_RLC_SEQUENCE_NUMBER_H
#define NR_RLC_SEQUENCE_NUMBER_H

#include <limits>
#include <iostream>
#include <stdint.h>
#include "ns3/assert.h"

namespace ns3
{
class SequenceNumber
{
  public:
	SequenceNumber(uint8_t modBitLen)
	{
		NS_ASSERT_MSG(modBitLen == 6 || modBitLen == 12 || modBitLen == 18, "Sequence Number must be 6 or 12 or 18");
		m_modBitLen = modBitLen;
		m_modulusMask = (1 << modBitLen) - 1;
		m_value = 0;
	}

	SequenceNumber(uint32_t value, uint8_t modBitLen)
	{
		NS_ASSERT_MSG(modBitLen == 6 || modBitLen == 12 || modBitLen == 18, "Sequence Number must be 6 or 12 or 18");
		m_modBitLen = modBitLen;
		m_modulusMask = (1 << modBitLen) - 1;
		m_value = value & m_modulusMask;
	}

	SequenceNumber(SequenceNumber const &oth)
		: m_value(oth.m_value),
		  m_modBitLen(oth.m_modBitLen),
		  m_modulusMask(oth.m_modulusMask)
	{
	}

	SequenceNumber &operator=(uint32_t value)
	{
		m_value = value & m_modulusMask;
		return *this;
	}

	void SetMod(uint8_t modBitLen)
	{
		NS_ASSERT_MSG(modBitLen == 6 || modBitLen == 12 || modBitLen == 18, "Sequence Number must be 6 or 12 or 18");
		m_modBitLen = modBitLen;
		m_modulusMask = (1 << modBitLen) - 1;
		m_value = m_value & m_modulusMask;
	}

	uint8_t GetMod()
	{
		return m_modBitLen;
	}

	uint32_t GetValue() const
	{
		return m_value;
	}

	SequenceNumber operator++(int)
	{
		SequenceNumber retval(*this);
		m_value = (m_value + 1) & m_modulusMask;
		return retval;
	}

	SequenceNumber operator+(uint32_t delta) const
	{
		return SequenceNumber(m_value + delta, m_modBitLen);
	}

	SequenceNumber operator-(uint32_t delta) const
	{
		return SequenceNumber((m_value - (delta & m_modulusMask) + m_modulusMask + 1) & m_modulusMask, m_modBitLen);
	}

	uint32_t operator-(const SequenceNumber &other) const
	{
		NS_ASSERT_MSG(m_modBitLen == other.m_modBitLen, "Sequence Number of different modulus bit can not calculate");
		return (m_value - other.m_value + m_modulusMask + 1) & m_modulusMask;
	}

	bool operator>(const SequenceNumber &other) const
	{
		return m_value > other.m_value;
	}

	bool operator==(const SequenceNumber &other) const
	{
		return (m_value == other.m_value);
	}

	bool operator!=(const SequenceNumber &other) const
	{
		return (m_value != other.m_value);
	}

	bool operator<=(const SequenceNumber &other) const
	{
		return (!this->operator>(other));
	}

	bool operator>=(const SequenceNumber &other) const
	{
		return (this->operator>(other) || this->operator==(other));
	}

	bool operator<(const SequenceNumber &other) const
	{
		return !this->operator>(other) && m_value != other.m_value;
	}

	friend std::ostream &operator<<(std::ostream &os, const SequenceNumber &val);

  private:
	uint32_t m_value;
	uint8_t m_modBitLen;
	uint32_t m_modulusMask;
};

class SequenceNumber6
{
  public:
	SequenceNumber6()
	{
		m_modulusMask = (1 << 6) - 1;
		m_value = 0;
	}

	explicit SequenceNumber6(uint8_t value)
	{
		m_modulusMask = (1 << 6) - 1;
		m_value = value & m_modulusMask;
	}

	SequenceNumber6(SequenceNumber6 const &oth)
		: m_value(oth.m_value),
		  m_modulusMask(oth.m_modulusMask)
	{
	}

	SequenceNumber6 &operator=(uint8_t value)
	{
		m_value = value & m_modulusMask;
		return *this;
	}

	uint8_t GetMod()
	{
		return 6;
	}

	uint8_t GetValue() const
	{
		return m_value;
	}

	SequenceNumber6 operator++(int)
	{
		SequenceNumber6 retval(*this);
		m_value = (m_value + 1) & m_modulusMask;
		return retval;
	}

	SequenceNumber6 operator+(uint8_t delta) const
	{
		return SequenceNumber6(m_value + delta);
	}

	SequenceNumber6 operator-(uint8_t delta) const
	{
		return SequenceNumber6((m_value - (delta & m_modulusMask) + m_modulusMask + 1) & m_modulusMask);
	}

	uint8_t operator-(const SequenceNumber6 &other) const
	{
		return (m_value - other.m_value + m_modulusMask + 1) & m_modulusMask;
	}

	bool operator>(const SequenceNumber6 &other) const
	{
		return m_value > other.m_value;
	}

	bool operator==(const SequenceNumber6 &other) const
	{
		return (m_value == other.m_value);
	}

	bool operator!=(const SequenceNumber6 &other) const
	{
		return (m_value != other.m_value);
	}

	bool operator<=(const SequenceNumber6 &other) const
	{
		return (!this->operator>(other));
	}

	bool operator>=(const SequenceNumber6 &other) const
	{
		return (this->operator>(other) || this->operator==(other));
	}

	bool operator<(const SequenceNumber6 &other) const
	{
		return !this->operator>(other) && m_value != other.m_value;
	}

	friend std::ostream &operator<<(std::ostream &os, const SequenceNumber6 &val);

  private:
	uint8_t m_value;
	uint8_t m_modulusMask;
};

class SequenceNumber12
{
  public:
	SequenceNumber12()
	{
		m_modulusMask = (1 << 12) - 1;
		m_value = 0;
	}

	explicit SequenceNumber12(uint16_t value)
	{
		m_modulusMask = (1 << 12) - 1;
		m_value = value & m_modulusMask;
	}

	SequenceNumber12(SequenceNumber12 const &oth)
		: m_value(oth.m_value),
		  m_modulusMask(oth.m_modulusMask)
	{
	}

	SequenceNumber12 &operator=(uint16_t value)
	{
		m_value = value & m_modulusMask;
		return *this;
	}

	uint16_t GetMod()
	{
		return 12;
	}

	uint16_t GetValue() const
	{
		return m_value;
	}

	SequenceNumber12 operator++(int)
	{
		SequenceNumber12 retval(*this);
		m_value = (m_value + 1) & m_modulusMask;
		return retval;
	}

	SequenceNumber12 operator+(uint16_t delta) const
	{
		return SequenceNumber12(m_value + delta);
	}

	SequenceNumber12 operator-(uint16_t delta) const
	{
		return SequenceNumber12((m_value - (delta & m_modulusMask) + m_modulusMask + 1) & m_modulusMask);
	}

	uint16_t operator-(const SequenceNumber12 &other) const
	{
		return (m_value - other.m_value + m_modulusMask + 1) & m_modulusMask;
	}

	bool operator>(const SequenceNumber12 &other) const
	{
		return m_value > other.m_value;
	}

	bool operator==(const SequenceNumber12 &other) const
	{
		return (m_value == other.m_value);
	}

	bool operator!=(const SequenceNumber12 &other) const
	{
		return (m_value != other.m_value);
	}

	bool operator<=(const SequenceNumber12 &other) const
	{
		return (!this->operator>(other));
	}

	bool operator>=(const SequenceNumber12 &other) const
	{
		return (this->operator>(other) || this->operator==(other));
	}

	bool operator<(const SequenceNumber12 &other) const
	{
		return !this->operator>(other) && m_value != other.m_value;
	}

	friend std::ostream &operator<<(std::ostream &os, const SequenceNumber12 &val);

  private:
	uint16_t m_value;
	uint16_t m_modulusMask;
};

class SequenceNumber18
{
  public:
	SequenceNumber18()
	{
		m_modulusMask = (1 << 18) - 1;
		m_value = 0;
	}

	explicit SequenceNumber18(uint32_t value)
	{
		m_modulusMask = (1 << 18) - 1;
		m_value = value & m_modulusMask;
	}

	SequenceNumber18(SequenceNumber18 const &oth)
		: m_value(oth.m_value),
		  m_modulusMask(oth.m_modulusMask)
	{
	}

	SequenceNumber18 &operator=(uint32_t value)
	{
		m_value = value & m_modulusMask;
		return *this;
	}

	uint32_t GetMod()
	{
		return 18;
	}

	uint32_t GetValue() const
	{
		return m_value;
	}

	SequenceNumber18 operator++(int)
	{
		SequenceNumber18 retval(*this);
		m_value = (m_value + 1) & m_modulusMask;
		return retval;
	}

	SequenceNumber18 operator+(uint32_t delta) const
	{
		return SequenceNumber18(m_value + delta);
	}

	SequenceNumber18 operator-(uint32_t delta) const
	{
		return SequenceNumber18((m_value - (delta & m_modulusMask) + m_modulusMask + 1) & m_modulusMask);
	}

	uint32_t operator-(const SequenceNumber18 &other) const
	{
		return (m_value - other.m_value + m_modulusMask + 1) & m_modulusMask;
	}

	bool operator>(const SequenceNumber18 &other) const
	{
		return m_value > other.m_value;
	}

	bool operator==(const SequenceNumber18 &other) const
	{
		return (m_value == other.m_value);
	}

	bool operator!=(const SequenceNumber18 &other) const
	{
		return (m_value != other.m_value);
	}

	bool operator<=(const SequenceNumber18 &other) const
	{
		return (!this->operator>(other));
	}

	bool operator>=(const SequenceNumber18 &other) const
	{
		return (this->operator>(other) || this->operator==(other));
	}

	bool operator<(const SequenceNumber18 &other) const
	{
		return !this->operator>(other) && m_value != other.m_value;
	}

	friend std::ostream &operator<<(std::ostream &os, const SequenceNumber18 &val);

  private:
	uint32_t m_value;
	uint32_t m_modulusMask;
};

} // namespace ns3

#endif // NR_RLC_SEQUENCE_NUMBER_H
