#ifndef WEATHER_HXX
#define WEATHER_HXX

#include "weatherinterpretation.hxx"
#include "weatherrecord.hxx"


class Weather
{
    public:
        WeatherInterpretation interpretation;
        WeatherRecord record;
};

#endif // WEATHER_HXX
