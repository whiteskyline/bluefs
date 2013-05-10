/*
 * =====================================================================================
 *
 *       Filename:  chunkfile.cpp
 *
 *    Description:  实现chunkfile接口内容
 *
 *        Version:  1.0
 *        Created:  2013年05月01日 00时29分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lin Mingxing (ming.horizon), ming.horizon@gmail.com
 *        Company:  personal.com
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstring>

#include <boost/filesystem.hpp>

#include <glog/logging.h>

#include <chunkfile.h>
#include <common/sfs_common.h>
#include <gen-cpp/ErrorCode_constants.h>
#include <gen-cpp/ErrorCode_types.h>

using namespace std;

using namespace boost::system;
using namespace boost::filesystem;

using namespace com::personal::sfs::common;
using namespace com::personal::file;

int ChunkFile::open(const std::string &filename, const FileType& fileType, const FileState& fileState)throw (SFS_NS_COMMON::SfsException)
{

		if (filename.empty()) {
				LOG(ERROR) << "file name is empty";
				SfsUtil::throw_except(ErrorCode::InvalidArgu, "filename empty");
		}

		if (fileType == InvalidChunkFileType) {
				LOG(ERROR) << "cannot open with invalid chunk file type";
				return -1;
		}

		if (fileState == ChunkFileClosed) {
				LOG(ERROR) << "cannot open with chunk file closed state";
				return -1;
		}

		if (state != ChunkFileClosed) {
				LOG(ERROR) << "chunk file is opened when it never be closed";
				return -1;
		}

		try {
				path filepath(filename);
				if (!exists(filepath.parent_path()))
				{
						if (!create_directories(filepath.parent_path()))
						{
								LOG(ERROR) << "cannot create parent_path for " << filename << ", please check disk space or permissions";
								return -1;
						}

				}

				switch (fileType)
				{
						case DataChunkFileType:
								fileSize = ChunkDataFileSize;
								break;
						case IndexChunkFileType:
								fileSize = ChunkIndexFileSize;
								break;
						default:
								LOG(ERROR) << "cannot create file with invalid file type";
								return -1;
				}

				if (!exists(filepath))
				{

						if (initFile(filename, fileSize, true) < 0)
						{
								LOG(ERROR) << "create file failed:" << filename;
								return -1;
						}
				}

				// 打开文件流
				state = fileState;
				ios_base::openmode mode;
				switch (state)
				{
						case ChunkFileReadOnly:
								mode = ios_base::in | ios_base::binary;
								break;
						case ChunkFileAppendOnly:
								mode = ios_base::in | ios_base::out | ios_base::binary;
								break;
						default:
								LOG(ERROR) << "invalid open mode";
								break;
				}

				stream.exceptions(ios_base::badbit);
				stream.open(filename.c_str(), mode);
		} catch (filesystem_error & e) {
				LOG(ERROR) << "boost::filesystem::filesystem_error catched! " << e.what();
		} catch (ios_base::failure & e) {
				LOG(ERROR) << "ios_base::failure catched! " << e.what();
		}

		return 0;

}

int32_t ChunkFile::read(char buffer[], int count)
{

		if (buffer == NULL)
		{
				LOG(ERROR) << "invalid buffer is null";
		}

		if (count <= 0 || count > fileSize)
		{
				LOG(ERROR) << "invalid count :" << count;
		}

		try {
				int toReadCount = count;
				int currentReadCount = 0;
				while (toReadCount > 0)
				{
						stream.read(buffer + (count  - toReadCount), toReadCount);
						currentReadCount = stream.gcount();
						if (currentReadCount > 0)
						{
								toReadCount -= currentReadCount;
						}
				}
				return (count - toReadCount);
		} catch (ios_base::failure & e) {
				LOG(ERROR) << "ios_base::failure catched! " << e.what();
				return -1;
		}
}

int32_t ChunkFile::read(int offset, char buffer[], int count)
{
		if (offset < 0 || offset + count >= fileSize || count <= 0) {
				LOG(ERROR) << "invalid offset" << offset << ", filesize is " << fileSize << ", count is " << count;
				return ErrorCode::InvalidFileOffset;
		}

		try {
				if (tellg() != offset)
				{
						stream.seekg(offset, std::ios_base::beg);
						DLOG(INFO) << "file get pointer is reset to:" << tellg();
				}

				return read(buffer, count);
		} catch (ios_base::failure & e) {
				LOG(ERROR) << "ios_base::failure catched! " << e.what();
				return -1;
		}

}

int32_t ChunkFile::write(char buffer[], int count)
{

		if (buffer == NULL || count <=0)
		{
			LOG(ERROR) << "invalid argument buffer:" << buffer << " count:" << count;
			return -1;
		}		

		try {
				int availPos = availableCapacity();
				if (availPos < count)
				{
						LOG(ERROR) << "cannot write to:" << filename << " cause file space not enough, " << availPos << " only" ;
						return ErrorCode::ChunkFileSpaceNotEnough;
				}

				stream.write(buffer, count);
				return count;
		} catch (ios_base::failure & e) {
				LOG(ERROR) << "write to file error: " << e.what();
				return -1; 
		}
}

int32_t ChunkFile::write(int offset, char buffer[], int count)
{
		
		if (buffer == NULL)
		{
			LOG(ERROR) << "null buffer";
			return -1;
		}

		if (offset < 0 || offset + count >= fileSize || count <= 0)
		{
				LOG(ERROR) << "invalid write offset:" << offset << ", filesize is:" << fileSize; 
				return -1;
		}

		try {
				if (tellp() != offset)
				{
						stream.seekp(offset, std::ios_base::beg);
						if (!stream)
						{
								LOG(ERROR) << "failured to seekp " << filename;
								return -1;
						}
				}

				stream.write(buffer, count);
				return count;
		} catch (ios_base::failure & e) {
				LOG(ERROR) << "ios_base::failure catched! " << e.what();
				return -1;
		}
}

bool ChunkFile::flush()
{
		if (state == ChunkFileClosed)
		{
				LOG(ERROR) << "chunk file is closed. no flush is allowed.";
				return false;
		}

		try {
				stream.flush();
				return true;
		} catch (ios_base::failure & e) {
				LOG(ERROR) << "cannot flush to file: " << e.what();
				return false;
		}
}

int ChunkFile::availableCapacity()
{
		return fileSize - tellp();
}

int64_t ChunkFile::tellp()
{
		return (uint64_t)stream.tellp();
}

int64_t ChunkFile::tellg()
{
		return (uint64_t)stream.tellg();
}

int ChunkFile::initFile(const std::string& filename, int allocateSize, bool fillZero)
{
		path p(filename);
		if (!exists(p))
		{
				// 对应的目录不存在
				if (!exists(p.parent_path())) {
						if (!create_directories(p.parent_path()))
						{
								LOG(ERROR) << "cannot create directories for:" << filename;
								return ErrorCode::CreateDirectoryError;
						}
				}
		}

		bool success = true;
		int fd = ::open(p.c_str(), O_RDWR | O_CREAT, 0644);

		if (fd < 0) {
				LOG(ERROR) << "cannot not open file, fd is " << fd;
				success = false;
		}

		if (success) {
				if (posix_fallocate(fd, 0, allocateSize) < 0)
				{
						LOG(ERROR) << "fallocate file space failed, check disk space"; 
						success = false;
				}
				else
				{
						::lseek(fd, 0, SEEK_SET);
				}
		}

		if (success && fillZero) {
				// 将整个文件写成0
				int remainSize = allocateSize;
				void* buffer = calloc(1, ChunkFileWriteBufferSize);

				if (buffer) {
						while (remainSize) {
								int currentWriteSize = ChunkFileWriteBufferSize;
								if (remainSize < currentWriteSize) {
										currentWriteSize = remainSize;
								}

								if (::write(fd, buffer, currentWriteSize) < 0) {
										LOG(ERROR) << "write zeros to file failed.";
										success = false;
										break;
								}

								remainSize -= currentWriteSize;
						}
						free(buffer);
				}
				else {
						success = false;
				}

		}

		if (!success) {
				return -1;
		}
		else
		{
			return 0;
		}

}
