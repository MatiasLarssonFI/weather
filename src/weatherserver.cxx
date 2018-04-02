#include "weatherserver.hxx"

#include "weathersource.hxx"
#include "filesystemweathersrc.hxx"
#include "openweathermapsrc.hxx"
#include "nordicinterpreter.hxx"

#include <stdexcept>
#include <cstdlib>
#include <string>


constexpr char WeatherServer::_conf_filename[];
const std::string WeatherServer::_working_dir = std::string(std::getenv("HOME")) + "/.mlweather";

WeatherServer::WeatherServer()
    : m_sources() // emplacements in body
    , m_settings_helper(WeatherServer::_conf_filename, m_sources, WeatherServer::_working_dir)
    , m_settings() // assignment in body
{
    using Tintr = NordicInterpreter;
    initSources<Tintr, OpenWeatherMapSrc<Tintr>, FileSystemWeatherSrc<Tintr>>();
    // m_sources must be populated in order to makeSettings()
    m_settings = m_settings_helper.makeSettings();

    // configure sources using settings
    for (auto & src : m_sources) {
        src->configure(m_settings);
    }
}


Weather WeatherServer::currentWeather() {
    for (auto & src : m_sources) {
        if (src->isAvailable()) {
            return src->read();
        }
    }
    throw std::runtime_error("none of the weather sources are available");
}
