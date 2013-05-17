/*
 * =====================================================================================
 *
 *       Filename:  chunkfile_test.cpp
 *
 *    Description:  针对所有的块文件操作进行测试
 *
 *
 *        Version:  1.0
 *        Created:  2013年05月04日 09时12分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lin Mingxing (ming.horizon), ming.horizon@gmail.com
 *        Company:  personal.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#include <glog/logging.h>
#include <gtest/gtest.h>

#include <common/sfs_common.h>
#include <indexfileiterator.h>
#include <chunkfile.h>
#include <environment.h>
#include <chunkmanager.h>

using namespace com::personal::sfs::file;
using namespace com::personal::sfs::manager;
using namespace com::personal::sfs::core;

class ChunkFileTest: public ::testing::Test {

public:
	const static int ChunkFileContentTestBufferSize = 16 * 1024; // 16K测试缓冲区大小

protected:
	ChunkFileTest() {
		// set up work
	}

	virtual ~ChunkFileTest() {
		// clean work
	}

	virtual void SetUp() {

	}

	virtual void TearDown() {

	}
};

/*
 * 测试创建文件是否正常
 */TEST_F(ChunkFileTest, CreateFile)
{

	// 首先删除原来的文件
	if (access("/tmp/a", R_OK) == 0) {
		remove("/tmp/a/b/1.idx");
		rmdir("/tmp/a/b");
		rmdir("/tmp/a");
	}

	ChunkFile file;
	int ret = file.open(std::string("/tmp/a/b/1.idx"), DataChunkFileType,
			ChunkFileReadOnly);
	EXPECT_EQ(0, ret)
		;

	// 测试写入的文件内容是否都正确
	int fd = ::open("/tmp/a/b/1.idx", O_RDWR);
	EXPECT_LT(0, fd)
		;

	char buffer[16 * 1024];

	int readCount = 0;
	int currentReadResult = 0;
	bool success = true;
	while ((currentReadResult = read(fd, buffer, 16 * 1024)) > 0) {
		readCount += currentReadResult;
		int idx = 0;
		while (idx < currentReadResult) {
			if (buffer[idx] != 0) {
				success = false;
			}
			idx++;
		}
	}
	EXPECT_EQ(true, success)
		;
	EXPECT_EQ(100 * 1024 * 1024, readCount)
		;

}

/*
 * 测试文件写入是否正确，并且正常
 */TEST_F(ChunkFileTest, WriteFileTest)
{

	ChunkFile file;
	int ret = file.open(std::string("/tmp/a/b/1.idx"), DataChunkFileType,
			ChunkFileAppendOnly);
	EXPECT_EQ(0, ret)
		;

	char buffer[] = "abcdefghijkl";
	int writeSize = sizeof(buffer) - 1;

	EXPECT_EQ(writeSize, file.write(buffer, writeSize))
		;

	// 持续写入99个abcdefghijkl
	int count = 99;
	while (count > 0) {
		file.write(buffer, writeSize);
		count--;
	}

	EXPECT_EQ(true, file.flush())
		;

	// 验证写入的内容是否正确
	LOG(INFO) << "current file read pointer is:" << file.tellg();

	count = 0;
	bool everReadSuccess = false;
	int totalReadCount = 0;
	int currentReadCount = 0;
	while (count < 100) {
		if ((currentReadCount = file.read(count * 12, buffer, writeSize)) > 0) {
			// 检查最终的结果
			everReadSuccess = true;
			if (strncmp("abcdefghijkl", buffer, 12) != 0) {
				LOG(ERROR) << "string compare failed!" << buffer;
				everReadSuccess = false;
				break;
			}
			totalReadCount += currentReadCount;
			EXPECT_EQ(12, currentReadCount)
				;
		}
		count++;
	}

	EXPECT_EQ(true, everReadSuccess)
		;
	EXPECT_EQ(1200, totalReadCount)
		;

}

/*
 * 对于写入的各种错误情况的测试
 */TEST_F(ChunkFileTest, WriteNegativeTest)
{
	ChunkFile file;
	file.open("/tmp/a/b/1.idx", DataChunkFileType, ChunkFileReadOnly);

	char buffer[] = "abcd";

	EXPECT_THROW(file.write(buffer, -1), SFS_NS_COMMON::SfsException)
;		EXPECT_THROW(file.write(NULL, 4), SFS_NS_COMMON::SfsException);
		EXPECT_THROW(file.write(-1, buffer, 4), SFS_NS_COMMON::SfsException);
		EXPECT_THROW(file.write(0, buffer, -1), SFS_NS_COMMON::SfsException);
		EXPECT_THROW(file.write(0, NULL, 4), SFS_NS_COMMON::SfsException);

	}

int run_bash(char * const cmd, char *argus[]) {
	pid_t pid = fork();
	if (pid == 0) {
		// 子进程执行文件操作, 因为执行了exec，因此，后面的代码永远不会被运行到
		execvp(cmd, argus);
		return 0; // this line will never be reached.
	} else if (pid > 0) {
		// 父进行直接返回
		int retCode;
		pid_t rpid = wait(&retCode);
		if (rpid < 0) {
			LOG(ERROR) << "failed to wait for child thread";
			return -1;
		} else {
			LOG(INFO) << "finished running " << cmd;
			return 0;
		}
	} else {
		// 创建子进程出错
		LOG(ERROR) << "failed to create process";
		return -1;
	}
}

//TEST_F(ChunkFileTest, TestRunBash)
//{
//char *argu[] = {"-c", "touch", "/tmp/2.idx", NULL};
//EXPECT_EQ(0, run_bash("sh", argu));
//EXPECT_EQ(0, run_bash("sh", argu));
//	system("touch /tmp/1.idx");
//}

/*
 * 建立样本文件
 */
void recreateSampleFile() {
	int result = system("rm -f /tmp/1idx");
	result = result + 1;
	Environment env;
	env.setStorageDir("/tmp");

	ChunkManager manager(env);
	manager.initBlocks();

	struct IndexEntry entry;
	entry.mHeader.type = (char) (enum EntryType) FileAdd;
	entry._data.addFile.chunkId = 1;
	strcpy(entry._data.addFile.fileKey.filename, "abcdefghijk");
	strcpy(entry._data.addFile.folderKey.folderName, "fabcdefghij");
	entry._data.addFile.offset = 0x1;
	entry._data.addFile.length = 0x2;

	manager.appendIndex(entry);
	manager.appendIndex(entry);

}

/*
 * 测试chunkmanager是否能够正确扫描对应的文件
 */TEST_F(ChunkFileTest, ChunkManagerScanFileTest)
{

	// 在目录下创建文件
	recreateSampleFile();

}

/*
 * 测试使用迭代器进行索引文件读取
 */TEST_F(ChunkFileTest, IndexIteratorTest)
{
	recreateSampleFile();
	IndexFileIterator endIter;
	IndexFileIterator begIter("/tmp/1idx");

	while (++begIter != endIter) {
		IndexEntry & entry = const_cast<IndexEntry &>(*begIter);
		LOG(INFO) << "header length:" << (int)entry.mHeader.length;
		LOG(INFO) << "header type" << (int)entry.mHeader.type;
		LOG(INFO) << "header timestamp" << entry.mHeader.timestamp;
		LOG(INFO) << "content filename" << entry._data.addFile.fileKey.filename;
		LOG(INFO) << "content folder name" << entry._data.addFile.folderKey.folderName;
		LOG(INFO) << "content chunk id" << entry._data.addFile.chunkId;
		LOG(INFO) << "content length" << entry._data.addFile.length << " offset" << entry._data.addFile.offset;
	}

}
