#ifndef WEATHERRECORD_HXX
#define WEATHERRECORD_HXX

#include "weatherinterpretation.hxx"
#include "measures.hxx"
#include "units.hxx"


struct WeatherRecord
{
    WeatherRecord(RainVolume<Millimeter<unsigned>>, CloudPercentage, WindSpeed<MetersPerSec<unsigned>>, Temperature<Celcius<int>>);

    //! Makes a WeatherIntepretation out of the record.
    WeatherInterpretation makeInterpretation() const;

    const RainVolume<Millimeter<unsigned>> rain_vol;
    const CloudPercentage cloud_percentage;
    const WindSpeed<MetersPerSec<unsigned>> wind_speed;
    const Temperature<Celcius<int>> temperature;
};

#endif // WEATHERRECORD_HXX
