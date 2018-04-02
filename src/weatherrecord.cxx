#include "weatherrecord.hxx"

#include "weatherinterpretation.hxx"
#include "json.hpp"

#include <cmath>


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
