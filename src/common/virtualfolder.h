/*
 * virtualfolder.h
 *
 *	sfs 支持单级的虚拟目录，该目录仅仅作为一个标记存在
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef VIRTUALFOLDER_H_
#define VIRTUALFOLDER_H_

#include <string>
#include <string.h>
#include <stdint.h>

namespace com {
namespace personal {
namespace sfs {
namespace common {

const int FOLDERNAME_LENGTH = 12;

struct VirtualFolderKeyCore {
	char folderName[FOLDERNAME_LENGTH];
};

struct VirtualFolderKey: VirtualFolderKeyCore {
public:
	std::string to_string() const {
		char buffer[FOLDERNAME_LENGTH + 1];
		strncpy(buffer, folderName, FOLDERNAME_LENGTH);
		buffer[FOLDERNAME_LENGTH] = 0;
		return buffer;
	}
};

class VirtualFolder {
private:
	std::string folderName;
};

}
}
}
} // end of namespaces

#endif /* VIRTUALFOLDER_H_ */
