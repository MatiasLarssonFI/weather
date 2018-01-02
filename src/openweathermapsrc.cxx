#include "openweathermapsrc.hxx"

#include "configwritecontext.hxx"
#include "json.hpp"
#include "http/http_request.hxx"

#include <stdexcept>
#include <utility>
#include <unordered_map>
#include <string>
#include <ostream>
#include <fstream>
#include <chrono>
#include <ctime>


constexpr std::chrono::minutes OpenWeatherMapSrc::m_request_interval;

OpenWeatherMapSrc::OpenWeatherMapSrc(std::string working_dir)
    : m_last_request_time(makeLastRequestTime())
    , m_wd(std::move(working_dir))
{}


void OpenWeatherMapSrc::configure(std::unordered_map<std::string, std::string> const & settings) {
    m_out_path = m_wd + "/" + settings.at("openweathermapsrc_out");
    m_api_key = settings.at("openweathermapsrc_apikey");
    m_api_host = settings.at("openweathermapsrc_host");
    m_city_id = settings.at("openweathermapsrc_city_id");
}


bool OpenWeatherMapSrc::isAvailable() const {
    return m_last_request_time == std::chrono::system_clock::time_point::min() ||
            ( m_last_request_time > std::chrono::system_clock::now() - OpenWeatherMapSrc::m_request_interval &&
            !m_api_key.empty() && !m_api_host.empty() );
}


Weather OpenWeatherMapSrc::read() {
    if (isAvailable()) {
        HTTPRequest r(m_api_host + "/data/2.5/weather?id=" + m_city_id  + "&units=metric&APPID=" + m_api_key);
        HTTPResponse resp = r.perform();
        unsigned http_status = resp.HTTPCode();
        if (http_status < 300 && http_status >= 200) {
            bool is_rainy = false, is_sunny = false, is_windy = false, is_warm = false;
            using json = nlohmann::json;
            std::string const & resp_body = resp.body();
            json j = json::parse(resp_body);
            is_rainy = j["rain"]["3h"] > 1; // mm
            is_sunny = j["clouds"]["all"] < 10; // %
            is_windy = j["wind"]["speed"] > 5; // m/s
            is_warm = j["main"]["temp"] > 12; // °C
            m_last_request_time = std::chrono::system_clock::now();
            save_request_time(m_last_request_time);
            save_response_body(resp_body);
            return Weather { { is_rainy, is_sunny, is_windy, is_warm } };
        } else {
            throw std::runtime_error(std::string("OpenWeatherMapSrc request failed. Body: ") + resp.body());
        }
    } else {
        throw std::runtime_error("OpenWeatherMapSrc not available");
    }
}


void OpenWeatherMapSrc::writeDefaultConfig(ConfigWriteContext & ctx) const {
    ctx.add("openweathermapsrc_apikey", "[api key]");
    ctx.add("openweathermapsrc_host", "[host address]");
    ctx.add("openweathermapsrc_out", "[file which API response is written to]");
    ctx.add("openweathermapsrc_city_id", "[city ID]");
}


OpenWeatherMapSrc::~OpenWeatherMapSrc() {
    std::ofstream f(m_wd + "/" + m_request_time_f, std::ios_base::out|std::ios_base::trunc);
    if (f.is_open()) {
        f << m_last_request_time.time_since_epoch().count() << "\n";
    }
}


std::chrono::system_clock::time_point OpenWeatherMapSrc::makeLastRequestTime() const {
    std::ifstream f(m_wd + "/" + m_request_time_f);
    if (f.is_open()) {
        unsigned s;
        f >> s;
        if (f.good()) {
            return std::chrono::system_clock::time_point(std::chrono::system_clock::duration(s));
        }
    }
    return std::chrono::system_clock::time_point::min();
}
