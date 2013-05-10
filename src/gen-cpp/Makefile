#
# 定义libsfsthrift.a文件的编译规则
# 林明星(ming.horizon@gmail.com)
# 
#

TARGET = $(OUTPUT_PATH)/libsfsthrift.a

include ../../default.mk

$(TARGET): libsfsthrift.os
	ar rv $@ `find *.o`

libsfsthrift.os: $(wildcard ../../thrift/*.thrift)
	thrift -out ./ --gen cpp $<
	$(GCC) -c $(CPPFLAGS) `find *.cpp`

.PHONY: clean
clean:
	-rm ./*.cpp ./*.h ./*.o
	-rm $(TARGET)
