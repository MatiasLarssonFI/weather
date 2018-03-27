#ifndef UNITS_HXX
#define UNITS_HXX

#include <type_traits>


/*!
 * \tparam T Data type
 */
template <class T, class U = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
struct Unit {
    const T value;

    Unit(T value)
        : value(value)
    {}

    Unit() = default;

    operator T () const {
        return value;
    }

    friend bool operator < (Unit<T> r, T l) {
        return r.value < l;
    }

    friend bool operator > (Unit<T> r, T l) {
        return l < r.value;
    }

    friend bool operator == (Unit<T> r, T l) {
        return r.value == l;
    }
};


template <class T>
struct Millimeter : public Unit<T> {
    using Unit<T>::Unit;

};

template <class T>
struct MetersPerSec : public Unit<T> {
    using Unit<T>::Unit;
};

template <class T>
struct Celcius : public Unit<T> {
    using Unit<T>::Unit;
};

#endif // UNITS_HXX
