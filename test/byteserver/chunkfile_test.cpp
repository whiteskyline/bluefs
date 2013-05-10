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

#include <glog/logging.h>
#include <gtest/gtest.h>

#include <chunkfile.h>

using namespace com::personal::file;

class ChunkFileTest: public ::testing::Test
{

		public:
			const static int ChunkFileContentTestBufferSize = 16 * 1024; 	// 16K测试缓冲区大小
		
		protected:
				ChunkFileTest()
				{  
						// set up work
				}

				virtual ~ChunkFileTest()
				{
						// clean work
				}

				virtual void SetUp()
				{  

				}

				virtual void TearDown()
				{  

				}  
};

/*
 * 测试创建文件是否正常
 */
TEST_F(ChunkFileTest, CreateFile)
{

	// 首先删除原来的文件
	if (access("/tmp/a", R_OK) == 0)
	{
		remove("/tmp/a/b/1.idx");
		rmdir("/tmp/a/b");
		rmdir("/tmp/a");
	}

	ChunkFile file;
	int ret = file.open(std::string("/tmp/a/b/1.idx"), DataChunkFileType, ChunkFileReadOnly);
	EXPECT_EQ(0, ret);

	// 测试写入的文件内容是否都正确
	int fd = ::open("/tmp/a/b/1.idx", O_RDWR);
	EXPECT_LT(0, fd);

	char buffer[16 * 1024];
	
	int readCount = 0;
	int currentReadResult = 0;
	bool success = true;
	while ((currentReadResult = read(fd, buffer, 16 * 1024)) > 0)
	{
		readCount += currentReadResult;
		int idx = 0;
		while (idx < currentReadResult)
		{
			if (buffer[idx] != 0)
			{
				success = false;
			}
			idx++;
		}
	}
	EXPECT_EQ(true, success);
	EXPECT_EQ(100 * 1024 * 1024, readCount);

}

/*
 * 测试文件写入是否正确，并且正常
 */
TEST_F(ChunkFileTest, WriteFileTest)
{
	
	ChunkFile file;
	int ret = file.open(std::string("/tmp/a/b/1.idx"), DataChunkFileType, ChunkFileAppendOnly);
	EXPECT_EQ(0, ret);

	char buffer[] = "abcdefghijkl";
	int writeSize = sizeof(buffer) - 1;

	EXPECT_EQ(writeSize, file.write(buffer, writeSize));

	// 持续写入99个abcdefghijkl
	int count = 99;
	while (count > 0)
	{
		file.write(buffer, writeSize);
		count--;
	}
	
	EXPECT_EQ(true, file.flush());

	// 验证写入的内容是否正确
	
	LOG(INFO) << "current file read pointer is:" << file.tellg();

	count = 0;
	bool everReadSuccess = false;
	int totalReadCount = 0;
	int currentReadCount = 0;
	while (count < 100) {
		if (( currentReadCount = file.read(count * 12, buffer, writeSize)) > 0)
		{
			// 检查最终的结果
			everReadSuccess = true;
			if (strncmp("abcdefghijkl", buffer, 12) != 0)
			{
				LOG(ERROR) << "string compare failed!" << buffer;
				everReadSuccess = false;
				break;
			}
			totalReadCount += currentReadCount;
			EXPECT_EQ(12, currentReadCount);
		}
		count++;
	}

	EXPECT_EQ(true, everReadSuccess);
	EXPECT_EQ(1200, totalReadCount);

}

/*
 * 对于写入的各种错误情况的测试
 */
TEST_F(ChunkFileTest, WriteNegativeTest)
{
	ChunkFile file;
	file.open("/tmp/a/b/1.idx", DataChunkFileType, ChunkFileReadOnly);

	char buffer[] = "abcd";

	EXPECT_EQ(-1, file.write(buffer, -1));
	EXPECT_EQ(-1, file.write(NULL, 4));
	EXPECT_EQ(-1, file.write(-1, buffer, 4));
	EXPECT_EQ(-1, file.write(0, buffer, -1));
	EXPECT_EQ(-1, file.write(0, NULL, 4));

}
