/*
 * chunkmanager.cpp
 *
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/algorithm/string.hpp>

#include <glog/logging.h>

#include <common/sfs_common.h>
#include <common/converter.h>
#include <chunkfile.h>
#include <chunkmanager.h>

using namespace com::personal::sfs::common;
using namespace com::personal::sfs::file;
using namespace com::personal::sfs::manager;

// 初始化chunk文件
void ChunkManager::init() {

}

// 初始化所有的block文件
void ChunkManager::initBlocks() throw (SFS_NS_COMMON::SfsException) {
	const std::string & storageDir = getEnv().getStorageDir();

	if (storageDir.empty()) {
		LOG(ERROR) << "storage dir is empty!";
		SfsUtil::throw_except(ErrorCode::InvalidEnvConfig,"storage dir is empty");
	}

	boost::filesystem::path path(storageDir);
	if (!exists(path)) {
		LOG(ERROR) << "storage dir " << storageDir << " does not exists.";
		SfsUtil::throw_except(ErrorCode::InvalidEnvConfig, "storage dir does not exists");
	}

	std::vector<int> ids;
	scanFiles(IndexChunkFileType, storageDir, ids);

	int curIdxId = 1;
	if (!ids.empty())
	{
		// 空目录，执行chunkfile初始化
		std::sort(ids.begin(), ids.end());
		curIdxId = ids[ids.size() - 1];
	}

	// 初始化idx文件的信息
	loadIdxFile(curIdxId);
}

// 读取正确的文件id列表
void ChunkManager::scanFiles(FileType type, const std::string dir,
		std::vector<int> &idx) {
	const std::string& ext = getExt(type);
	int extLen = ext.length();

	DLOG(INFO) << "loading " << ext << " files from dir:" << dir;

	boost::filesystem::directory_iterator end_itr;
	boost::filesystem::directory_iterator cur_itr(dir);

	std::string filename;

	while (cur_itr != end_itr) {

		if (boost::filesystem::is_regular_file(cur_itr->status())) {

			filename = cur_itr->path().filename().string();

			if (boost::iends_with(filename, ext)) {
				DLOG(INFO) << "find idx file:" << filename;
				filename = filename.substr(0, filename.length() - extLen);
				try {
					int id = Converter::parseUint32(filename);
					idx.push_back(id);
				} catch (SFS_NS_COMMON::SfsException &e) {
					LOG(WARNING) << "idx filename is not int:" << filename;
					continue;
				}
			}
		}
		else
		{
			LOG(WARNING) << "find an directory " << filename << "in directory:" << dir;
		}

		cur_itr++;
	}

}

std::string ChunkManager::getIdxFilename(int idx) {
	std::stringstream stream;
	stream << getEnv().getStorageDir() << "/" << idx << getExt(IndexChunkFileType);
	return stream.str();
}

// 将某个idx文件中的索引信息载入
void ChunkManager::loadIdxFile(int idx) throw (SFS_NS_COMMON::SfsException) {
	std::string target = getIdxFilename(idx);

	// 避免在更换当前的IndexFile的时候发生文件写入的操作
	boost::mutex::scoped_lock(mIndexFileToken.mutex);

		if (!mIndexFileToken.file.isClosed()) {
			mIndexFileToken.file.close();
		}

		if (mIndexFileToken.file.open(target, IndexChunkFileType, ChunkFileAppendOnly) < 0) {
			// 打开文件失败
			LOG(ERROR) << "failed open idx file:" << target;
			SfsUtil::throw_except(ErrorCode::OpenFileError, "open file error");
		} else {
			LOG(INFO) << "open idx file:" << target << " success";
			mIndexFileToken.chunkOffset = 0;
			mIndexFileToken.chunkSerialNo = idx;
		}

	}

// 获取某个文件类型的扩展名
const std::string& ChunkManager::getExt(FileType type) throw (SFS_NS_COMMON::SfsException)
{
	switch(type)
	{
		case DataChunkFileType:
		return DAT_FILE_EXTENSION;
		case IndexChunkFileType:
		return IDX_FILE_EXTENSION;
		default:
		LOG(ERROR) << "invalid file type:" << (int)type;
		SfsUtil::throw_except(ErrorCode::InvalidArgu, "invalid file type");
	}
	return DAT_FILE_EXTENSION;
}

// 在文件当中添加index信息
void ChunkManager::appendIndex(struct IndexEntry& entry) throw (SFS_NS_COMMON::SfsException) {
	ChunkFileWriteToken & token = getCurrentWriteChunk(
			IndexChunkFileType);
	boost::mutex::scoped_lock lock(token.mutex);
	// 获得crc数据
	entry.mHeader.length = entry.getLengthOfEntry(entry.mHeader.type);
	if (entry.mHeader.length == 0) {
		SfsUtil::throw_except(ErrorCode::InvalidArgu, "Invalid Entry Type");
	}

	entry.mHeader.crc = entry.crc();
	DLOG(INFO) << "write crc:" << entry.mHeader.crc;

	// 开始往内部写入数据
	token.file.write((char *)&entry.mHeader, sizeof(IndexHeader));
	token.file.write((char *)entry.getOffsetOfEntry(entry.mHeader.type), entry.mHeader.length);

}

ChunkFileWriteToken& ChunkManager::getCurrentWriteChunk(FileType type)
		throw (SFS_NS_COMMON::SfsException) {
			switch (type) {
				case IndexChunkFileType:
				return mIndexFileToken;
				case DataChunkFileType:
				return mDataFileToken;
				default:
				LOG(ERROR) << "invalid file type:" << (int)type;
				SfsUtil::throw_except(ErrorCode::InvalidArgu, "invalid file type");
			}
			return mDataFileToken;
		}
