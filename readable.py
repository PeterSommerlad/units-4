#!/usr/bin/python

import sys, re, fileinput


space = "[\\s\n]"
commaRegex = space + "*," + space + "*"

def paren(regex):
    return "(" + regex + ")"

def commaList(regex):
    return paren(regex) + paren(commaRegex + regex) + "*"

def templateParam(regex):
    return "<" + space + "*" + regex + space + "*>"

integralConstantRegex = "std::__1::integral_constant" + templateParam("char" + commaRegex + "'(\\w)'")
compileStringRegex = paren("units::compile_string" + templateParam(paren(commaList(integralConstantRegex))))
dimensionRegex = "units::dimension" + templateParam((compileStringRegex) + commaRegex + (compileStringRegex))
dimExpRegex = "units::unit_detail::dim_exp" + templateParam(paren(dimensionRegex) + commaRegex + "(-?\\d+)")
unitRegex = "units::unit" + templateParam(commaList(dimExpRegex))
numberRegex = "units::unit_number" + templateParam("(\\w+)" + commaRegex + paren(unitRegex))

# integralConstantRegex = re.compile(integralConstantRegex, re.M)
# compileStringRegex = re.compile(compileStringRegex, re.M)
# dimensionRegex = re.compile(dimensionRegex, re.M)
# dimExpRegex = re.compile(dimExpRegex, re.M)
# unitRegex = re.compile(unitRegex, re.M)
# numberRegex = re.compile(numberRegex, re.M)
# print compileStringRegex

def isCompileString(text):
    return re.match(compileStringRegex, text) != None

def isDimExp(text):
    return re.match(dimExpRegex, text) != None

def isUnit(text):
    return re.match(unitRegex, text) != None

def parseCompileString(cstring):
    value = ''
    for x in cstring.split('std::__1::integral_constant'):
        match = re.match("<char,\\s*'(\\w)'>,?", x.strip())
        if match != None:
            value += match.group(1)
    return value

def parseDimExp(match):
    dimName = parseCompileString(match[2])
    baseUnit = parseCompileString(match[8])
    exp = int(match[-1])
    return (dimName, baseUnit, exp)

def parseUnit(unit):
    dimexps = []
    for result in re.findall(paren(dimExpRegex), unit):
        assert isDimExp(result[0])
        dimexps.append(parseDimExp(result))
    return dimexps

def formatDimExp(dim, exp):
    if exp == 1:
        return dim
    else:
        return '%s^%d' % (dim, exp)

def formatDims(dims, index=0, invert=False):
    factor = -1 if invert else 1
    return '*'.join(formatDimExp(dim[index], factor * dim[-1]) for dim in dims)

def formatUnit(unit, index=0):
    if len(unit) == 0:
        return '[scalar]'
    positiveDims = filter(lambda (n, b, exp) : exp > 0, unit)
    negativeDims = filter(lambda (n, b, exp) : exp < 0, unit)
    result = ''
    if len(positiveDims) == 0:
        result += '1'
    else:
        f = formatDims(positiveDims, index=index, invert=False)
        if len(positiveDims) != 1:
            f = paren(f)
        result += f
    if len(negativeDims) != 0:
        result += '/'
        f = formatDims(negativeDims, index=index, invert=True)
        if len(negativeDims) != 1:
            f = paren(f)
        result += f

    return result

def parseUnitNumber(match):
    number = match.group(1)
    unit = parseUnit(match.group(2))
    return number + paren(formatUnit(unit, index=1))

contents = ''
for line in fileinput.input():
    contents += line

print re.sub(numberRegex, parseUnitNumber, contents, re.M),
