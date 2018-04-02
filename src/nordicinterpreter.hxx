#ifndef NORDICINTERPRETER_HXX
#define NORDICINTERPRETER_HXX

#include "weatherinterpretation.hxx"
#include "weatherrecord.hxx"
#include "units.hxx"
#include "measures.hxx"


struct NordicInterpreter
{
    WeatherInterpretation interpret(WeatherRecord const & wr) const;
};

#endif // NORDICINTERPRETATION_HXX
