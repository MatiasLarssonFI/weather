#include "filesystemweathersrc.hxx"

#include "weather.hxx"
#include "configwritecontext.hxx"
#include "measures.hxx"
#include "units.hxx"
#include "weatherrecord.hxx"

#include "json.hpp"

#include <ostream>
#include <fstream>
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
        using json = nlohmann::json;
        std::ifstream json_file(m_path);
        json j;
        json_file >> j;
        auto wr = j.get<WeatherRecord>();
        return { wr.makeInterpretation(), std::move(wr) };
    } else {
        throw std::runtime_error("FileSystemWeatherSrc not available");
    }
}


void FileSystemWeatherSrc::writeDefaultConfig(ConfigWriteContext & ctx) const {
    ctx.add("filesystemweathersrc_in", m_wd + "/fs_src_in");
}
