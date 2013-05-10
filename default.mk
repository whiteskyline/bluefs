#
# 编译过程中的所有值定义
#
#
#

.SUFFIXES: .cpp .d

# 编译工具
GCC = g++

# 输入输出位置定义
OUTPUT_PATH = ../../objs

# 编译过程的值定义
DEFS += -DHAVE_NETINET_IN_H

# 编译过程需要的头文件目录
INCLUDES += -I./ \
			-I../ \
			-I../../deps/glog/src

CPPFLAGS += -Wall -Wextra -Wno-unused-parameter -g -ggdb -O2 $(INCLUDES) $(DEFS)

# 编译过程的lib库查询目录
LDFLAGS += -L../../deps/boost/stage \
			-L../../deps/glog/.libs

# 编译过程的所有以来的公共lib
LIBS += -lboost_filesystem \
		-lboost_system \
		-lglog

ALL_LIBS += $(LDFLAGS) $(LIBS)

SOURCES = $(wildcard *.cpp)

LOCAL_OBJS = $(SOURCES:.cpp=.o)

LOCAL_DEPS = $(SOURCES:.cpp=.d)

# 打包文件的定义
%.a: $(LOCAL_OBJS)
	ar rcu $@ $^

%.d: $.cpp
	@create $@
	$(CPP) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(LOCAL_DEPS)
