#ifndef WEATHERSERVERSETTINGS_HXX
#define WEATHERSERVERSETTINGS_HXX

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

#include <sys/stat.h>

/*!
 * \tparam TpSrc weather source pointer type
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
         * \throw std::runtime_error
         */
        t_settings makeSettings() const {
            if (!m_wd.empty()) {
                const std::string dir = m_wd + "/.mlweather";
                const std::string path = dir + "/" + m_conf_filename;
                std::ifstream conf_file(path);
                if (!conf_file) {
                    createDefaultConfig(dir, path);
                }
                conf_file.open(path);
                if (conf_file) {
                    t_settings settings;
                    std::string key, value;
                    while (std::getline(conf_file, key, ':')) {
                        if (!key.empty()) {
                            std::getline(conf_file, value);
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
            ::mkdir(parent_dir.c_str(), S_IRWXU);
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
