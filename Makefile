#
# Makefile
# Computer Networking Programing Assignments
#
#  Created by Phillip Romig on 4/3/12.
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
CXXFLAGS = -g  -std=c++11 -DBOOST_LOG_DYN_LINK
LDFLAGS = -g 
#CXXFLAGS = -g -pthread -std=c++11 -DBOOST_LOG_DYN_LINK
#LDFLAGS = -g -pthread

#
# You should be able to add object files here without changing anything else
#
TARGET = main
OBJ_FILES = ${TARGET}.o  Proc.o Thread.o Simulation.o Burst.o Event.o
INC_FILES = ${TARGET}.hpp Proc.hpp Thread.hpp Simulation.hpp Burst.hpp Event.hpp

#
# Any libraries we might need.
#
LIBRARYS = -lbsd -lboost_regex -lboost_log_setup -lboost_log  -lboost_thread -lboost_system -lpthread -lpcap


${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}

%.o : %.cpp ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core ${TARGET} ${OBJ_FILES}

#
# This might work to create the submission tarball in the formal I asked for.
#
submit:
	rm -f core ${TARGET} ${OBJ_FILES}
	mkdir `whoami`
	cp Makefile README.txt *.hpp *.cpp `whoami`
	tar zcf `whoami`.tgz `whoami`
