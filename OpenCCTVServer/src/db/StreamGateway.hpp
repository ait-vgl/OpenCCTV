/*
 * StreamGateway.hpp
 *
 *  Created on: Oct 27, 2014
 *      Author: lakindu
 */

#ifndef STREAMGATEWAY_HPP_
#define STREAMGATEWAY_HPP_

#include "DbConnector.hpp"
#include "Stream.hpp"
#include <vector>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

namespace db
{
class StreamGateway
{
private:
	Connection* _pDbConn;
	static const string _SELECT_STREAM_INFO_SQL;
public:
	StreamGateway();
	~StreamGateway();
	vector<Stream> findAll();
};
} /* namespace db */

#endif /* STREAMGATEWAY_HPP_ */
