#
# 对于chunkfile的测试过程
# 林明星(ming.horizon@gmail.com)
# 
#

# 导入所有测试环境默认编译变量

include ../../default.mk
include ../test.mk

DEP_OBJS += $(filter-out ../../src/byteserver/byteserver.o, $(wildcard ../../src/byteserver/*.o))

# 测试需要的默认的选项
INCLUDES += -I../../src/byteserver

TARGET = $(OUTPUT_PATH)/test_datanode

$(TARGET): $(LOCAL_OBJS)
	echo $(LOCAL_OBJS)
	$(LTOOL) -o $@ $(LOCAL_OBJS) $(DEP_OBJS) $(LTOOLFLAGS) $(LDFLAGS) $(CPPFLAGS) $(ALL_LIBS)

#boost_test: boost_test.o
#	$(LTOOL) -o $@ $< $(LTOOLFLAGS) $(LDFLAGS) $(CPPFLAGS) $(ALL_LIBS)

.PHONY: clean
clean:
	-rm *.o 
	-rm $(TARGET) 
