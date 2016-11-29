#include "AnalyticInstanceGateway.hpp"

namespace opencctv
{
namespace db
{

AnalyticInstanceGateway::AnalyticInstanceGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
		_pStatementPtr = NULL;
	}
	catch (sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticInstanceGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch (opencctv::Exception &e)
	{
		throw opencctv::Exception(e);
	}
}

AnalyticInstanceGateway::~AnalyticInstanceGateway()
{

	(*_pStatementPtr).close();
	delete _pStatementPtr;
	_pStatementPtr = NULL;
	delete _pDbConnPtr;
	_pDbConnPtr = NULL;
}


void AnalyticInstanceGateway::findAll(std::vector<opencctv::dto::AnalyticInstance> &vToStoreAIS)
{
	const std::string QUERY = "SELECT ai.id as 'analytic_instance_id',  a.filename FROM  analytics AS a, analytic_instances As ai WHERE (ai.status = 2) AND (ai.analytic_id = a.id);";

	try
	{
		if (_pStatementPtr)
		{
			delete _pStatementPtr;
			_pStatementPtr = NULL;
		}

		_pStatementPtr = (*_pDbConnPtr).prepareStatement(QUERY);
		//(*_pStatementPtr).setInt(1, iStreamId);
		sql::ResultSet *pResultsPtr = (*_pStatementPtr).executeQuery();

		opencctv::dto::AnalyticInstance ai;

		while ((*pResultsPtr).next())
		{
			ai.setId((*pResultsPtr).getInt("analytic_instance_id"));
			ai.setAnalyticFilename((*pResultsPtr).getString("filename"));
			ai.setAnalyticDirLocation((*pResultsPtr).getString("filename"));
			vToStoreAIS.push_back(ai);
		}

		(*pResultsPtr).close();
		delete pResultsPtr;
	}
	catch (sql::SQLException &e)
	{
		//std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
		std::string sErrorMsg = "AnalyticInstanceGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//TODO :: Add to log
	}
}

void AnalyticInstanceGateway::findAnalyticInstance(std::vector<opencctv::dto::AnalyticInstance> &vToStoreAIS, unsigned int iAnalyticInstanceId)
{
	const std::string QUERY = "SELECT ai.id as 'analytic_instance_id',  a.filename FROM  analytics AS a, analytic_instances As ai WHERE (ai.status = ?) AND (ai.analytic_id = a.id);";

	try
	{
		if (_pStatementPtr)
		{
			delete _pStatementPtr;
			_pStatementPtr = NULL;
		}

		_pStatementPtr = (*_pDbConnPtr).prepareStatement(QUERY);
		(*_pStatementPtr).setInt(1, iAnalyticInstanceId);
		sql::ResultSet *pResultsPtr = (*_pStatementPtr).executeQuery();

		opencctv::dto::AnalyticInstance ai;

		while ((*pResultsPtr).next())
		{
			ai.setId((*pResultsPtr).getInt("analytic_instance_id"));
			ai.setAnalyticFilename((*pResultsPtr).getString("filename"));
			ai.setAnalyticDirLocation((*pResultsPtr).getString("filename"));
			vToStoreAIS.push_back(ai);
		}

		(*pResultsPtr).close();
		delete pResultsPtr;
	}
	catch (sql::SQLException &e)
	{
		//std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
		std::string sErrorMsg = "AnalyticInstanceGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//TODO :: Add to log
	}
}

int AnalyticInstanceGateway::updateStatus(unsigned int iAnalyticInstanceId, int iStatus)
{
	const std::string QUERY = "UPDATE analytic_instances SET status = ? WHERE id = ? ;";

	try
	{
		_pStatementPtr = (*_pDbConnPtr).prepareStatement(QUERY);

		(*_pStatementPtr).setInt(1, iStatus); // set the analytic instance id
		(*_pStatementPtr).setInt(2, iAnalyticInstanceId);

		int recordUpdate  = 0;
		recordUpdate = (*_pStatementPtr).executeUpdate();

		return recordUpdate;

	}
	catch (sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticInstanceGateway:findAll: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
}

} /* namespace db */
} /* namespace opencctv */
