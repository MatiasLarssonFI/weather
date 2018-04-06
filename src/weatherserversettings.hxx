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
            , m_settings(readSettings())
        {}


        /*!
         * Updates the settings map.
         *
         * Creates default settings if there are
         * no settings on disk yet.
         *
         * \throw std::runtime_error
         */
        void updateSettings() {
            // if settings file didn't exist or was empty, create default
            if (m_settings.empty()) {
                createDefaultConfig(m_wd, m_wd + "/" + m_conf_filename);
                m_settings = readSettings();
            }
        }


        //! Returns a map of currently read settings
        t_settings const & settings() const {
            return m_settings;
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
            // file path
            const std::string path = m_wd + "/" + m_conf_filename;
            std::ifstream conf_file(path);
            t_settings settings;

            if (conf_file) {
                // parse settings of format 'key: value'
                std::string key, value;
                while (std::getline(conf_file, key, ':')) {
                    if (!key.empty() && key[0] != '#') { // # stands for comment
                        std::getline(conf_file, value);
                        ltrim(value);
                        settings[key] = value;
                    }
                }
            }
            return settings;
        }


        const std::string m_conf_filename;
        const std::vector<TpSrc> & m_sources;
        const std::string m_wd;
        t_settings m_settings; //!< settings read from disk
};

#endif // WEATHERSERVERSETTINGS_HXX
