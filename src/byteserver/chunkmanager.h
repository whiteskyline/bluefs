/*
 * chunkmanager.h
 *
 *	chunk管理的测试
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef CHUNKMANAGER_H_
#define CHUNKMANAGER_H_

#include <stdint.h>

#include <boost/thread/mutex.hpp>

#include <indexentry.h>
#include <chunkfile.h>
#include <environment.h>
#include <managerbase.h>

namespace com {
namespace personal {
namespace sfs {
namespace manager {

// 提供对于ChunkFile写入的权限，写入ChunkFile必须带有这个token
class ChunkFileWriteToken {
public:
	// 块文件标识
	uint32_t chunkSerialNo;
	uint32_t chunkOffset;

	// 文件写锁
	boost::mutex mutex;
	com::personal::sfs::file::ChunkFile file;

};

// 块文件管理
class ChunkManager: public ManagerBase {
public:
	ChunkManager(com::personal::sfs::core::Environment & env):ManagerBase(env) {

	}

	void init();

public:

	// 初始化所有的块文件数据
	void initBlocks() throw (SFS_NS_COMMON::SfsException);

	// 获得路径下所有某个类型的文件id
	void scanFiles(com::personal::sfs::file::FileType type, const std::string dir, std::vector<int> &idx);

	// 将文件索引添加到当前路径
	void appendIndex(/* out */com::personal::sfs::common::IndexEntry & entry) throw (SFS_NS_COMMON::SfsException);

	// 载入对应的idx文件的信息
	void loadIdxFile(int idx) throw (SFS_NS_COMMON::SfsException);

	// 获得对应索引文件的文件名称
	std::string getIdxFilename(int idx);

	const std::string &getExt(com::personal::sfs::file::FileType type) throw (SFS_NS_COMMON::SfsException);

protected:
	ChunkFileWriteToken &getCurrentWriteChunk(com::personal::sfs::file::FileType type) throw (SFS_NS_COMMON::SfsException);

private:
	ChunkFileWriteToken mIndexFileToken;
	ChunkFileWriteToken mDataFileToken;

};

}
}
}
} // end of namespaces

#endif /* CHUNKMANAGER_H_ */
