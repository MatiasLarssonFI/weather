#ifndef OPENWEATHERMAPSRC_HXX
#define OPENWEATHERMAPSRC_HXX

#include "weathersource.hxx"
#include "configwritecontext.hxx"
#include "http/http_request.hxx"
#include "measures.hxx"
#include "units.hxx"
#include "weatherrecord.hxx"

#include "json.hpp"

#include <chrono>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <ctime>


//! Source for OpenWeatherMap
/*!
 *\tparam T Implements WeatherInterpretation interpret(WeatherRecord const & wr) const;
 */
template <class T>
class OpenWeatherMapSrc : public WeatherSource
{
    public:
        OpenWeatherMapSrc(std::string working_dir)
            : m_wd(std::move(working_dir))
            , m_last_request_time(makeLastRequestTime())
        {}


        void configure(std::unordered_map<std::string, std::string> const & settings) {
            m_out_path = m_wd + "/" + settings.at("openweathermapsrc_out");
            m_api_key = settings.at("openweathermapsrc_apikey");
            m_api_host = settings.at("openweathermapsrc_host");
            m_city_id = settings.at("openweathermapsrc_city_id");
        }


        Weather read() {
            if (isAvailable()) {
                HTTPRequest r(m_api_host + "/data/2.5/weather?id=" + m_city_id  + "&units=metric&APPID=" + m_api_key);
                const HTTPResponse resp = r.perform();
                std::string const & resp_body = resp.body();
                const unsigned http_status = resp.HTTPCode();
                if (http_status < 300 && http_status >= 200) {
                    saveResponseBody(resp_body);
                    m_last_request_time = std::chrono::system_clock::now();
                    using json = nlohmann::json;
                    json j = json::parse(resp_body);
                    auto wr = j.get<WeatherRecord>();
                    const T interpreter;
                    WeatherInterpretation interpretation(interpreter.interpret(wr));
                    return { std::move(interpretation), std::move(wr) };
                } else {
                    throw std::runtime_error(std::string("OpenWeatherMapSrc request failed. Body: ") + resp_body);
                }
            } else {
                throw std::runtime_error("OpenWeatherMapSrc not available");
            }
        }


        bool isAvailable() const {
            return m_last_request_time == std::chrono::system_clock::time_point::min() ||
                    ( m_last_request_time < std::chrono::system_clock::now() - OpenWeatherMapSrc::m_request_interval &&
                    !m_api_key.empty() && !m_api_host.empty() );
        }


        void writeDefaultConfig(ConfigWriteContext & ctx) const {
            ctx.add("openweathermapsrc_apikey", "[api key]");
            ctx.add("openweathermapsrc_host", "[host address]");
            ctx.add("openweathermapsrc_out", "[file which API response is written to]");
            ctx.add("openweathermapsrc_city_id", "[city ID]");
        }

        virtual ~OpenWeatherMapSrc() {
            saveRequestTime();
        }
    private:
        //! Loads the last request time (from disk).
        std::chrono::system_clock::time_point makeLastRequestTime() const {
            std::ifstream f(m_wd + "/" + m_request_time_f);
            if (f.is_open()) {
                unsigned long t;
                f >> t;
                if (f.good()) {
                    return std::chrono::system_clock::time_point(std::chrono::system_clock::duration(t));
                }
            }
            return std::chrono::system_clock::time_point::min();
        }


        //! Writes the last request time to disk
        void saveRequestTime() const {
            std::ofstream f(m_wd + "/" + m_request_time_f, std::ios_base::out|std::ios_base::trunc);
            if (f.is_open()) {
                f << m_last_request_time.time_since_epoch().count() << "\n";
            }
        }

        //! Writes the HTTP response body to disk
        void saveResponseBody(std::string const & body) const {
            std::ofstream f(m_out_path, std::ios_base::out|std::ios_base::trunc);
            if (f.is_open()) {
                f << body << "\n";
            }
        }


        static constexpr auto m_request_interval = std::chrono::minutes(10);
        static constexpr auto m_request_time_f = "openweathermapsrc_lastrequest";

        const std::string m_wd;
        std::chrono::system_clock::time_point m_last_request_time;

        std::string m_out_path;
        std::string m_api_key;
        std::string m_api_host;
        std::string m_city_id;
};

template <class T>
constexpr std::chrono::minutes OpenWeatherMapSrc<T>::m_request_interval;

#endif // OPENWEATHERMAPSRC_HXX
