#
# 编译数据服务器的所有内容
# Lin Mingxing(ming.horizon@gmail.com)
#
#


TARGET = $(OUTPUT_PATH)/libsfscommon.a

include ../../default.mk

.DEFAULT: $(TARGET)
$(TARGET): $(LOCAL_OBJS)

.PHONY: clean
clean:
	-rm *.o
	-rm $(TARGET)
