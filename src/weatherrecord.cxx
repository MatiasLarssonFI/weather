#include "weatherrecord.hxx"

#include "weatherinterpretation.hxx"
#include "measures.hxx"
#include "units.hxx"

#include <utility>


WeatherRecord::WeatherRecord(RainVolume<Millimeter<unsigned>> rv, CloudPercentage cp, WindSpeed<MetersPerSec<unsigned>> ws, Temperature<Celcius<int>> t)
    : m_rain_vol(std::move(rv))
    , m_cloud_percentage(std::move(cp))
    , m_wind_speed(std::move(ws))
    , m_temperature(std::move(t))
{}


WeatherInterpretation WeatherRecord::makeInterpretation() const {
    const bool is_rainy = m_rain_vol > (unsigned)1,
        is_sunny = m_cloud_percentage < (unsigned)10,
        is_windy = m_wind_speed > (unsigned)5,
        is_warm = m_temperature > 12;

    return WeatherInterpretation { is_rainy, is_sunny, is_windy, is_warm };
}
