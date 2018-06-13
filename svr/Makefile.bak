###########################################
CC = g++
CPPFLAGS = -std=c++11 -Wall -g3
INCFLAGS = $(addprefix -I,$(INCLUDE_DIRS))
LDDIR_FLAGS = $(addprefix -L,$(LD_DIRS))
LDFLAGS = $(addprefix -l,$(LIBRARYS))
TARGET := ameison

###########################################
INCLUDE_DIRS = . ./common/ext/test
SRC_DIRS = . ./common/ext/test
LD_DIRS =
LIBRARYS = boost_system boost_thread boost_serialization
BUILD_DIR = ./build/

##########################################
ABS_INCDIRS := $(abspath $(INCLUDE_DIRS))
ABS_SRCDIRS := $(abspath $(SRC_DIRS))
ABS_LDDIRS := $(abspath $(LD_DIRS))
ABS_BUILDDIR := $(abspath $(BUILD_DIR))

SRCS := $(foreach dir,$(ABS_SRCDIRS),$(wildcard $(dir)/*.cpp))
OBJS := $(addprefix $(BUILD_DIR),$(notdir $(SRCS:.cpp=.o)))

###########################################

.PHONY : GenRule
GenRule:
%.d : %.cpp
	set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    echo -e "\t"$(CC) $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS) $(LDDIR_FLAGS) -c $(<) -o $(@:.d=.o) >> $@; \
	rm -f $@.$$$$

#sinclude $(SRCS:.cpp=.d)
sinclude $(OBJS:.o=.d)

echo :
	@echo $(OBJS)
	@echo $(SRCS)
	@echo $(ABS_SRCDIRS)

.PHONY : $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS) $(LDDIR_FLAGS) $(OBJS) -o $(TARGET)

.PHONY : clean
clean:
	rm -rf $(ABS_BUILDDIR)/*
