#ifndef WEATHERSERVER_HXX
#define WEATHERSERVER_HXX

#include "filesystemweathersrc.hxx"
#include "openweathermapsrc.hxx"

#include <unordered_map>
#include <string>
#include <memory>


class WeatherServer
{
    public:
        using t_settings = std::unordered_map<std::string, std::string>;
        using t_pSrc = std::unique_ptr<WeatherSource>;


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


        /*!
         * Creates the default config file.
         */
        void createDefaultConfig(std::string const & parent_dir, std::string const & full_path) const;


        t_settings m_settings;
        std::vector<t_pSrc> m_sources; //!< sources are ordered by descending priority

        static const char _working_dir[];
        static constexpr char _conf_filename[] = "server_config";
};

#endif // WEATHERSERVER_HXX
