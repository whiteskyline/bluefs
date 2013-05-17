/*
 * =====================================================================================
 *
 *       Filename:  sfs_exception.h
 *
 *    Description:  建立文件系统的异常体系
 *
 *        Version:  1.0
 *        Created:  2013年05月09日 11时27分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lin Mingxing (ming.horizon), ming.horizon@gmail.com
 *        Company:  personal.com
 *
 * =====================================================================================
 */

#ifndef SFS_EXCEPTION_H
#define SFS_EXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

#include <gen-cpp/ErrorCode_constants.h>

namespace com
{
namespace personal
{
namespace sfs
{
namespace common
{

class SfsException: public std::exception
{
	public:
		SfsException(const int code, const std::string &verbose) throw():mCode(code), mVerbose(verbose)
		{
			std::ostringstream oss;
			oss << "Error: " << mCode << " "<< mVerbose;
			mWhat = oss.str(); 
		}

		~SfsException() throw()
		{
			
		}
		virtual const char* what() const throw()
		{
			return mWhat.c_str();
		}

		int getCode()
		{
			return mCode;
		}

		const std::string & getVerbose()
		{
			return mVerbose;
		}

	private:
		int mCode;
		std::string mVerbose;
		std::string mWhat;

};

}
}
}
}	// End Of Namespace com::personal::sfs

#endif

