/*
 * UrlEncode.hpp
 *
 *  Created on: 24.12.2013
 *      Author: martin
 */

#ifndef URLENCODE_HPP_
#define URLENCODE_HPP_

#include <string>

namespace MaEr
{

    /**
     * Based on: http://stackoverflow.com/questions/3589936/c-urlencode-library-unicode-capable
     */
    std::string UrlEncode(const std::string & url);
    std::wstring UrlEncode(const std::wstring & url);

} /* namespace MaEr */

#endif /* URLENCODE_HPP_ */
