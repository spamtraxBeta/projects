
#include <boost/test/unit_test.hpp>
#include <MaEr/log/logHelper.hpp>

#include <iostream>

void fooFunc()
{
    logDeclaration();

    BOOST_LOG_TRIVIAL(error)<<"trivial error log";

    logTrace()<<"3 This is a trace message";
    logInfo()<<"3 Hello, this is a info log";
    logWarning()<<"3 This is a warning";
}

BOOST_AUTO_TEST_CASE(logss)
{
    MaEr::LogHelper::init();
    MaEr::LogHelper::addFileLogger("logFile.log");
    logDeclaration();


    logTrace()<<"This is a trace message";
    logInfo()<<"Hello, this is a info log";
    logWarning()<<"This is a warning";

    fooFunc();

    {
        BOOST_LOG_NAMED_SCOPE("whateverScope");
        fooFunc();

        logTrace()<<"Other Trace";
        logInfo()<<"Other Info";
        logWarning()<<"Other Warning";
    }

}
