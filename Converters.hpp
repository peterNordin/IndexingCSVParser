//!
//! @file   Converters.hpp
//! @author Peter Nordin
//! @date   2015-02-03
//!
//! @brief Contains the converter template speciallization implementations
//!

#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

namespace indcsvp
{

//! @brief The default converter template function
//! @details This function will allways fail, template speciallisations for each type are required
//! @tparam T The type that we want to interpret the contests of pBuffer as.
//! @param[in] pBuff The character buffer to convert
//! @param[out] rIsOK Reference to bool flag tellig you if parsing completed successfully
//! @returns Type default constructed value;
template <typename T>
T converter(const char* pBuff, bool &rIsOK)
{
    rIsOK = false;
    return T();
}

//! @brief The std::string converter speciallized template function
//! @param[in] pBuff The character buffer to convert
//! @param[out] rIsOK Reference to bool flag telling you if parsing completed successfully
//! @returns The contents of pBuff as a std::string
template<> inline
std::string converter<std::string>( const char* pBuff, bool &rIsOK)
{
    rIsOK = true;
    return std::string(pBuff);
}

template<> inline
double converter<double>( const char* pBuff, bool &rIsOK)
{
    char *pEnd;
    double d = strtod(pBuff, &pEnd);
    rIsOK = (*pEnd == '\0');
    return d;
}

template<> inline
unsigned long int converter<unsigned long int>( const char* pBuff, bool &rIsOK)
{
    char *pEnd;
    long int ul = strtoul(pBuff, &pEnd, 10); //!< @todo maybe support other bases then 10, see strtol documentation
    rIsOK = (*pEnd == '\0');
    return ul;
}

template<> inline
long int converter<long int>( const char* pBuff, bool &rIsOK)
{
    char *pEnd;
    long int i = strtol(pBuff, &pEnd, 10); //!< @todo maybe support other bases then 10, see strtol documentation
    rIsOK = (*pEnd == '\0');
    return i;
}

}


#endif // CONVERTERS_HPP

