#
# 测试环境下的编译选项赋值
# 林明星(ming.horizon@gmail.com)
# 
#

INCLUDES += -I../../deps/gtest/include \
				 -I../../src

LDFLAGS += -L../../deps/gtest

LIBS += -lgtest

ALL_LIBS += $(LIBS) -lglog

LTOOL = libtool --mode=link g++ -g -O

# 所有的测试的编译选项
LTOOLFLAGS += ../../deps/gtest/lib/libgtest_main.la -lgtest
