#ifndef WEATHERSERVER_HXX
#define WEATHERSERVER_HXX

#include "filesystemweathersrc.hxx"
#include "openweathermapsrc.hxx"

#include <unordered_map>
#include <string>
#include <ifstream>


class WeatherServer
{
    public:
        using t_settings = std::unordered_map<std::string, std::string>;


        /*!
         * Constructor.
         * \throw std::runtime_error
         */
        WeatherServer();


        /*!
         * Get current weather.
         *
         * \throw std::runtime_error If weather info is unavailable
         * \return Current weather
         */
        Weather currentWeather();


        virtual ~WeatherServer() {};
    private:
        /*!
         * \throw std::runtime_error
         */
        FileSystemWeatherSrcConf makeFSConf() const;


        /*!
         * \throw std::runtime_error
         */
        OpenWeatherMapSrcConf makeOWMConf() const;


        /*!
         * \throw std::runtime_error
         */
        t_settings makeSettings() const;


        t_settings m_settings;
        std::vector<WeatherHandle> m_handles; //!< handles are ordered by descending priority
};

#endif // WEATHERSERVER_HXX
