/*
 * indexentry.cpp
 *
 *	对于文件入口的定义
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#include <boost/crc.hpp>

#include <common/sfs_common.h>
#include <gen-cpp/ErrorCode_constants.h>
#include <indexentry.h>

using namespace com::personal::sfs::common;

int IndexEntry::getLengthOfEntry(char type)
{
	switch((EntryType)type) {
	case FileAdd:
		return sizeof(FileAddIndexData);
	case FileUndoCommit:
		return sizeof(FileUndoCommitIndexData);
	default:
		LOG(ERROR) << "invalid entry type:" << (int)type;
		return 0;
	}
}

#define OFFSET(field, structure) (int)(&((structure *)0)->field)
void * IndexEntry::getOffsetOfEntry(char type)
{
	switch((EntryType)type) {
	case FileAdd:
		return (void*)&_data.addFile;
	case FileUndoCommit:
		return (void*)&_data.undoCommit;
	default:
		return (void*)&_data;
	}

}

uint16_t IndexEntry::crc()
{
	boost::crc_16_type result;

	result.process_bytes(&this->mHeader + 2, sizeof(IndexHeader) - 2);
	result.process_bytes(getOffsetOfEntry(this->mHeader.type), getLengthOfEntry(this->mHeader.type));

	return result.checksum();

}
