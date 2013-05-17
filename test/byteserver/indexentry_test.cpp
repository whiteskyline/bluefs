/*
 * indexentrytest.cpp
 *
 *	对于indexentry的写入进行校验
 *  Created on: May 11, 2013
 *      Author: ianlin
 */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <glog/logging.h>
#include <gtest/gtest.h>

class IndexEntryTest:public ::testing::Test {
protected:
	IndexEntryTest() {
		// set up work
	}

	virtual ~IndexEntryTest() {
		// clean work
	}

	virtual void SetUp() {

	}

	virtual void TearDown() {

	}
};

// 写入index文件的测试
TEST_F(IndexEntryTest, WriteIndexFileTest)
{

}
