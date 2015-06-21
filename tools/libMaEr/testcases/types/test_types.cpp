#include <boost/test/unit_test.hpp>

#include <MaEr/types/byte_t.hpp>

#include <stdexcept>
#include <iostream>
#include <deque>
#include <bitset>


//std::ostream& operator<<(std::ostream& os, const ToggleReceiver::toggleResult & tr)
//{
//    os<<"toggleResult(level="<<tr.level<<", duration="<<tr.duration;
//    return os;
//}


BOOST_AUTO_TEST_SUITE(types_byte)

// test input values in range 0..255
BOOST_AUTO_TEST_CASE(types_byte_intValue_LE_255)
{
    // test values 0..255
    for (int i = 0; i <= 0xFF; ++i)
    {
        std::stringstream ss;
        ss<<i;

        MaEr::byte_t b(0);
        ss>>b;

        BOOST_REQUIRE_EQUAL(b.value(), i);
    }
}

// test binary input value
BOOST_AUTO_TEST_CASE(types_byte_binary)
{
    // test values 0..255
    for (int i = 0; i <= 0xFF; ++i)
    {
        std::bitset<8> x(i);

        std::stringstream ss;
        ss<<x;

        MaEr::byte_t b(0);
        ss>>b;

        BOOST_REQUIRE_EQUAL(b.value(), i);
    }
}


void test_types_byte_binary_fail(const std::string & input)
{
    std::stringstream ss;
    ss<<input;

    try
    {
        MaEr::byte_t b(0);
        ss>>b;
        std::stringstream errorss;
        errorss<<"Reached unexpected code line; input = "<<input;
        BOOST_ERROR(errorss.str());
    }
    catch (std::exception & ex)
    {
        std::string str;
        ss>>str;

        BOOST_REQUIRE_EQUAL(str, input);
    }
    catch (...)
    {
        std::stringstream errorss;
        errorss<<"Caught unexpected exception; input = "<<input;
        BOOST_ERROR(errorss.str());
    }
}

// test invalid binary input
BOOST_AUTO_TEST_CASE(types_byte_binary_fail)
{
    test_types_byte_binary_fail("0000000x");
    test_types_byte_binary_fail("00i00000");
    test_types_byte_binary_fail("x0i01100");
    test_types_byte_binary_fail("xx");
}

// test input values > 255
BOOST_AUTO_TEST_CASE(types_byte_intValueg_GT_255)
{
    // test values 256..1000
    for (int i = 256; i <= 1000000; i += 123)
    {
        std::stringstream ss;
        ss<<i;

        MaEr::byte_t b(0);

        try
        {
            ss>>b;

            std::stringstream errorss;
            errorss<<"Reached unexpected code line; i = "<<i;
            BOOST_ERROR(errorss.str());
        }
        catch (std::out_of_range & ex)
        {
            std::string str;
            ss>>str;

            std::stringstream referenceSS;
            referenceSS<<i;

            std::string reference;
            referenceSS>>reference;

            BOOST_REQUIRE_EQUAL(str, reference);
        }
        catch (...)
        {
            BOOST_ERROR("Caught unexpected exception");
        }
    }
}

BOOST_AUTO_TEST_SUITE_END() // types


