#ifndef WEATHERSERVERSETTINGS_HXX
#define WEATHERSERVERSETTINGS_HXX

#include "configwritecontext.hxx"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm>

#include <sys/stat.h>


//! Trim leading whitespaces
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}


/*!
 * \tparam TpSrc source pointer type
 */
template <class TpSrc>
class WeatherServerSettings
{
    public:
        using t_settings = std::unordered_map<std::string, std::string>;

        WeatherServerSettings(std::string conf_filename, std::vector<TpSrc> const & sources, std::string working_dir)
            : m_conf_filename(std::move(conf_filename))
            , m_sources(sources)
            , m_wd(std::move(working_dir))
        {}

        /*!
         * Returns a map of the settings.
         * \throw std::runtime_error
         */
        t_settings makeSettings() const {
            if (!m_wd.empty()) {
                // file path
                const std::string path = m_wd + "/" + m_conf_filename;
                std::ifstream conf_file(path);

                // if file doesn't exist, create default
                if (!conf_file) {
                    createDefaultConfig(m_wd, path);
                    conf_file.open(path);
                }

                if (conf_file) {
                    // parse settings of format 'key: value'
                    t_settings settings;
                    std::string key, value;
                    while (std::getline(conf_file, key, ':')) {
                        if (!key.empty()) {
                            std::getline(conf_file, value);
                            ltrim(value);
                            settings[key] = value;
                        }
                    }
                    return settings;
                } else {
                    throw std::runtime_error(path + ": failed to open config file for reading.");
                }
            } else {
                throw std::runtime_error("failed to get env var for home dir");
            }
        }
    private:
        /*!
         * Creates the default config file.
         */
        void createDefaultConfig(std::string const & parent_dir, std::string const & full_path) const {
            // create directory for config
            ::mkdir(parent_dir.c_str(), S_IRWXU);

            // write default config from sources
            std::ofstream conf_file(full_path);
            ConfigWriteContext ctx(conf_file);

            if (conf_file) {
                for (auto & src : m_sources) {
                    src->writeDefaultConfig(ctx);
                }
            }
        }


        const std::string m_conf_filename;
        const std::vector<TpSrc> & m_sources;
        const std::string m_wd;
};

#endif // WEATHERSERVERSETTINGS_HXX
