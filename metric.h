#ifndef UNITS_METRIC_H
#define UNITS_METRIC_H

#include "number.h"
#include "general.h"

namespace units {
namespace metric {

namespace detail {
    using pi = std::ratio<31415926535897932, 10000000000000000>;
} /* namespace detail */

#define UNIT_TYPE(x) unit_type<decltype(x)>

#define SI_SMALLER_PREFIXES(base)                                              \
    const auto atto##base  = std::atto{} * base;                                     \
    const auto femto##base = std::femto{} * base;                                    \
    const auto pico##base  = std::pico{} * base;                                     \
    const auto nano##base  = std::nano{} * base;                                     \
    const auto micro##base = std::micro{} * base;                                    \
    const auto milli##base = std::milli{} * base;                                    \
    const auto centi##base = std::centi{} * base;                                    \
    const auto deci##base  = std::deci{} * base;

#define SI_LARGER_PREFIXES(base)                                               \
    const auto deca##base  = std::deca{} * base;                                     \
    const auto hecto##base = std::hecto{} * base;                                    \
    const auto kilo##base  = std::kilo{} * base;                                     \
    const auto mega##base  = std::mega{} * base;                                     \
    const auto giga##base  = std::giga{} * base;                                     \
    const auto tera##base  = std::tera{} * base;                                     \
    const auto peta##base  = std::peta{} * base;                                     \
    const auto exa##base   = std::exa{} * base;

#define SI_ALL_PREFIXES(base)                                                  \
    SI_SMALLER_PREFIXES(base)                                                  \
    SI_LARGER_PREFIXES(base)                                                   \

////////////////////////////////// BASE UNITS //////////////////////////////////

// Length

DEFINE_DIMENSION(Distance, meter);
SI_ALL_PREFIXES(meter);

// Mass

DEFINE_DIMENSION(Mass, kilogram);
const auto gram = std::milli{} * kilogram;

const auto femtogram = std::femto{} * gram;
const auto picogram  = std::pico{} * gram;
const auto nanogram  = std::nano{} * gram;
const auto microgram = std::micro{} * gram;
const auto milligram = std::milli{} * gram;
const auto centigram = std::centi{} * gram;
const auto decigram  = std::deci{} * gram;

const auto decagram  = std::deca{} * gram;
const auto hectogram = std::hecto{} * gram;
const auto megagram  = std::mega{} * gram;
const auto gigagram  = std::giga{} * gram;
const auto teragram  = std::tera{} * gram;
const auto petagram  = std::peta{} * gram;
const auto exagram   = std::exa{} * gram;
const auto tonne     = megagram;

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
const auto degree =  detail::pi{} * (std::ratio<1, 180>{} * radian);

//////////////////////////////// DERIVED UNITS /////////////////////////////////

// Area

const auto square_meter = meter * meter;
using Area = UNIT_TYPE(square_meter);

// Volume

const auto cubic_meter = meter * meter * meter;
using Volume = UNIT_TYPE(cubic_meter);
const auto liter = std::milli{} * cubic_meter;

const auto femtoliter = std::femto{} * liter;
const auto picoliter  = std::pico{} * liter;
const auto nanoliter  = std::nano{} * liter;
const auto microliter = std::micro{} * liter;
const auto milliliter = std::milli{} * liter;
const auto centiliter = std::centi{} * liter;
const auto deciliter  = std::deci{} * liter;

SI_LARGER_PREFIXES(liter);

// Velocity

const auto mps = meter / second;
using Velocity = UNIT_TYPE(mps);

// Acceleration

const auto mps2 = mps / second;
using Acceleration = UNIT_TYPE(mps2);
const auto kph = kilometer / hour;
const auto gravity = 9.80665 * mps2;

// Frequency

const auto hertz = second.exp<-1>();
using Frequency = UNIT_TYPE(hertz);
SI_ALL_PREFIXES(hertz);

// Force

const auto newton = kilogram * meter / (second * second);
using Force = UNIT_TYPE(newton);
SI_ALL_PREFIXES(newton);

// Pressure

const auto pascal = newton / (meter * meter);
using Pressure = UNIT_TYPE(pascal);
SI_ALL_PREFIXES(pascal);

// Energy

const auto joule = newton * meter;
using Energy = UNIT_TYPE(joule);
SI_ALL_PREFIXES(joule);

// Power

const auto watt = joule / second;
using Power = UNIT_TYPE(watt);
SI_ALL_PREFIXES(watt);

// Electric charge

const auto coloumb = ampere * second;
using ElectricCharge = UNIT_TYPE(coloumb);
SI_ALL_PREFIXES(coloumb);

// Voltage

const auto volt = watt / ampere;
using Voltage = UNIT_TYPE(volt);
SI_ALL_PREFIXES(volt);

// Electric capacitance

const auto farad = coloumb / volt;
using ElectricCapacitance = UNIT_TYPE(farad);
SI_ALL_PREFIXES(farad);

// Electric resistance

const auto ohm = volt / ampere;
using ElectricResistance = UNIT_TYPE(ohm);
SI_ALL_PREFIXES(ohm);

// Electrical conductance

const auto siemens = ampere / volt;
using ElectricalConductance = UNIT_TYPE(siemens);
SI_ALL_PREFIXES(siemens);

// Magnetic flux

const auto weber = volt * second;
using MagneticFlux = UNIT_TYPE(weber);
SI_ALL_PREFIXES(weber);

// Magnetic field strength

const auto tesla = weber / meter.exp<2>();
using MagneticField = UNIT_TYPE(tesla);
SI_ALL_PREFIXES(tesla);

// Inductance

const auto henry = weber / ampere;
using Inductance = UNIT_TYPE(henry);
SI_ALL_PREFIXES(henry);

// Radioactivity

const auto becquerel = hertz;
using Radioactivity = UNIT_TYPE(becquerel);
SI_ALL_PREFIXES(becquerel);

// Absorbed dose (of ionizing radiation)

const auto gray = joule / kilogram;
using AbsorbedDose = UNIT_TYPE(gray);
SI_ALL_PREFIXES(gray);

// Equivalent dose (of ionizing radiation) J/kg    m2⋅s−2

const auto sievert = joule / kilogram;
using EquivalentDos = UNIT_TYPE(sievert);
SI_ALL_PREFIXES(sievert);

// Catalytic activity

const auto katal = mole / second;
using CatalyticActivity = UNIT_TYPE(katal);
SI_ALL_PREFIXES(katal);

} /* namespace metric */
} /* namespace units */

#endif
