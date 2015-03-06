#ifndef UNITS_METRIC_H
#define UNITS_METRIC_H

#include "unit.h"

namespace units {
namespace metric {

namespace detail {
    using pi = std::ratio<31415926535897932, 10000000000000000>;
} /* namespace detail */

#define UNIT_TYPE(x) unit_type<decltype(x)>

#define SI_SMALLER_PREFIXES(base)                                              \
    auto atto##base  = std::atto{} * base;                                     \
    auto femto##base = std::femto{} * base;                                    \
    auto pico##base  = std::pico{} * base;                                     \
    auto nano##base  = std::nano{} * base;                                     \
    auto micro##base = std::micro{} * base;                                    \
    auto milli##base = std::milli{} * base;                                    \
    auto centi##base = std::centi{} * base;                                    \
    auto deci##base  = std::deci{} * base;

#define SI_LARGER_PREFIXES(base)                                               \
    auto deca##base  = std::deca{} * base;                                     \
    auto hecto##base = std::hecto{} * base;                                    \
    auto kilo##base  = std::kilo{} * base;                                     \
    auto mega##base  = std::mega{} * base;                                     \
    auto giga##base  = std::giga{} * base;                                     \
    auto tera##base  = std::tera{} * base;                                     \
    auto peta##base  = std::peta{} * base;                                     \
    auto exa##base   = std::exa{} * base;

#define SI_ALL_PREFIXES(base)                                                  \
    SI_SMALLER_PREFIXES(base)                                                  \
    SI_LARGER_PREFIXES(base)                                                   \

////////////////////////////////// BASE UNITS //////////////////////////////////

// Length

DEFINE_DIMENSION(Distance, meter);
SI_ALL_PREFIXES(meter);

// Time

DEFINE_DIMENSION(Time, second);
SI_SMALLER_PREFIXES(second);
auto minute = std::ratio<60, 1>{} * second;
auto hour   = std::ratio<60, 1>{} * minute;
auto day    = std::ratio<24, 1>{} * hour;
auto week   = std::ratio<7, 1>{} * day;
auto year   = std::ratio<365, 1>{} * day;

// Mass

DEFINE_DIMENSION(Mass, kilogram);
auto gram = std::milli{} * kilogram;

auto femtogram = std::femto{} * gram;
auto picogram  = std::pico{} * gram;
auto nanogram  = std::nano{} * gram;
auto microgram = std::micro{} * gram;
auto milligram = std::milli{} * gram;
auto centigram = std::centi{} * gram;
auto decigram  = std::deci{} * gram;

auto decagram  = std::deca{} * gram;
auto hectogram = std::hecto{} * gram;
auto megagram  = std::mega{} * gram;
auto gigagram  = std::giga{} * gram;
auto teragram  = std::tera{} * gram;
auto petagram  = std::peta{} * gram;
auto exagram   = std::exa{} * gram;
auto tonne     = megagram;

// Temperature

DEFINE_DIMENSION(Temperature, kelvin);

// Current

DEFINE_DIMENSION(Current, ampere);

// Luminous intensity

DEFINE_DIMENSION(LuminousIntensity, candela);

// Amount of substance

DEFINE_DIMENSION(Substance, mole);

// Angle

DEFINE_DIMENSION(Angle, radian);
auto degree =  detail::pi{} * (std::ratio<1, 180>{} * radian);

//////////////////////////////// DERIVED UNITS /////////////////////////////////

// Area

auto square_meter = meter * meter;
using Area = UNIT_TYPE(square_meter);

// Volume

auto cubic_meter = meter * meter * meter;
using Volume = UNIT_TYPE(cubic_meter);
auto liter = std::milli{} * cubic_meter;

auto femtoliter = std::femto{} * liter;
auto picoliter  = std::pico{} * liter;
auto nanoliter  = std::nano{} * liter;
auto microliter = std::micro{} * liter;
auto milliliter = std::milli{} * liter;
auto centiliter = std::centi{} * liter;
auto deciliter  = std::deci{} * liter;

SI_LARGER_PREFIXES(liter);

// Velocity

auto mps = meter / second;
using Velocity = UNIT_TYPE(mps);

// Acceleration

auto mps2 = mps / second;
using Acceleration = UNIT_TYPE(mps2);

// Frequency

auto hertz = second.exp<-1>();
using Frequency = UNIT_TYPE(hertz);
SI_ALL_PREFIXES(hertz);

// Force

auto newton = kilogram * meter / (second * second);
using Force = UNIT_TYPE(newton);
SI_ALL_PREFIXES(newton);

// Pressure

auto pascal = newton / (meter * meter);
using Pressure = UNIT_TYPE(pascal);

// Energy

auto joule = newton * meter;
using Energy = UNIT_TYPE(joule);

// Power

auto watt = joule / second;
using Power = UNIT_TYPE(watt);

// Electric charge

auto coloumb = ampere * second;
using ElectricCharge = UNIT_TYPE(coloumb);

// Voltage

auto volt = watt / ampere;
using Voltage = UNIT_TYPE(volt);

// Electric capacitance

auto farad = coloumb / volt;
using ElectricCapacitance = UNIT_TYPE(farad);

// Electric resistance

auto ohm = volt / ampere;
using ElectricResistance = UNIT_TYPE(ohm);

// Electrical conductance

auto siemens = ampere / volt;
using ElectricalConductance = UNIT_TYPE(siemens);

// Magnetic flux

auto weber = volt * second;
using MagneticFlux = UNIT_TYPE(weber);

// Magnetic field strength

auto tesla = weber / meter.exp<2>();
using MagneticField = UNIT_TYPE(tesla);

// Inductance

auto henry = weber / ampere;
using Inductance = UNIT_TYPE(henry);

// Radioactivity

auto becquerel = hertz;
using Radioactivity = UNIT_TYPE(becquerel);

// Absorbed dose (of ionizing radiation)

auto gray = joule / kilogram;
using AbsorbedDose = UNIT_TYPE(gray);

// Equivalent dose (of ionizing radiation) J/kg    m2⋅s−2

auto sievert = joule / kilogram;
using EquivalentDos = UNIT_TYPE(sievert);

// Catalytic activity

auto katal = mole / second;
using CatalyticActivity = UNIT_TYPE(katal);

} /* namespace metric */
} /* namespace units */

#endif
