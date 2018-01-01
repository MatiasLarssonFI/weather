#ifndef WEATHERSERVERSETTINGS_HXX
#define WEATHERSERVERSETTINGS_HXX

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

#include <sys/stat.h>

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
                const std::string dir = m_wd + "/.mlweather";
                const std::string path = dir + "/" + m_conf_filename;
                std::ifstream conf_file(path);

                // if file doesn't exist, create default
                if (!conf_file) {
                    createDefaultConfig(dir, path);
                }
                conf_file.open(path);
                if (conf_file) {
                    // parse settings of format 'key: value'
                    t_settings settings;
                    std::string key, value;
                    while (std::getline(conf_file, key, ':')) {
                        if (!key.empty()) {
                            conf_file >> value;
                            conf_file.ignore(); // ignore rest of the line
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
            if (conf_file) {
                for (auto & src : m_sources) {
                    src->writeDefaultConfig(conf_file);
                }
            }
        }


        const std::string m_conf_filename;
        const std::vector<TpSrc> & m_sources;
        const std::string m_wd;
};

#endif // WEATHERSERVERSETTINGS_HXX
