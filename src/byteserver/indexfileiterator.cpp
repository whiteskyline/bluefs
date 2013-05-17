/*
 * indexfileiterator.cpp
 *
 *  定义index文件的迭代，保证能够顺利地从已经有的indexfile中读取出正确的内容
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#include <string>
#include <memory>

#include <chunkfile.h>
#include <indexentry.h>
#include <indexfileiterator.h>

using namespace com::personal::sfs::file;
using namespace com::personal::sfs::file;
// 创建blockfile的内容
IndexFileIterator::IndexFileIterator(const std::string &filename) throw (SFS_NS_COMMON::SfsException)
{
	filePtr.reset(new ChunkFile());
	filePtr->open(filename, IndexChunkFileType, ChunkFileReadOnly);
}

// 自增所以数据文件
void IndexFileIterator::increment()
{
	if (filePtr.get() == NULL)
	{
		return;
	}

	int readCount = filePtr->read((char *)&currentEntry.mHeader, sizeof(IndexHeader));
	if (readCount != sizeof(IndexHeader)) {
		LOG(ERROR) << "cannot read enough coung as header: read:" << readCount << "; expect:" << sizeof(IndexHeader);
		countBadEntries++;
		return;
	}
	DLOG(INFO) << "read " << readCount << " for head";

	if (isEnd(currentEntry.mHeader)) {
		// 到达文件尾部
		DLOG(INFO) << "end of file reached";
		filePtr.reset(NULL);
		return;
	}

	readCount = filePtr->read((char *)currentEntry.getOffsetOfEntry(currentEntry.mHeader.type),
			currentEntry.getLengthOfEntry(currentEntry.mHeader.type));

	if (readCount != currentEntry.getLengthOfEntry(currentEntry.mHeader.type)) {
		LOG(ERROR) << "cannot read enough count as data: read:" << readCount << "; expect:" << currentEntry.getLengthOfEntry(currentEntry.mHeader.type);
		return;
	}
	DLOG(INFO) << "read " << readCount << " for content";

	//检验校验和是否正确
	if (currentEntry.crc() != currentEntry.mHeader.crc) {
		LOG(ERROR) << "wrong crs checksum; read:" << currentEntry.mHeader.crc << " expect:" << currentEntry.crc();
	}

}
