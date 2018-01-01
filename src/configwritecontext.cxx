#include "configwritecontext.hxx"

ConfigWriteContext::ConfigWriteContext(std::ostream & stream)
    : m_stream(stream)
{}


void ConfigWriteContext::add(std::string key, std::string value) {
    m_stream << key << ": " << value << "\n";
}
