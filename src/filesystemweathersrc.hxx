#ifndef FILESYSTEMWEATHERSOURCE_HXX
#define FILESYSTEMWEATHERSOURCE_HXX

#include "weather.hxx"
#include "weathersource.hxx"
#include "configwritecontext.hxx"
#include "measures.hxx"
#include "units.hxx"
#include "weatherrecord.hxx"
#include "weatherinterpretation.hxx"

#include "json.hpp"

#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>


//! Source for a file on disk
/*!
 *\tparam T Implements WeatherInterpretation interpret(WeatherRecord const & wr) const;
 */
template <class T>
class FileSystemWeatherSrc : public WeatherSource
{
    public:
        FileSystemWeatherSrc(std::string working_dir)
            : m_wd(std::move(working_dir))
        {}

        void configure(std::unordered_map<std::string, std::string> const & settings) {
            m_path = m_wd + "/" + settings.at("filesystemweathersrc_in");
        }


        Weather read() {
            if (isAvailable()) {
                // read from JSON file
                using json = nlohmann::json;
                std::ifstream json_file(m_path);
                json j;
                json_file >> j;
                auto wr = j.get<WeatherRecord>(); // JSON to WeatherRecord (calls from_json(nlohmann::json const &, WeatherRecord&))
                const T interpreter;
                WeatherInterpretation interpretation(interpreter.interpret(wr));
                return { std::move(interpretation), std::move(wr) };
            } else {
                throw std::runtime_error("FileSystemWeatherSrc not available");
            }
        }


        bool isAvailable() const {
            const char* path_cstr = m_path.c_str();
            const std::time_t unix_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct stat status;
            stat(path_cstr, &status);
            return access(path_cstr, R_OK) == 0 && // if file is readable
                    unix_now - status.st_mtime < 10 * 60; // if the file is less than 10 minutes old
        }


        void writeDefaultConfig(ConfigWriteContext & ctx) const {
            ctx.add("filesystemweathersrc_in", m_wd + "/fs_src_in");
        }


        virtual ~FileSystemWeatherSrc() {};
    private:
        const std::string m_wd;
        std::string m_path;
};

#endif // FILESYSTEMWEATHERSOURCE_HXX
