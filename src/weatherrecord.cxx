#include "weatherrecord.hxx"

#include "weatherinterpretation.hxx"
#include "json.hpp"

#include <cmath>
#include <chrono>


void from_json(const nlohmann::json& j, WeatherRecord& wr) {
    // JSON spec at https://openweathermap.org/current

    unsigned unix_now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    auto rain_it = j.find("rain");
    if (rain_it != j.end()) {
        wr.rain_vol.value = (unsigned)(*rain_it).at("3h");
    } else {
        wr.rain_vol.value = 0;
    }

    auto snow_it = j.find("snow");
    if (snow_it != j.end()) {
        wr.snow_vol.value = (unsigned)(*snow_it).at("3h");
    } else {
        wr.snow_vol.value = 0;
    }

    wr.cloud_percentage.value = j.at("clouds").at("all");
    wr.wind_speed.value = (unsigned)std::round((float)j.at("wind").at("speed"));
    wr.temperature.value = (int)j.at("main").at("temp");
    wr.humidity_percentage.value = j.at("main").at("humidity");
    wr.sunrise_time = j.at("sys").at("sunrise");
    wr.sunset_time = j.at("sys").at("sunset");
    wr.is_sun_up = unix_now > wr.sunrise_time && unix_now < wr.sunset_time;
}
