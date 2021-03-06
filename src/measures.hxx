#ifndef MEASURES_HXX
#define MEASURES_HXX

#include <utility>


/*!
 * \tparam T Measurement unit
 */
template <class T>
struct Measure {
    T value;

    explicit Measure(T value)
        : value(std::move(value))
    {}

    Measure() = default;

    operator T () const {
        return value;
    }
};

template <class T>
struct RainVolume : public Measure<T> {
    using Measure<T>::Measure;
};

template <class T>
struct WindSpeed : public Measure<T> {
    using Measure<T>::Measure;
};

template <class T>
struct Temperature : public Measure<T> {
    using Measure<T>::Measure;
};

struct CloudPercentage : public Measure<unsigned> {
    using Measure<unsigned>::Measure;
};

struct HumidityPercentage : public Measure<unsigned> {
    using Measure<unsigned>::Measure;
};


#endif // MEASURES_HXX
