#ifndef WEATHERRECORD_HXX
#define WEATHERRECORD_HXX

#include "weatherinterpretation.hxx"
#include "measures.hxx"
#include "units.hxx"

#include "json.hpp"


struct WeatherRecord
{
    //! Makes a WeatherIntepretation out of the record.
    WeatherInterpretation makeInterpretation() const;

    RainVolume<Millimeter<unsigned>> rain_vol;
    CloudPercentage cloud_percentage;
    WindSpeed<MetersPerSec<unsigned>> wind_speed;
    Temperature<Celcius<int>> temperature;
};

void from_json(const nlohmann::json& j, WeatherRecord& wr);

#endif // WEATHERRECORD_HXX
