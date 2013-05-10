#
# sfs文件系统项目编译配置
# Lin Mingxing(ming.horizon@gmail.com)
#
.PHONY: all test clean
all:
	$(MAKE) -C src/gen-cpp
	$(MAKE) -C src/common
	$(MAKE) -C src/byteserver

test: all
	$(MAKE) -C test/byteserver

clean:
	$(MAKE) -C src/gen-cpp clean
	$(MAKE) -C src/common clean
	$(MAKE) -C src/byteserver clean
	-rm -rf objs/*
