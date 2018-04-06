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
    : m_sources() // emplacements will be done by initSources()
    , m_settings_helper(WeatherServer::_conf_filename, m_sources, WeatherServer::_working_dir)
    , m_settings(m_settings_helper.settings()) // assignment in body
{
    initSources();
    // m_sources must be populated in order to makeSettings()
    m_settings_helper.updateSettings();

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


void WeatherServer::initSources() {
    const std::string intrStrategy = m_settings.count("interpretation") ? m_settings.at("interpretation") : "nordic";
    if (intrStrategy == "nordic") {
        using Tintr = NordicInterpreter;
        emplaceSources<Tintr, OpenWeatherMapSrc<Tintr>, FileSystemWeatherSrc<Tintr>>();
    } else {
        throw std::runtime_error(std::string("bad weather interpreter strategy ") + intrStrategy);
    }
}


template <class Tintr, class Tsrc, class... Tsources>
void WeatherServer::emplaceSources() {
    m_sources.emplace_back(new Tsrc(WeatherServer::_working_dir));
    emplaceSources<Tintr, Tsources...>();
}


// Terminal method
template <class Tintr>
void WeatherServer::emplaceSources() const {}
