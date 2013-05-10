#
# ErrorCode.thrift
# 定义程序中所有可以使用的错误码的集合
#

namespace cpp com.personal.sfs.common
namespace java com.personal.sfs.common

# 错误码定义
struct Error {
	1:i32 seq,
	2:string title,
	3:string message
}

# 错误码集合定义
enum ErrorCode {
	
	Success = 0,
	
	# 文件IO的错误定义
	OpenFileError = -10001,
	ChunkFileSpaceNotEnough = -10002,
	InvalidFileOffset = -10003,
	CreateDirectoryError = -10004,

	# 参数错误等等
	InvalidArgu = -20001	

}
