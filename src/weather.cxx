#include <iostream>

#include "weatherserver.hxx"


int main() {
    WeatherServer server;
    const Weather w(server.currentWeather());
    const auto& intrp = w.interpretation;
    const auto& rec = w.record;
    std::cout << (intrp.is_clear ? "Clear\n" : "Cloudy\n")
            << (intrp.is_sunny ? "Sunny\n" : "")
            << (rec.is_sun_up ? "Sun is up\n" : "Sun is down\n")
            << (intrp.is_windy ? "Windy\n" : "Little or no wind\n")
            << (intrp.is_rainy ? "Rainy\n" : "No rain\n")
            << (intrp.is_warm ? "Warm\n" : "Cool or cold") << std::endl;
}
