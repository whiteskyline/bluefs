/*
 * environment.h
 *
 *	全局的系统配置，各个模块之间用来互相协调的配置中枢
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <string>

namespace com {
namespace personal {
namespace sfs {
namespace core {

class Environment {

public:

	void setStorageDir(const std::string &storageDir) {
		m_StorageDir.clear();
		m_StorageDir.reserve(storageDir.length());
		m_StorageDir.assign(storageDir);
	}

	const std::string& getStorageDir() {
		return m_StorageDir;
	}

private:
	std::string m_StorageDir;

};

}
}
}
} // end of namespaces

#endif /* ENVIRONMENT_H_ */
