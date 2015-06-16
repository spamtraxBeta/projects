#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE urlencode
#include <boost/test/unit_test.hpp>
#include <MaEr/urlencode/urlencode.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(urlencode_noUnicode)
{
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("ä"), "%C3%A4");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("ö"), "%C3%B6");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("ü"), "%C3%BC");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("Ä"), "%C3%84");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("Ö"), "%C3%96");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("Ü"), "%C3%9C");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("ß"), "%C3%9F");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode("&"), "%26");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode(""), "");
    BOOST_CHECK_EQUAL(MaEr::UrlEncode(""), "");
}

BOOST_AUTO_TEST_CASE(urlencode_unicode)
{
    BOOST_REQUIRE(MaEr::UrlEncode(L"ä") == L"%C3%A4");
    BOOST_REQUIRE(MaEr::UrlEncode(L"ö") == L"%C3%B6");
    BOOST_REQUIRE(MaEr::UrlEncode(L"ü") == L"%C3%BC");
    BOOST_REQUIRE(MaEr::UrlEncode(L"Ä") == L"%C3%84");
    BOOST_REQUIRE(MaEr::UrlEncode(L"Ö") == L"%C3%96");
    BOOST_REQUIRE(MaEr::UrlEncode(L"Ü") == L"%C3%9C");
    BOOST_REQUIRE(MaEr::UrlEncode(L"ß") == L"%C3%9F");
    BOOST_REQUIRE(MaEr::UrlEncode(L"&") == L"%26");

}

