#include "nr-rlc-sequence-number.h"

namespace ns3
{
std::ostream &operator<<(std::ostream &os, const SequenceNumber &val)
{
    os << val.m_value;
    return os;
}

std::ostream &operator<<(std::ostream &os, const SequenceNumber12 &val)
{
    os << val.m_value;
    return os;
}
} // namespace ns3
