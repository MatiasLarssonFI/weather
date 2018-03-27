#ifndef MEASURES_HXX
#define MEASURES_HXX

#include <utility>


/*!
 * \tparam T Measurement unit
 */
template <class T>
struct Measure {
    T value;

    Measure(T value)
        : value(std::move(value))
    {}

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


#endif // MEASURES_HXX
