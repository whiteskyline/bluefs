/*
 * =====================================================================================
 *
 *       Filename:  chunkfile.h
 *
 *    Description:  定义块文件的操作接口
 *    			1)该类的所有方法都不使用同步过程，因此需要对于需要同步的外部请求，需要自行添加必要的锁
 *
 *        Version:  1.0
 *        Created:  2013年04月29日 17时33分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lin Mingxing (ming.horizon), ming.horizon@gmail.com
 *        Company:  personal.com
 *
 * =====================================================================================
 */

#ifndef CHUNKFILE_H
#define CHUNKFILE_H

#include <stdint.h>
#include <fstream>

#include <common/sfs_utils.h>

namespace com
{
namespace personal
{
namespace file
{

using namespace com::personal::sfs::common;

/*
 * 数据文件状态
 * ChunkFileClosed:文件已关闭
 * ChunkFileReadOnly:文件只读
 * ChunkFileAppendOnly:文件只可写
 */
enum FileState {
	ChunkFileClosed, ChunkFileReadOnly, ChunkFileAppendOnly
};

/*
 * 数据文件类型
 * InvalidChunkFileType: 不可用的文件类型
 * DataChunkFileType: 数据文件
 * IndexChunkFileType: 索引数据文件类型
 */
enum FileType {
	InvalidChunkFileType, DataChunkFileType, IndexChunkFileType
};

class ChunkFile {

	public:
		const static int ChunkDataFileSize = 100 * 1024 * 1024;		// 100MB默认空间大小
		const static int ChunkIndexFileSize = 100 * 1024 * 1024;	// Index文件100MB默认空间大小
		const static int ChunkFileWriteBufferSize = 16 * 1024; 		// 16K写入缓冲区

	public:
		/*
 		 * 在创建的时候指定文件的类型，当关闭之后文件是closed状态，并且该对象将不可用
 		 *
 		 */
		ChunkFile(): state(ChunkFileClosed), type(InvalidChunkFileType)
		{
			// nothing to do
		}

	public:
		/*
 		 * 打开相应的文件
 		 */
		int open(const std::string& filename, const FileType& fileType, const FileState& fileState)throw (SFS_NS_COMMON::SfsException);

		/*
		 * 从当前位置读取对应的内容到缓冲区当中
 		 */
		int32_t read(char buffer[], int count);
		int32_t read(int offset, char buffer[], int count);

		/*
 		 * 将缓冲区内的内容写到文件的对应位置
 		 */
		int32_t write(char buffer[], int count);
		int32_t write(int offset, char buffer[], int count);

		/*
 		 * 对于缓冲区进行刷新 
 		 */
		bool flush();

		/*
 		 * 查询对应的chunk文件的剩余容量
 		 */
		int availableCapacity();

		/*
 		 * 获取文件输入的指针位置
 		 */ 
		int64_t tellp();

		/*
 		 * 获取文件的读取指针位置
 		 */
		int64_t tellg();

	private:
		/*
 		 * 初始化文件内容
 		 * 创建文件，并且初始化文件的所有内容为0
 		 */	
		int initFile(const std::string& filename, int allocateSize, bool fillZero);

	private:
		enum FileState state;
		enum FileType type;
		std::fstream stream;
		std::string filename;
		int fileSize;
};

}
}
}

#endif
