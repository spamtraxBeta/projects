
#include <boost/test/unit_test.hpp>
#include <MaEr/log/logHelper.hpp>

#include <iostream>

void fooFunc()
{
    DECLARE_LOGGER();

    logTrace()<<"3 This is a trace message";
    logInfo()<<"3 Hello, this is a info log";
    logWarning()<<"3 This is a warning";
}

BOOST_AUTO_TEST_CASE(logss)
{
    MaEr::LogHelper & logHelper = MaEr::LogHelper::instance();
    logHelper.addFileLogger("logFile.log");
    DECLARE_LOGGER();

    BOOST_LOG_TRIVIAL(error)<<"trivial error log";

    logTrace()<<"This is a trace message";
    logInfo()<<"Hello, this is a info log";
    logWarning()<<"This is a warning";

    fooFunc();

    {
        DECLARE_LOGGER();
        logTrace()<<"2 This is a trace message";
        logInfo()<<"2 Hello, this is a info log";
        logWarning()<<"2 This is a warning";
    }

}
