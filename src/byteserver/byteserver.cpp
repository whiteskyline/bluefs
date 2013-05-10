/*
 * =====================================================================================
 *
 *       Filename:  byteserver.cpp
 *
 *    Description:  数据服务器的完整入口
 *
 *        Version:  1.0
 *        Created:  2013年05月03日 09时51分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lin Mingxing (ming.horizon), ming.horizon@gmail.com
 *        Company:  personal.com
 *
 * =====================================================================================
 */

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>
using namespace boost::system;
using namespace boost::filesystem;

int main()
{
		//		path p("/tmp");
		//		p /= "x";
		//		std::string filename = "a.jpg";
		//		p.append(filename.begin(), filename.end());
		//		std::cout << p << std::endl;
		//		return 1;

		boost::scoped_ptr<int> i(new int);
		*i = 2;
		std::cout << *i << std::endl;
		return 0; 

}
