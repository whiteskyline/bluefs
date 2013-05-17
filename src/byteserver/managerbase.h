/*
 * managerbase.h
 *
 *	定义
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#ifndef MANAGERBASE_H_
#define MANAGERBASE_H_

#include <environment.h>

namespace com {
namespace personal {
namespace sfs {
namespace manager {

class ManagerBase {
public:
	ManagerBase(com::personal::sfs::core::Environment & env);

public:
	// 初始化所有数据
	void init() {

	}
	com::personal::sfs::core::Environment & getEnv() {
		return env;
	}
private:
	com::personal::sfs::core::Environment env;
};

}
}
}
} // end of namespaces

#endif /* MANAGERBASE_H_ */
