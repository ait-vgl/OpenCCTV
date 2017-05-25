/*
 * ResultAppInstController.cpp
 *
 *  Created on: May 11, 2017
 *      Author: anjana
 */

#include "ResultAppInstController.hpp"
#include "../analytic/ApplicationModel.hpp"

namespace result {

ResultAppInstController::ResultAppInstController(unsigned int iResultAppInstId)
{
	_iResultsAppInstId = iResultAppInstId;
	_pRAppConnector = NULL;
}

bool ResultAppInstController::init()
{
	//Step 1 - Find the results app instance details from DB
	result::db::ResultsAppInstanceGateway resultsAppInstGateway;
	result::db::dto::ResultsAppInstance rAppInst;
	rAppInst.setResultsAppInstanceId(0);
	std::stringstream ssMsg;

	try
	{
		resultsAppInstGateway.findRAppInstance(_iResultsAppInstId,rAppInst);
	}catch (opencctv::Exception &e)
	{
		ssMsg << "Database Error. " << e.what();
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
		return false;
	}

	//If failed to find results app instance in DB return error
	if(rAppInst.getResultsAppInstanceId() == 0)
	{
		ssMsg << "Results app instance id : " << _iResultsAppInstId;
		ssMsg << ". Failed to find the results app instance details in DB";
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
		return false;
	}

	//Step 2 - Load an instance of the results app connector plugin
	// If Results results app plugin loader has been constructed, use it.
	//Otherwise construct it
	analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();
	analytic::util::PluginLoader<result::api::ResultsAppConnector>* pRAppPluginLoader = NULL;
	unsigned int iResultsAppConnectorId = rAppInst.getResultsAppConnectorId();

	if (pModel->containsRAppPluginLoader(iResultsAppConnectorId))
	{
		pRAppPluginLoader = pModel->getRAppPluginLoaders()[iResultsAppConnectorId];
	}
	else
	{
		try
		{
			pRAppPluginLoader = new analytic::util::PluginLoader<result::api::ResultsAppConnector>();
			pRAppPluginLoader->loadPlugin(rAppInst.getLibFilePath());
			pModel->getRAppPluginLoaders()[iResultsAppConnectorId] = pRAppPluginLoader;;
		} catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to load plugin: ";
			sErrMsg.append(rAppInst.getResultsAppConnectorName()).append(". ");
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return false;
		}
	}

	//Create results app connector plugin instance
	try
	{
		_pRAppConnector = pRAppPluginLoader->createPluginInstance();

	}catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create results app connector for ";
		sErrMsg.append(rAppInst.getResultsAppInstanceName()).append(". ");
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	if(!_pRAppConnector)
	{
		return false;
	}

	//Step 3 - Read input files and parameters necessary for initializing the plugin.
	std::map<std::string, std::string> mapInputFiles;
	std::map<std::string, std::string> mapInputParams;
	std::string sOutputMsg;

	ssMsg << "Connector for results app instance  " << _iResultsAppInstId;
	ssMsg << " constructed.";
	opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());

	try
	{
		resultsAppInstGateway.findRAppInstanceFiles(_iResultsAppInstId, mapInputFiles);
		resultsAppInstGateway.findRAppInstanceParams(_iResultsAppInstId, mapInputParams);
	} catch (opencctv::Exception &e)
	{
		ssMsg << "Results app instance id: " << _iResultsAppInstId;
		ssMsg << ". Failed to initialize results app connector. ";
		ssMsg << e.what();
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
		return false;
	}

	if(_pRAppConnector->init(&mapInputParams, &mapInputFiles, sOutputMsg) != result::api::INIT_OK)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(sOutputMsg);
		return false;
	}

	//Step 4 - Add the initialized results app connector to the ApplicationModel
	std::pair <unsigned int,result::api::ResultsAppConnector*> rAppConn (iResultsAppConnectorId,_pRAppConnector);
	pModel->getRAppConnectors()[_iResultsAppInstId] = rAppConn;

	return true;
}

