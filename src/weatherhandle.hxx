#ifndef WEATHERHANDLE_HXX
#define WEATHERHANDLE_HXX


/*!
 * Proxy to a weather source.
 *
 * Provides a uniform interface to any weather
 * source.
 *
 * \tparam T a weather source, aliased to t_src
 */
template <class T>
class WeatherHandle
{
    using t_src = T;
    public:
        /*!
         * Constructor.
         * \param src The weather source
         */
        WeatherHandle(t_src src);


        /*!
         * Read weather.
         *
         * \throw std::runtime_error If !isAvailable()
         * \return The weather from t_src.
         */
        Weather read();


        /*!
         * Returns true if the handle can be read from.
         *
         * \return True if the handle can be read from.
         */
        bool isAvailable() const;
    private:
        t_src m_src;
};

#endif // WEATHERHANDLE_HXX
