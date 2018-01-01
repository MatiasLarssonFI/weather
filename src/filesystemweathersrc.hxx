#ifndef FILESYSTEMWEATHERSOURCE_HXX
#define FILESYSTEMWEATHERSOURCE_HXX

#include "weather.hxx"
#include "weathersource.hxx"

#include <ostream>
#include <string>
#include <unordered_map>


//! Source for a file on disk
class FileSystemWeatherSrc : public WeatherSource
{
    public:
        FileSystemWeatherSrc(std::string working_dir);

        virtual void configure(std::unordered_map<std::string, std::string> const & settings);
        virtual Weather read();
        virtual bool isAvailable() const;
        virtual void writeDefaultConfig(std::ostream & stream) const;

        virtual ~FileSystemWeatherSrc() {};
    private:
        const std::string m_wd;
        std::string m_path;
};

#endif // FILESYSTEMWEATHERSOURCE_HXX
