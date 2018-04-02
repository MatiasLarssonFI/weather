#include "nordicinterpreter.hxx"

#include "weatherinterpretation.hxx"
#include "weatherrecord.hxx"


WeatherInterpretation NordicInterpreter::interpret(WeatherRecord const & wr) const {
    const bool is_rainy = wr.rain_vol > (unsigned)1,
        is_sunny = wr.cloud_percentage < (unsigned)10,
        is_windy = wr.wind_speed > (unsigned)5,
        is_warm = wr.temperature > 12;

    return WeatherInterpretation { is_rainy, is_sunny, is_windy, is_warm };
}
