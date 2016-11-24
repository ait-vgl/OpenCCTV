#include "AnalyticInstanceGateway.hpp"

namespace opencctv {
namespace db {

const std::string AnalyticInstanceGateway::_UPDATE_ANALYTICINSTANCE_SQL = "UPDATE analytic_instances SET status = ? WHERE id = ? ;";

AnalyticInstanceGateway::AnalyticInstanceGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
        _pStatementPtr = NULL;
       
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticInstanceGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}

}

AnalyticInstanceGateway::~AnalyticInstanceGateway() {
    
    (*_pStatementPtr).close();
	delete _pStatementPtr;  _pStatementPtr = NULL;
	delete _pDbConnPtr; _pDbConnPtr = NULL;
    
}


int AnalyticInstanceGateway::updateStatus(unsigned int iAnalyticInstanceId, int iStatus)
{
	try
	{
         _pStatementPtr = (*_pDbConnPtr).prepareStatement(_UPDATE_ANALYTICINSTANCE_SQL);
		
        (*_pStatementPtr).setInt(1, iStatus); // set the analytic instance id
        (*_pStatementPtr).setInt(2, iAnalyticInstanceId); 
        
        int recordUpdate  = 0;
		recordUpdate = (*_pStatementPtr).executeUpdate();

        return recordUpdate;
        
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticInstanceGateway:findAll: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
}

} /* namespace db */
} /* namespace opencctv */
