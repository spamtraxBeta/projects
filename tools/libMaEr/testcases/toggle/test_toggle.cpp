//#define BOOST_TEST_DYN_LINK
//#define BOOST_TEST_MODULE toggle
#include <boost/test/unit_test.hpp>
#include <MaEr/toggle/toggle.hpp>

#include <iostream>
#include <deque>

BOOST_AUTO_TEST_SUITE(toggle)

class ToggleReceiver
{
public:
    ToggleReceiver()
    {

    }

    void onToggle(MaEr::CToggle::tenLevel level, int duration)
    {
        toggleResult tr(level, duration);
        m_result.push_back(tr);
    }

    void reset()
    {
        m_result.clear();
    }

    struct toggleResult
    {
        toggleResult(MaEr::CToggle::tenLevel l, int d):
            level(l)
          , duration(d)
        {

        }

        toggleResult(const toggleResult & rhs):
            level(rhs.level)
          , duration(rhs.duration)
        {

        }

        bool operator == (const toggleResult & rhs)const
        {
            bool result = this->level == rhs.level
                    &&
                    this->duration == rhs.duration;
            return result;
        }

        const MaEr::CToggle::tenLevel level;
        const int duration;

    private:



        // no assignment!
        toggleResult & operator=(const toggleResult & rhs);
    };

    typedef std::deque<toggleResult> list_t;
    const list_t & result()const
    {
        return m_result;
    }
private:
    list_t m_result;
};


std::ostream& operator<<(std::ostream& os, const ToggleReceiver::toggleResult & tr)
{
    os<<"toggleResult(level="<<tr.level<<", duration="<<tr.duration;
    return os;
}

BOOST_AUTO_TEST_CASE(toggle)
{

    boost::asio::io_service ioService;

    MaEr::CToggle toggle(ioService);

    ToggleReceiver toggleReceiver;
    toggle.toggle.connect
    (
        boost::bind
        (
            &ToggleReceiver::onToggle,
            boost::ref(toggleReceiver),
            _1, _2
        )
    );

    {
        int duration = 100;
        toggle.add(duration, "-_");
        const ToggleReceiver::list_t & result = toggleReceiver.result();
        BOOST_REQUIRE_EQUAL(result.size(), 2);
        BOOST_REQUIRE_EQUAL(result.at(0), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_hi, duration));
        BOOST_REQUIRE_EQUAL(result.at(1), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_lo, duration));
        toggleReceiver.reset();
    }

    {
        int duration = 10;
        toggle.add(duration, "-2_5---____");
        const ToggleReceiver::list_t & result = toggleReceiver.result();
        BOOST_REQUIRE_EQUAL(result.size(), 4);
        BOOST_REQUIRE_EQUAL(result.at(0), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_hi, duration * 2));
        BOOST_REQUIRE_EQUAL(result.at(1), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_lo, duration * 5));
        BOOST_REQUIRE_EQUAL(result.at(2), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_hi, duration * 3));
        BOOST_REQUIRE_EQUAL(result.at(3), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_lo, duration * 4));
        toggleReceiver.reset();
    }

    {
        int duration = 10;
        toggle.add(duration, "____-2_5--2-");
        const ToggleReceiver::list_t & result = toggleReceiver.result();
        BOOST_REQUIRE_EQUAL(result.size(), 4);
        BOOST_REQUIRE_EQUAL(result.at(0), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_lo, duration * 4));
        BOOST_REQUIRE_EQUAL(result.at(1), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_hi, duration * 2));
        BOOST_REQUIRE_EQUAL(result.at(2), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_lo, duration * 5));
        BOOST_REQUIRE_EQUAL(result.at(3), ToggleReceiver::toggleResult(MaEr::CToggle::nenLevel_hi, duration * 4));
        toggleReceiver.reset();
    }


}

BOOST_AUTO_TEST_SUITE_END() // toggle


