#include "weatherserver.hxx"

#include "weathersource.hxx"
#include "filesystemweathersrc.hxx"
#include "openweathermapsrc.hxx"

#include <ifstream>
#include <ofstream>
#include <stdexcept>
#include <cstdlib>
#include <string>


const char WeatherServer::_working_dir[] = std::getenv("HOME");

WeatherServer::WeatherServer()
    : m_sources({
        t_pSrc(new FileSystemWeatherSrc(WeatherServer::_working_dir)),
        t_pSrc(new OpenWeatherMapSrc(WeatherServer::_working_dir))
      })
    , m_settings(makeSettings())
{
    for (auto & src : m_sources) {
        src.configure(m_settings);
    }
}


Weather WeatherServer::currentWeather() {
    for (auto & handle : m_handles) {
        if (handle.isAvailable()) {
            return handle.read();
        }
    }
    throw std::runtime_error("none of the weather handles are available");
}


t_settings WeatherServer::makeSettings() const {
    if (const char* home = std::getenv("HOME")) {
        const std::string dir = std::string(home) + "/.mlweather";
        const std::string path = dir + "/" + WeatherServer::_conf_filename;
        std::ifstream conf_file(path);
        if (!conf_file) {
            createDefaultConfig(dir, path);
        }
        conf_file.open(path);
        if (conf_file) {
            t_settings settings;
            std::string key, value;
            while (std::getline(conf_file, key, ":")) {
                if (!key.empty()) {
                    std::getline(conf_file, value);
                    settings[key] = value;
                }
            }
            return settings;
        } else {
            throw std::runtime_error(path + ": failed to open config file for reading.");
        }
    } else {
        throw std::runtime_error("failed to get env var for home dir");
    }
}


void WeatherServer::createDefaultConfig(std::string const & parent_dir, std::string const & full_path) const {
    ::mkdir(parent_dir.c_str())
    std::ofstream conf_file(full_path);
    if (conf_file) {
        for (auto const & src : m_sources) {
            src.writeDefaultConfig(conf_file);
        }
    }
}
