#ifndef WEATHER_HXX
#define WEATHER_HXX

#include "weatherinterpretation.hxx"
#include "weatherrecord.hxx"


struct Weather
{
    const WeatherInterpretation interpretation;
    const WeatherRecord record;
};

#endif // WEATHER_HXX
