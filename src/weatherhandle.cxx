#include "weatherhandle.hxx"

WeatherHandle::WeatherHandle(t_src src)
    : m_src(std::move(src))
{}


Weather WeatherHandle::read() {
    return m_src.read();
}


bool WeatherHandle::isAvailable() const {
    return m_src.isAvailable();
}
