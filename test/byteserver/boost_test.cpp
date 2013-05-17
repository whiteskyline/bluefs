/*
 * boost_test.cpp
 * 对于boost库内容的实验场
 *  Created on: May 13, 2013
 *      Author: ianlin
 */

#include <string>
#include <sstream>
#include <exception>

#include <gtest/gtest.h>
#include <glog/logging.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

template<class T>
T convert(std::string str)
{
	T value;
	std::stringstream stream(str);
	stream >> value;

	if (!stream)
	{
		throw new std::exception();
	}

	return value;
}

struct sta
{
	int a;
	int b;
	int c;
};

union stb
{
	int a;
	double b;
	float c;
	struct sta d;
};

class BoostTest: public ::testing::Test
{

};
#define OFFSET(field, structure) (void *)(&((structure *)0)->field)
TEST_F(BoostTest, OffsetTest)
{
	LOG(INFO) << "offset" << OFFSET(b, struct sta);
	LOG(INFO) << "offset" << OFFSET(d, union stb);
	//LOG(INFO) << "offset" << &((struct sta *)0)->b;
}

TEST_F(BoostTest, DirectoryIteratorTest)
{
	std::string str("/tmp");
	std::string ext(".idx");

	int extLen = ext.length();

	boost::filesystem::directory_iterator end_itr;
	boost::filesystem::directory_iterator cur_itr(str);

	std::string filename;

	while (cur_itr != end_itr)
	{
		if (boost::filesystem::is_regular_file(cur_itr->status()))
		{
			if (boost::iends_with(cur_itr->path().filename().string(), ext))
			{
				DLOG(INFO) << "idx file:" << cur_itr->path().filename() << " is found.";
				filename = cur_itr->path().filename().string();
				filename = filename.substr(0, filename.length() - extLen);

				DLOG(INFO) << "id:" << convert<int>(filename);
			}
		}
		else
		{
			LOG(WARNING) << "find dir:" << cur_itr->path() \
					<< " in " << str;
		}
		cur_itr++;
	}
}
