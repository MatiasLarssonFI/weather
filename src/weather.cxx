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
            << (intrp.is_windy ? "Windy\n" : "")
            << (intrp.is_rainy ? "Rainy\n" : "")
            << (intrp.is_snowy ? "Snowy\n" : "")
            << (intrp.is_hot ? "Hot\n" : "")
            << (intrp.is_cold ? "Cold\n" : "")
            << (intrp.is_freezing ? "Freezing\n" : "")
            << (intrp.is_warm ? "Warm\n" : "") << std::endl;
}
