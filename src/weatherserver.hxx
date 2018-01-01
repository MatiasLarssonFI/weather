#ifndef WEATHERSERVER_HXX
#define WEATHERSERVER_HXX

#include "filesystemweathersrc.hxx"
#include "openweathermapsrc.hxx"
#include "weatherserversettings.hxx"

#include <unordered_map>
#include <string>
#include <memory>


class WeatherServer
{
    public:
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
        using t_pSrc = std::unique_ptr<WeatherSource>;
        using t_settings = WeatherServerSettings<t_pSrc>::t_settings;

        std::vector<t_pSrc> m_sources; //!< sources are ordered by descending priority
        WeatherServerSettings<t_pSrc> m_settings_helper;
        t_settings m_settings;

        static const std::string _working_dir;
        static constexpr char _conf_filename[] = "server_config";
};


#endif // WEATHERSERVER_HXX
