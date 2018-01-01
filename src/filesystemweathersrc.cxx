#include "filesystemweathersrc.hxx"

#include "weather.hxx"

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <unistd.h>


FileSystemWeatherSrc::FileSystemWeatherSrc(std::string working_dir)
    : m_wd(std::move(working_dir))
{}


void FileSystemWeatherSrc::configure(std::unordered_map<std::string, std::string> const & settings) {
    m_path = m_wd + "/" + settings.at("filesystemweathersrc_in");
}


bool FileSystemWeatherSrc::isAvailable() const {
    return access(m_path.c_str(), R_OK) == 0; // if file is readable
}


Weather FileSystemWeatherSrc::read() {
    if (isAvailable()) {
        bool is_rainy = false, is_sunny = false, is_windy = false, is_warm = false;
        //TBD
        return Weather { { is_rainy, is_sunny, is_windy, is_warm } };
    } else {
        throw std::runtime_error("FileSystemWeatherSrc not available");
    }
}


void FileSystemWeatherSrc::writeDefaultConfig(std::ostream & stream) const {
    stream << "filesystemweathersrc_in: " << m_wd << "/fs_src_in\n";
}
