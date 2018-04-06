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
            , global(readSettings())
            , m_settings_exist(!global.empty())
        {}

        /*!
         * Returns a map of the settings.
         *
         * Creates default settings if there are
         * no settings on disk yet.
         *
         * \throw std::runtime_error
         */
        t_settings makeSettings() const {
            // if settings file didn't exist, create default
            if (!m_settings_exist) {
                createDefaultConfig(m_wd, m_wd + "/" + m_conf_filename);
            }
            return readSettings();
        }
    private:
        /*!
         * Creates the default config file.
         */
        void createDefaultConfig(std::string const & parent_dir, std::string const & full_path) const {
            // create directory for config
            ::mkdir(parent_dir.c_str(), S_IRWXU);

            // write default config
            std::ofstream conf_file(full_path);
            if (conf_file) {
                ConfigWriteContext ctx(conf_file);
                ctx.add("interpretation", "nordic");
                for (auto const & src : m_sources) {
                    src->writeDefaultConfig(ctx);
                }
            }
        }


        //! Read settings from disk
        t_settings readSettings() const {
            if (!m_wd.empty()) {
                // file path
                const std::string path = m_wd + "/" + m_conf_filename;
                std::ifstream conf_file(path);

                if (conf_file) {
                    // parse settings of format 'key: value'
                    t_settings settings;
                    std::string key, value;
                    while (std::getline(conf_file, key, ':')) {
                        if (!key.empty() && key[0] != '#') { // # stands for comment
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


        const std::string m_conf_filename;
        const std::vector<TpSrc> & m_sources;
        const std::string m_wd;

    public:
        const t_settings global; //!< global (or pre-read) settings
    private:
        bool m_settings_exist; //!< if settings file was read successfully
};

#endif // WEATHERSERVERSETTINGS_HXX
