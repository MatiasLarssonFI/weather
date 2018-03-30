#ifndef OPENWEATHERMAPSRC_HXX
#define OPENWEATHERMAPSRC_HXX

#include "weathersource.hxx"
#include "configwritecontext.hxx"

#include <chrono>
#include <unordered_map>
#include <string>
#include <ostream>


//! Source for OpenWeatherMap
class OpenWeatherMapSrc : public WeatherSource
{
    public:
        OpenWeatherMapSrc(std::string working_dir);

        virtual void configure(std::unordered_map<std::string, std::string> const & settings);
        virtual Weather read();
        virtual bool isAvailable() const;
        virtual void writeDefaultConfig(ConfigWriteContext &) const;

        virtual ~OpenWeatherMapSrc();
    private:
        //! Loads the last request time (from disk).
        std::chrono::system_clock::time_point makeLastRequestTime() const;

        //! Writes the last request time to d
        void saveRequestTime() const;

        //! Writes the HTTP response body to disk
        void saveResponseBody(std::string const &) const;

        static constexpr auto m_request_interval = std::chrono::minutes(10);

        static constexpr auto m_request_time_f = "openweathermapsrc_lastrequest";

        const std::string m_wd;
        std::chrono::system_clock::time_point m_last_request_time;

        std::string m_out_path;
        std::string m_api_key;
        std::string m_api_host;
        std::string m_city_id;
};

#endif // OPENWEATHERMAPSRC_HXX
