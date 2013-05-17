/*
 * indexentry.h
 *
 *	索引的入口
 *
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef INDEXENTRY_H_
#define INDEXENTRY_H_

#include <sys/types.h>

#include <ostream>
#include <string>
#include <sstream>

#include <common/sfs_common.h>

namespace com {
namespace personal {
namespace sfs {
namespace common {

enum EntryType {
	FileAdd, FileUndoCommit
};

// 索引头部信息
struct IndexHeader
{
	uint16_t crc;		// index header剩余数据和data数据区的crc校验码
	char type;		// 数据的类型
	uint16_t length;		// data数据区的长度
	uint32_t timestamp;		// 数据产生时间，in seccond
};

// 添加文件的数据信息
struct FileAddIndexData
{
	VirtualFolderKey folderKey;
	VirtualFileKey fileKey;

	uint32_t chunkId;
	uint32_t offset;
	uint32_t length;
};

// 撤销commit的数据信息
struct FileUndoCommitIndexData
{
	VirtualFolderKey folderKey;
	VirtualFileKey fileKey;

	uint32_t chunkId;
	uint32_t offset;
	uint32_t length;
};

// 可打印的添加文件的数据信息
struct FileAddMessage: public FileAddIndexData
{
public:
	inline std::string to_string()const {
		std::ostringstream stream;
		stream << "add file:" << folderKey.to_string() << "/" << fileKey.to_string() << " add to chunkid:" \
				<< chunkId << " offset:" << offset << " length:" << length;
		return stream.str();
	}
};

struct FileUndoCommitMessage: public FileAddIndexData
{
public:
	inline std::string to_string()const {
		std::ostringstream stream;
		stream << "undocommit file:" << folderKey.to_string() << "/" << fileKey.to_string() << " add to chunkid:" \
				<< chunkId << " offset:" << offset << " length:" << length;
		return stream.str();
	}
};

union IndexEntryData {
	struct FileAddMessage addFile;
	struct FileUndoCommitMessage undoCommit;
};

// indexfile的入口
struct IndexEntry
{
public:
	IndexHeader mHeader;
	union IndexEntryData _data;

public:
	// 获得入口的数据长度
	static int getLengthOfEntry(char type);

	// 获得入口的数据偏移
	void * getOffsetOfEntry(char type);

	// 获得数据的crc校验内容, 校验的内容包括header(除开crc的部分，以及)
	uint16_t crc();
};

}
}
}
} // end of namespaces

#endif /* INDEXENTRY_H_ */
