#ifndef WEATHERRECORD_HXX
#define WEATHERRECORD_HXX

#include "weatherinterpretation.hxx"
#include "measures.hxx"
#include "units.hxx"


class WeatherRecord
{
    public:
        WeatherRecord(RainVolume<Millimeter<unsigned>>, CloudPercentage, WindSpeed<MetersPerSec<unsigned>>, Temperature<Celcius<int>>);

        //! Makes a WeatherIntepretation out of the record.
        WeatherInterpretation makeInterpretation() const;
    private:
        RainVolume<Millimeter<unsigned>> m_rain_vol;
        CloudPercentage m_cloud_percentage;
        WindSpeed<MetersPerSec<unsigned>> m_wind_speed;
        Temperature<Celcius<int>> m_temperature;
};

#endif // WEATHERRECORD_HXX
