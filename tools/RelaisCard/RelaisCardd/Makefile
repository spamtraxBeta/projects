SOURCES = CommandLine.cpp \
RelaisCard.cpp \
RelaisCardd.cpp \
RelaisCardInitializer.cpp \
TimeoutReader.cpp \
../Common/byte_t.cpp \
../Common/InterprocessComm.cpp \
../Common/log.cpp \
../Common/RelaisError.cpp \
../Common/RelaisFrame.cpp \


CC  = /usr/bin/g++
CFLAGS = -std=c++11 -Wall -g -I/home/martin/opt/lib/boost_1_55_0/include -DBOOST_LOG_DYN_LINK
LDFLAGS = -L/home/martin/opt/lib/boost_1_55_0/lib  \
-lboost_log_setup \
-lboost_thread \
-lboost_program_options \
-lboost_system \
-lboost_log \
-lboost_filesystem \
-lboost_chrono \
-lboost_date_time


DEPENDFILE = .depend
EXECUTABLE = RelaisCardd

OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: clean

all: $(SOURCES) $(EXECUTABLE)
	

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

dep: $(SOURCES)
	$(CC) $(CFLAGS) -MM $(SOURCES) > $(DEPENDFILE)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPENDFILE)
