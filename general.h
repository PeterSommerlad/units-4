#ifndef UNITS_GENERAL_H
#define UNITS_GENERAL_H

#include "number.h"

namespace units {

DEFINE_DIMENSION(time, second);
const auto minute = std::ratio<60, 1>{} * second;
const auto hour   = std::ratio<60, 1>{} * minute;
const auto day    = std::ratio<24, 1>{} * hour;
const auto week   = std::ratio<7, 1>{} * day;
const auto fortnight = std::ratio<2, 1>{} * week;
const auto year   = std::ratio<365, 1>{} * day;

const auto attosecond  = std::atto{} * second;
const auto femtosecond = std::femto{} * second;
const auto picosecond  = std::pico{} * second;
const auto nanosecond  = std::nano{} * second;
const auto microsecond = std::micro{} * second;
const auto millisecond = std::milli{} * second;
const auto centisecond = std::centi{} * second;
const auto decisecond  = std::deci{} * second;

} /* namespace units */

#endif
