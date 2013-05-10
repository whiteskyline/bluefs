/*
 * =====================================================================================
 *
 *       Filename:  sfs_utils.h
 *
 *    Description:  对于大部分的common文件的包含，都在这里进行，并提供统一的抛出
 *
 *        Version:  1.0
 *        Created:  2013年05月09日 22时34分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lin Mingxing (ming.horizon), ming.horizon@gmail.com
 *        Company:  personal.com
 *
 * =====================================================================================
 */


#ifndef SFS_UTILS_H
#define SFS_UTILS_H

#include <string>

#include <common/sfs_common.h>
#include <common/sfs_exception.h>

#include <gen-cpp/ErrorCode_types.h>

namespace com
{
namespace personal
{
namespace sfs
{
namespace common
{

class SfsUtil
{
public:
	static void throw_except(const int errorCode, std::string &what) throw (SFS_NS_COMMON::SfsException)
	{
		throw SFS_NS_COMMON::SfsException(errorCode, what);
	}

	static void throw_except(const int errorCode, const char *what) throw (SFS_NS_COMMON::SfsException)
	{
		throw SFS_NS_COMMON::SfsException(errorCode, std::string(what));
	}
	
};

}
}
}
}	// end of namespaces

#endif 
