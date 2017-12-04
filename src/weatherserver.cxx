#include "weatherserver.hxx"

#include "filesystemweathersrc.hxx"
#include "openweathermapsrc.hxx"

#include <ifstream>
#include <stdexcept>
#include <cstdlib>
#include <string>


WeatherServer::WeatherServer()
    : m_settings(makeSettings())
    , m_handles({
        new WeatherHandle<FileSystemWeatherSrc> (new FileSystemWeatherSrc(makeFSConf()))
        , new WeatherHandle<OpenWeatherMapSrc> (new OpenWeatherMapSrc(makeOWMConf()))
    })
{}


Weather WeatherServer::currentWeather() {
    for (auto & handle : m_handles) {
        if (handle.isAvailable()) {
            return handle.read();
        }
    }
    throw std::runtime_error("none of the weather handles are available");
}


FileSystemWeatherSrcConf makeFSConf() const {

}


OpenWeatherMapSrcConf makeOWMConf() const {

}


t_settings makeSettings() const {
    if (const char* home = std::getenv("HOME")) {
        const std::string path = std::string(home) + "/.mlweather/config";
        std::ifstream conf_file(path);
        if (conf_file.is_open()) {
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
