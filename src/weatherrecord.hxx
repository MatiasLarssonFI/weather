#ifndef WEATHERRECORD_HXX
#define WEATHERRECORD_HXX

#include "measures.hxx"
#include "units.hxx"

#include "json.hpp"


struct WeatherRecord
{
    RainVolume<Millimeter<unsigned>> rain_vol;
    CloudPercentage cloud_percentage;
    WindSpeed<MetersPerSec<unsigned>> wind_speed;
    Temperature<Celcius<int>> temperature;
    unsigned sunrise_time;
    unsigned sunset_time;
    bool is_sun_up;
};

void from_json(const nlohmann::json& j, WeatherRecord& wr);

#endif // WEATHERRECORD_HXX
