#ifndef WEATHERHANDLE_HXX
#define WEATHERHANDLE_HXX

#include "weather.hxx"

#include <ostream>
#include <unordered_map>


/*!
 * Abstract weather source.
 */
class WeatherSource
{
    public:
        /*!
         * Configures the source.
         *
         * settings is a map of the settings and must contain
         * the settings of the source.
         */
        virtual void configure(std::unordered_map<std::string, std::string> const & settings) = 0;


        /*!
         * Read weather.
         *
         * \throw std::runtime_error If !isAvailable()
         * \return The weather.
         */
        virtual Weather read() = 0;


        /*!
         * Returns true if the source can be read from.
         *
         * \return True if the source can be read from.
         */
        virtual bool isAvailable() const = 0;


        /*!
         * Writes the default configuration to the stream.
         *
         * The configuration format in regex is "(^key: value$)*".
         * The key should be prefixed with the weather source name.
         */
        virtual void writeDefaultConfig(std::ostream & stream) const = 0;


        virtual ~WeatherSource() {};
};

#endif // WEATHERHANDLE_HXX
