#include "weatherrecord.hxx"

#include "weatherinterpretation.hxx"
#include "measures.hxx"
#include "units.hxx"

#include <utility>


WeatherRecord::WeatherRecord(RainVolume<Millimeter<unsigned>> rv, CloudPercentage cp, WindSpeed<MetersPerSec<unsigned>> ws, Temperature<Celcius<int>> t)
    : rain_vol(std::move(rv))
    , cloud_percentage(std::move(cp))
    , wind_speed(std::move(ws))
    , temperature(std::move(t))
{}


WeatherInterpretation WeatherRecord::makeInterpretation() const {
    const bool is_rainy = rain_vol > (unsigned)1,
        is_sunny = cloud_percentage < (unsigned)10,
        is_windy = wind_speed > (unsigned)5,
        is_warm = temperature > 12;

    return WeatherInterpretation { is_rainy, is_sunny, is_windy, is_warm };
}
