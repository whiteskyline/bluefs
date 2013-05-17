/*
 * managerbase.cpp
 *
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#include <environment.h>
#include <managerbase.h>

using namespace com::personal::sfs::core;
using namespace com::personal::sfs::manager;

ManagerBase::ManagerBase(Environment &env) {
	this->env = env;
}
