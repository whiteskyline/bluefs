/*
 * indexfileiterator.h
 *
 *	定义文件迭代的工具类，保证能够进行正确的读取
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef INDEXFILEITERATOR_H_
#define INDEXFILEITERATOR_H_

#include <boost/iterator/iterator_facade.hpp>
#include <boost/shared_ptr.hpp>

#include <common/sfs_common.h>
#include <chunkfile.h>
#include <indexentry.h>

namespace com {
namespace personal {
namespace sfs {
namespace file {

class IndexFileIterator: public boost::iterator_facade<IndexFileIterator,
		const com::personal::sfs::common::IndexEntry, boost::single_pass_traversal_tag> {
public:
	IndexFileIterator() :appendOffset(0), countBadEntries(0) {

	}
	IndexFileIterator(const std::string &filename) throw (SFS_NS_COMMON::SfsException);

private:
	void increment();
	bool equal(const IndexFileIterator & iter)const {
		return (this->filePtr.get() == iter.filePtr.get());
	}

	boost::iterator_facade<IndexFileIterator,
			const com::personal::sfs::common::IndexEntry, boost::single_pass_traversal_tag>::reference dereference() const{
		BOOST_ASSERT(filePtr.get() != NULL);
		return currentEntry;
	}

	bool isEnd(const IndexHeader &header) {
		return ( 0 == header.crc
				&& 0 == header.length
				&& 0 == header.timestamp
				&& 0 == header.type);
	}

private:
	uint32_t appendOffset;
	uint32_t countBadEntries;
	std::auto_ptr<com::personal::sfs::file::ChunkFile> filePtr;
	std::string filename;
	com::personal::sfs::common::IndexEntry currentEntry;

	friend class boost::iterator_core_access;

};

}
}
}
}

#endif /* INDEXFILEITERATOR_H_ */
