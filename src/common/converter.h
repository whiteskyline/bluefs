/*
 * converter.h
 *
 *  Created on: May 13, 2013
 *      Author: ianlin
 */

#ifndef CONVERTER_H
#define CONVERTER_H

#include <sys/types.h>
#include <sstream>

#include <common/sfs_common.h>
#include <common/sfs_utils.h>

namespace com
{
namespace personal
{
namespace sfs
{
namespace common
{

template<class T>
T convert(const std::string &str) throw (SFS_NS_COMMON::SfsException)
{
	T value;
	std::stringstream stream(str);
	stream >> value;

	if (!stream)
	{
		SfsUtil::throw_except(ErrorCode::InvalidArgu, "invalid string for convert");
	}

	return value;
}

class Converter
{
public:
	static uint32_t parseUint32(const std::string &str) throw (SFS_NS_COMMON::SfsException) {
		return convert<uint32_t>(str);
	}
private:
};

}
}
}
}	// end of namespaces

#endif
