/*
 * virtualfile.h
 *
 *	虚拟文件内容的描述
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef VIRTUALFILE_H_
#define VIRTUALFILE_H_

#include <string>
#include <sstream>
#include <string.h>

namespace com
{
namespace personal
{
namespace sfs
{
namespace common
{

// 文件名称的长度
const int FILENAME_LENGTH = 12;

struct VirtualFileKeyCore {
	char filename[FILENAME_LENGTH];
};

struct VirtualFileKey:public VirtualFileKeyCore {
public:
	std::string to_string()const
	{
		char buffer[FILENAME_LENGTH + 1];
		strncpy(buffer, filename, FILENAME_LENGTH);
		buffer[FILENAME_LENGTH] = 0;
		return buffer;
	}
};

class VirtualFile
{

};

}
}
}
}	// end of namespaces

#endif /* VIRTUALFILE_H_ */