bool ResultAppInstController::authenticate()
{
	std::stringstream ssMsg;
	std::string sResult;

	if(!_pRAppConnector)
	{
		ssMsg << "Cannot find the connector plugin for the results app instance ";
		ssMsg << _iResultsAppInstId << ".";
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
		return false;
	}

	result::api::ResultAPICode iResultsCode = result::api::AUTH_FAIL;

	while(iResultsCode == result::api::AUTH_FAIL)
	{
		iResultsCode = _pRAppConnector->authenticate(sResult);
		if(iResultsCode == result::api::AUTH_OK)
		{
			break;
		}else
		{
			ssMsg << "Results app instance id: " << _iResultsAppInstId << ". ";
			ssMsg << sResult;
			opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
			boost::this_thread::sleep(boost::posix_time::seconds(5));
		}
	}

	return true;
}

bool ResultAppInstController::sendAllAnalyticInstInfo()
{
	//Find analytic instance info for this RAppInstance form DB
	//result::db::dto::AnalyticInstance analyticInstance;
	result::db::AnalyticInstanceGateway analyticInstanceGateway;
	std::vector<result::db::dto::AnalyticInstance> vAnalyticInstance;
	//std::map<std::string, std::string> mapAnalyticInstInfo;

	analyticInstanceGateway.findAnalyticInstancesForRAppInst(_iResultsAppInstId, vAnalyticInstance);

	//For each analytic instance send the info to the results app instance
	std::string sResult;
	bool bResult;
	std::stringstream ssMsg;
	for (std::vector<result::db::dto::AnalyticInstance>::iterator it = vAnalyticInstance.begin() ;
			it != vAnalyticInstance.end(); ++it)
	{
		if(!sendAnalyticInstInfo(*it))
		{
			ssMsg << sResult;
			ssMsg << ". Results app instance id: " << _iResultsAppInstId;
			ssMsg << ". Analytic instance id: " << it->getAnalyticInstId();
			ssMsg << " Failed to send analytic instance information to the results application instance";
			opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
		}
	}
	return true;
}

bool ResultAppInstController::sendAnalyticInstInfo(result::db::dto::AnalyticInstance analyticInstance)
{
	std::map<std::string, std::string> mapAnalyticInstInfo;
	std::stringstream ssId;
	std::string sResult;
	bool bResult;
	ssId << analyticInstance.getOpenCctvId();
	mapAnalyticInstInfo.insert(std::pair<std::string, std::string>("opencctv_id", ssId.str()));
	mapAnalyticInstInfo.insert(std::pair<std::string, std::string>("name", analyticInstance.getName()));
	mapAnalyticInstInfo.insert(std::pair<std::string, std::string>("description",analyticInstance.getDescription()));
	mapAnalyticInstInfo.insert(std::pair<std::string, std::string>("location", analyticInstance.getLocation()));
	bResult = _pRAppConnector->sendAnalyticInstanceInfo(&mapAnalyticInstInfo,sResult);

	std::map<unsigned int, AnalyticInstance_t>::iterator it;
	it =_mapAnalyticInstances.find(analyticInstance.getAnalyticInstId());
	//If the analytic instance is not in _mapAnalyticInstances insert it
	if(it == _mapAnalyticInstances.end())
	{
		AnalyticInstance_t analyticInst;
		analyticInst.analyticInstance = analyticInstance;
		analyticInst.isInfoSent = bResult;
		_mapAnalyticInstances.insert(std::pair<unsigned int,AnalyticInstance_t>(analyticInstance.getAnalyticInstId(),analyticInst));
	}else //Otherwise update the sent info status of the analytic instance
	{
		it->second.isInfoSent = bResult;
	}

	return bResult;
}

