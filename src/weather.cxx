#include <iostream>

#include "weatherserver.hxx"


int main() {
    WeatherServer server;
    const Weather w(server.currentWeather());
    const auto& intrp = w.interpretation;
    std::cout << (intrp.is_clear ? "Clear\n" : "Cloudy\n")
            << (intrp.is_windy ? "Windy\n" : "Little or no wind\n")
            << (intrp.is_rainy ? "Rainy\n" : "No rain\n")
            << (intrp.is_warm ? "Warm\n" : "Cool or cold") << std::endl;
}
