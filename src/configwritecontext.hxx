#ifndef CONFIGWRITECONTEXT_HXX
#define CONFIGWRITECONTEXT_HXX

#include <ostream>
#include <string>


//! Implements config writing (to a stream).
class ConfigWriteContext
{
    public:
        ConfigWriteContext(std::ostream &);


        /*!
         * Adds a setting.
         */
        void add(std::string key, std::string value);
    private:
        std::ostream & m_stream;
};

#endif // CONFIGWRITECONTEXT_HXX
