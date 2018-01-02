#include "openweathermapsrc.hxx"

#include "configwritecontext.hxx"

#include <stdexcept>
#include <utility>
#include <unordered_map>
#include <string>
#include <ostream>
#include <fstream>
#include <chrono>
#include <ctime>


constexpr std::chrono::seconds OpenWeatherMapSrc::m_request_interval;

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
    return m_last_request_time > std::chrono::system_clock::now() - OpenWeatherMapSrc::m_request_interval &&
            !m_api_key.empty() && !m_api_host.empty();
}


Weather OpenWeatherMapSrc::read() {
    if (isAvailable()) {
        bool is_rainy = false, is_sunny = false, is_windy = false, is_warm = false;
        //TBD
        return Weather { { is_rainy, is_sunny, is_windy, is_warm } };
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
    return std::chrono::system_clock::from_time_t((std::time_t)0);
}
