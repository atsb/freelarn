#pragma once
/**
 * \file console_css.h
 * \date 2017-09-05
 * \date 2017-10-27
 * \version 1.0.0
 * \author Yong Xiaoran
 * \brief A read-only config file reader with type conversion, inspired and modified from s-imran/config
 * \example
 * \code
int main()
{
    ConfigReader::instance()->load("1.config");
    std::cout << ConfigReader::instance()->read<string>("username");
}
//use with auto
    auto reader = ConfigReader::instance();
    reader->load("1.config");
    reader->read<double>("price");
    std::cout << reader->size() << std::endl;
 * \endcode
*/

#include <map>
#include <string>
#include <string_view>
#include <fstream>
#include <cassert>
#include <type_traits>
#include <algorithm>

using std::string;
using std::string_view;

/**
 * \date 2017-10-27
 * \class ConfigReader
 * \brief This is an instance class of ConfigReader
*/
class ConfigReader
{

public:
    /**
    * \brief get the instance (or if not exist, create one) of the ConfigReader class
    * \return ConfigRead* static pointer
    */
    static ConfigReader* instance();

    /** \brief load config from file.
     * \param filename
     * \remarks config files are key value pairs seperated by ':';
     * \warning throw runtime_error if cannot open file.
     */
    void load(const string & filename)
    {
        std::ifstream fs(filename);
        if(!fs.is_open())
            throw std::runtime_error("ConfigReader::read cannot open file \""+ filename + "\"");
        while(!fs.eof())
        {
            string buffer;
            std::getline(fs, buffer);
            if(!buffer.empty())
            {
                const size_t delim_pos = buffer.find(':');
                if(delim_pos!=string::npos)
                {
                    const string & key = buffer.substr(0, delim_pos);
                    string value(buffer.substr(delim_pos+1));
                    const size_t comment_pos(value.find("//"));
                    if(comment_pos!=string::npos)
                        value = value.substr(0, comment_pos);
                    value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                    _data.insert(std::make_pair(std::move(key), value));
                }
            }
        }
    }

    /** \brief read a value by a key (template function)
     * \param key
     * \return value(T type)
     * \example read<string>("name"); read<int>("phone_number")
     * \warning throw invalid_argument if un-supported type detected.
     */
    template<typename T>
    T read(const string & key) const
    {
        throw std::invalid_argument("ConfigReader::read<T>() un-supported type, template specialization not found");
    }

    /** \brief returns the size of the buffered data.
     *  \return size_t num of buffered configuration k-v pairs.
     */
    size_t size() const
    {
        return _data.size();
    }

public:
    //constructor & destructor
    ConfigReader(const ConfigReader&) = delete;
    ConfigReader & operator=(const ConfigReader&) = delete;
    ConfigReader && operator=(const ConfigReader&&) = delete;

private:
    static ConfigReader* _instance;
    ConfigReader():_data(){;}
    std::map<const string, string> _data;
};

inline ConfigReader* ConfigReader::_instance = nullptr;

inline ConfigReader* ConfigReader::instance()
{
    if(!_instance)
        _instance = new ConfigReader();
    return _instance;
}

/** \brief template specialization for std::string_view
 * \param key
 * \return converted string_view
 */
template<>
string_view ConfigReader::read<string_view>(const string & key) const
{
    return _data.at(key);
}

/** \brief template specialization for std::string
 * \param key
 * \return converted string
 */
template<>
string ConfigReader::read<string>(const string & key) const
{
    return _data.at(key);
}

/** \brief template specialization for double
 * \param key
 * \return converted double
 */
template<>
double ConfigReader::read<double>(const string & key) const
{
    return std::stod(_data.at(key));
}

/** \brief template specialization for float
 * \param key
 * \return converted float
 */
template<>
float ConfigReader::read<float>(const string & key) const
{
    return std::stof(_data.at(key));
}

/** \brief template specialization for int
 * \param key
 * \return converted int
 */
template<>
int ConfigReader::read<int>(const string & key) const
{
    return std::stoi(_data.at(key));
}

/** \brief template specialization for long
 * \param key
 * \return converted long
 */
template<>
long ConfigReader::read<long>(const string & key) const
{
    return std::stol(_data.at(key));
}

/** \brief template specialization for long double
 * \param key
 * \return converted long double
 */
template<>
long double ConfigReader::read<long double>(const string & key) const
{
    return std::stold(_data.at(key));
}

/** \brief template specialization for long long
 * \param key
 * \return converted long long
 */
template<>
long long ConfigReader::read<long long>(const string & key) const
{
    return std::stoll(_data.at(key));
}

/** \brief template specialization for unsigned int
 * \param key
 * \return converted unsigned int
 */
template<>
unsigned int ConfigReader::read<unsigned int>(const string & key) const
{
    return std::stoul(_data.at(key));
}

/** \brief template specialization for unsigned long
 * \param key
 * \return converted unsigned long
 */
template<>
unsigned long ConfigReader::read<unsigned long>(const string & key) const
{
    return std::stoul(_data.at(key));
}

/** \brief template specialization for unsigned long long
 * \param key
 * \return converted unsigned long long
 */
template<>
unsigned long long ConfigReader::read<unsigned long long>(const string & key) const
{
    return std::stoull(_data.at(key));
}

