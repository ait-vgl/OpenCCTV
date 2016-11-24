/*
 * AnalyticInstanceStreamGateway.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "AnalyticInstanceStreamGateway.hpp"

namespace opencctv {
namespace db {

//"SELECT ainsts.id, ainsts.analytic_instance_id, ainps.name, a.filename FROM analytic_instance_streams AS ainsts, analytic_input_streams AS ainps, analytics AS a WHERE (ainsts.stream_id = ?) AND (ainsts.analytic_input_stream_id = ainps.id ) AND (ainps.analytic_id = a.id)";

      // for start all ai.status = 2
const std::string AnalyticInstanceStreamGateway::_SELECT_ANALYTIC_INSTANCE_STREAM_SQL = "SELECT ainsts.id, ainsts.analytic_instance_id, ainps.name, a.filename " //, ainsts.stream_id "
"FROM analytic_instance_streams AS ainsts, analytic_input_streams AS ainps, analytics AS a, analytic_instances As ai "
"WHERE (ainsts.stream_id = ?) AND (ai.status = 2) "
"AND (ainsts.analytic_input_stream_id = ainps.id ) "
"AND (ainps.analytic_id = a.id) "
"AND (ai.id = ainsts.analytic_instance_id); ";

AnalyticInstanceStreamGateway::AnalyticInstanceStreamGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
        _pStatementPtr = NULL;
		

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticInstanceStreamGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}
}

AnalyticInstanceStreamGateway::~AnalyticInstanceStreamGateway() {
	(*_pStatementPtr).close();
	delete _pStatementPtr;  _pStatementPtr = NULL;
	delete _pDbConnPtr; _pDbConnPtr = NULL;
}

void AnalyticInstanceStreamGateway::findAllForStream(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS)
{
	try
	{
        if (_pStatementPtr) {
            delete _pStatementPtr;
            _pStatementPtr = NULL;
        }
        
        _pStatementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_ANALYTIC_INSTANCE_STREAM_SQL);
		(*_pStatementPtr).setInt(1, iStreamId);
		sql::ResultSet* pResultsPtr = (*_pStatementPtr).executeQuery();

		opencctv::dto::AnalyticInstanceStream ais;
		while((*pResultsPtr).next())
		{
			ais.setId((*pResultsPtr).getInt("id"));
			ais.setAnalyticInstanceId((*pResultsPtr).getInt("analytic_instance_id"));
			ais.setInputName((*pResultsPtr).getString("name"));
			ais.setAnalyticFilename((*pResultsPtr).getString("filename"));
			ais.setAnalyticDirLocation((*pResultsPtr).getString("filename"));
			//ais.setStreamId((*pResultsPtr).getInt("stream_id"));

			vToStoreAIS.push_back(ais);
			//ainsts.id, ainsts.analytic_instance_id, ainsts.analytic_input_stream_id, ainps.name, a.filename
		}
		(*pResultsPtr).close();
		delete pResultsPtr;
	}
	catch(sql::SQLException &e)
	{
		//std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
		std::string sErrorMsg = "AnalyticInstanceStreamGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//TODO :: Add to log
	}
}

void AnalyticInstanceStreamGateway::findAllForStreamByAnalyticInstanceId(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS, unsigned int iAnalyticInstanceId)
{
    const std::string query = "SELECT ainsts.id, ainsts.analytic_instance_id, ainps.name, a.filename " //, ainsts.stream_id "
"FROM analytic_instance_streams AS ainsts, analytic_input_streams AS ainps, analytics AS a, analytic_instances As ai "
"WHERE (ainsts.stream_id = ?) AND (ai.id = ?) "
"AND (ainsts.analytic_input_stream_id = ainps.id ) "
"AND (ainps.analytic_id = a.id) "
"AND (ai.id = ainsts.analytic_instance_id); ";
	try
	{
        if (_pStatementPtr) {
            delete _pStatementPtr;
            _pStatementPtr = NULL;
        }
        _pStatementPtr = (*_pDbConnPtr).prepareStatement(query);
		(*_pStatementPtr).setInt(1, iStreamId);
        (*_pStatementPtr).setInt(2, iAnalyticInstanceId);
		sql::ResultSet* pResultsPtr = (*_pStatementPtr).executeQuery();

		opencctv::dto::AnalyticInstanceStream ais;
		while((*pResultsPtr).next())
		{
			ais.setId((*pResultsPtr).getInt("id"));
			ais.setAnalyticInstanceId((*pResultsPtr).getInt("analytic_instance_id"));
			ais.setInputName((*pResultsPtr).getString("name"));
			ais.setAnalyticFilename((*pResultsPtr).getString("filename"));
			ais.setAnalyticDirLocation((*pResultsPtr).getString("filename"));
			//ais.setStreamId((*pResultsPtr).getInt("stream_id"));

			vToStoreAIS.push_back(ais);
			//ainsts.id, ainsts.analytic_instance_id, ainsts.analytic_input_stream_id, ainps.name, a.filename
		}
		(*pResultsPtr).close();
		delete pResultsPtr;
	}
	catch(sql::SQLException &e)
	{
		//std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
		std::string sErrorMsg = "AnalyticInstanceStreamGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//TODO :: Add to log
	}
}

} /* namespace db */
} /* namespace opencctv */


