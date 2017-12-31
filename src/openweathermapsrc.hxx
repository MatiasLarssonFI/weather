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
        OpenWeatherMapSrc(std::string working_dir);

        virtual void configure(std::unordered_map<std::string, std::string> const & settings);
        virtual Weather read();
        virtual bool isAvailable() const;
        virtual void writeDefaultConfig(std::ostream & stream) const;

        virtual ~OpenWeatherMapSrc();
    private:
        std::chrono::system_clock::time_point makeLastRequestTime() const;


        std::chrono::system_clock::time_point m_last_request_time;
        static constexpr auto m_request_interval = std::chrono::seconds(5);

        static constexpr auto m_request_time_f = "/tmp/openweathermapsrc_lastrequest";

        const std::string m_wd;
        std::string m_out_path;
        std::string m_api_key;
        std::string m_api_host;
};

#endif // OPENWEATHERMAPSRC_HXX
