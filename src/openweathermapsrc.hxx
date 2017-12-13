#ifndef OPENWEATHERMAPSRC_HXX
#define OPENWEATHERMAPSRC_HXX

#include "weathersource.hxx"

#include <chrono>
#include <unordered_map>
#include <string>
#include <ostream>


class OpenWeatherMapSrc : public WeatherSource
{
    public:
        OpenWeatherMapSrc(OpenWeatherMapSrcConf conf);

        virtual void configure(std::unordered_map<std::string, std::string> const & settings);
        virtual Weather read();
        virtual bool isAvailable() const;
        virtual void writeDefaultConfig(std::ostream & stream) const;

        virtual ~OpenWeatherMapSrc();
    private:
        std::chrono::time_point makeLastRequestTime() const;


        std::chrono::system_clock::time_point m_last_request_time;
        static inline constexpr auto m_request_interval = std::chrono::seconds(5);

        static inline constexpr auto m_request_time_f = "/tmp/openweathermapsrc_lastrequest";

        const std::string m_wd;
        const std::string m_out_path;
        const std::string m_api_key;
        const std::string m_api_host;
};

#endif // OPENWEATHERMAPSRC_HXX