void ResultAppInstController::sendAnalyticResults()
{
	result::db::AnalyticResultGateway analyticResultGateway;
	std::vector<result::db::dto::AnalyticResult> vAnalyticResult;
	std::string sResult;
	int iResult;
	bool bResult;
	result::api::ResultAPICode iResultCode;

	while(true)
	{
		//Select all available analytic results for this results app instance
		analyticResultGateway.findAnalyticResultsForRAppInst(_iResultsAppInstId, vAnalyticResult);

		//Send each result
		std::vector<result::db::dto::AnalyticResult>::iterator vit;
		for (vit = vAnalyticResult.begin(); vit != vAnalyticResult.end(); /*++it*/)//Iterator incremented by erase operation
		{
			std::map<unsigned int, AnalyticInstance_t>::iterator mit;
			mit = _mapAnalyticInstances.find(vit->getAnalyticInstId());
			if(mit != _mapAnalyticInstances.end())
			{
				bResult = true;
				//If analytic instance info has not yet sent try to send them first
				if(!mit->second.isInfoSent)
				{
					bResult = sendAnalyticInstInfo(mit->second.analyticInstance);
				}

				if(bResult)
				{
					sendAnalyticResult(*vit);
				}
			}

			vit = vAnalyticResult.erase(vit);
		}
	}
}

void ResultAppInstController::sendAnalyticResult(result::db::dto::AnalyticResult& analyticResult)
{
	result::db::AnalyticResultGateway analyticResultGateway;
	std::vector<result::db::dto::AnalyticResult> vAnalyticResult;
	std::string sResult;
	int iResult;
	bool bResult;
	result::api::ResultAPICode iResultCode;
	std::stringstream ssMsg;

	std::map<std::string, std::string> mapData;
	mapData.insert(std::pair<std::string, std::string>("analytic_instance_id",boost::lexical_cast<std::string>(analyticResult.getAnalyticInstId())));
	mapData.insert(std::pair<std::string, std::string>("result", analyticResult.getResult()));
	mapData.insert(std::pair<std::string, std::string>("timestamp", analyticResult.getTimestamp()));

	//Find the images and videos related to this analytic result
	std::map<std::string, std::string> mapImages;
	std::map<std::string, std::string> mapVideos;
	analyticResultGateway.findResultImages(analyticResult.getResultsId(),mapImages);
	analyticResultGateway.findResultVideos(analyticResult.getResultsId(),mapVideos);

	iResultCode = _pRAppConnector->sendAnalyticResults(&mapData,&mapImages,&mapVideos,sResult);

	//If sent is successful, add a record in "sent_analytic_results" table in DB
	if(iResultCode == result::api::SEND_ANALYTIC_RESULT_OK)
	{
		iResult = analyticResultGateway.insertToSentResults(analyticResult.getResultsId(),_iResultsAppInstId);
	}else
	{
		ssMsg << sResult;
		ssMsg << ". Analytic results id: ";
		ssMsg << analyticResult.getResultsId();
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
	}
}

void ResultAppInstController::addAnalyticInstance(unsigned int iAnalyticInstId)
{
	//If the analytic instance already exist in _mapAnalyticInstances return
	std::map<unsigned int, AnalyticInstance_t>::iterator it;
	it =_mapAnalyticInstances.find(iAnalyticInstId);
	if(it != _mapAnalyticInstances.end())
	{
		return;
	}

	//Find the details of the analytic instance from the DB
	result::db::dto::AnalyticInstance analyticInstance;
	result::db::AnalyticInstanceGateway analyticInstanceGateway;
	analyticInstanceGateway.findAnalyticInstance(iAnalyticInstId,analyticInstance);

	//Add the details to the map _mapAnalyticInstances
	AnalyticInstance_t anlyticInst;
	anlyticInst.analyticInstance = analyticInstance;
	anlyticInst.isInfoSent = false;
	_mapAnalyticInstances.insert(std::pair<unsigned int,
			AnalyticInstance_t>(iAnalyticInstId,anlyticInst));
}

void ResultAppInstController::removeAnalyticInstance(unsigned int iAnalyticInstId)
{
	std::map<unsigned int, AnalyticInstance_t>::iterator it;
	it =_mapAnalyticInstances.find(iAnalyticInstId);
	if(it != _mapAnalyticInstances.end())
	{
		_mapAnalyticInstances.erase(it);
	}
}

ResultAppInstController::~ResultAppInstController()
{
	// TODO Auto-generated destructor stub
}

} /* namespace result */
