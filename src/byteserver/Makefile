#
# 编译数据服务器的所有内容
# Lin Mingxing(ming.horizon@gmail.com)
#
#


TARGET = $(OUTPUT_PATH)/byteserver

include ../../default.mk

$(TARGET): $(LOCAL_OBJS)
	$(GCC) $< $(ALL_LIBS) -o $@

.PHONY: clean
clean:
	-rm *.o
