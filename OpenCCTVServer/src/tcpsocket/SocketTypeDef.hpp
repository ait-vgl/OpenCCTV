/*
 * SocketTypeDef.hpp
 *
 *  Created on: Feb 5, 2015
 *      Author: anjana
 */

#ifndef SOCKETTYPEDEF_HPP_
#define SOCKETTYPEDEF_HPP_

namespace tcpsocket {

using namespace std;

struct ReceivedDataStruct
{
	char* dataPtr;
	size_t length;
};

} /* namespace tcpsocket */



#endif /* SOCKETTYPEDEF_HPP_ */
