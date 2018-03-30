#include "weatherrecord.hxx"

#include "weatherinterpretation.hxx"
#include "json.hpp"

#include <utility>


WeatherInterpretation WeatherRecord::makeInterpretation() const {
    const bool is_rainy = rain_vol > (unsigned)1,
        is_sunny = cloud_percentage < (unsigned)10,
        is_windy = wind_speed > (unsigned)5,
        is_warm = temperature > 12;

    return WeatherInterpretation { is_rainy, is_sunny, is_windy, is_warm };
}


void from_json(const nlohmann::json& j, WeatherRecord& wr) {
    // JSON spec at https://openweathermap.org/current
    auto rain_it = j.find("rain");
    if (rain_it != j.end()) {
        wr.rain_vol.value = (unsigned)(*rain_it).at("3h");
    }

    wr.cloud_percentage.value = (unsigned)j.at("clouds").at("all");
    wr.wind_speed.value = (unsigned)j.at("wind").at("speed");
    wr.temperature.value = (int)j.at("main").at("temp");
}
