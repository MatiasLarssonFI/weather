#include "nordicinterpreter.hxx"

#include "weatherinterpretation.hxx"
#include "weatherrecord.hxx"


WeatherInterpretation NordicInterpreter::interpret(WeatherRecord const & wr) const {
    const bool is_rainy = wr.rain_vol > (unsigned)1,
        is_snowy = wr.snow_vol > (unsigned)1,
        is_clear = wr.cloud_percentage < (unsigned)10,
        is_windy = wr.wind_speed > (unsigned)5,
        is_warm = wr.temperature > 12,
        is_hot = wr.temperature > 16,
        is_cold = wr.temperature < 2,
        is_freezing = wr.temperature < -8;
    const bool is_sunny = is_clear && wr.is_sun_up;

    return WeatherInterpretation {
        is_rainy, is_snowy, is_sunny, is_clear, is_windy, is_warm,
        is_hot, is_cold, is_freezing
    };
}
