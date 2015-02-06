/*
 * main.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: lakindu
 */

#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "mq/TcpMq.hpp"
#include "AnalyticServerMessage.hpp"
#include "util/ConfigDetails.hpp"
#include "filetransfer/FileTransferServer.hpp"

using namespace std;
using namespace mq;
using namespace util;
using namespace filetransfer;


string getQueueAddress(string hostname, string portNumber) ;

int main(int argc, char* argv[])
{
	int iCount = 1; //A temp var; the number of images to be processed by an analytic
	//TODO : Remove the count in the later implementations.

	if (argc > 1) {
		iCount = atoi(argv[1]);
	}

	ConfigDetails *_pConfigDetails = ConfigDetails::getInstance();
	if(!_pConfigDetails)
	{
		exit(EXIT_FAILURE);
	}

	string sAnalyticServerAddress = _pConfigDetails->getHostAddress();
	unsigned int iAnalyticServerPort = _pConfigDetails->getHostPort();

	//cout << "sAnalyticServerAddress : " << sAnalyticServerAddress << " iAnalyticServerPort : " << iAnalyticServerPort << endl;

	/*FileTransferServer fileTransferServer(8888);
	fileTransferServer.receiveAllFiles();
	exit(0);*/


	cout << "AnalyticStarter:main: Analytic server started....." << endl << endl;

	TcpMq mq;
	mq.createNew(iAnalyticServerPort, ZMQ_REP);
	while(true)
	{
		//Read analytic start request from OpenCCTV server
		string sRequestStr = mq.read();


		//Extract the data in the analytic start request
		string sAnalyticLocation;
		unsigned int iStreamId;
		unsigned int iAnalyticInstanceId;
		unsigned int iAnalyticId;
		AnalyticServerMessage::extractStartAnalyticRequestData(sRequestStr, sAnalyticLocation, iAnalyticInstanceId, iAnalyticId, iStreamId);

		//Input and output queue addresses of the analytic queues
		string sAnalyticQueueInPort = boost::lexical_cast<string>(++iAnalyticServerPort);
		string sAnalyticQueueInAddress  = getQueueAddress(sAnalyticServerAddress, sAnalyticQueueInPort);

		string sAnalyticQueueOutPort = boost::lexical_cast<string>(++iAnalyticServerPort);
		string sAnalyticQueueOutAddress  = getQueueAddress(sAnalyticServerAddress, sAnalyticQueueOutPort);

		cout << "Received and analytic start request for analytic " << sAnalyticLocation << endl;
		cout << " Image queues are at : in - " << sAnalyticQueueInAddress << " out - " << sAnalyticQueueOutAddress<< endl << endl;

		pid_t pid = fork();
		if(pid == 0)
		{
			char* exec_args[9];

			string sPathStr = "/usr/local/opencctv/AnalyticServer_AnalyticRunner";
			exec_args[0] = &sPathStr[0];

			string sStremaIdStr = boost::lexical_cast<string>(iStreamId);
			exec_args[1] = &sStremaIdStr[0];

			string sAnalyticInstIdStr = boost::lexical_cast<string>(iAnalyticInstanceId);
			exec_args[2] = &sAnalyticInstIdStr[0];

			string sAnalyticIdStr = boost::lexical_cast<string>(iAnalyticId);
			exec_args[3] = &sAnalyticIdStr[0];

			exec_args[4] = &sAnalyticLocation[0];

			exec_args[5] = &sAnalyticQueueInPort[0];

			exec_args[6] = &sAnalyticQueueOutPort[0];

			string sNumCount = boost::lexical_cast<string>(iCount);
			exec_args[7] = &sNumCount[0];

			exec_args[8] = NULL;

			execv(exec_args[0], exec_args);

			//If exec returns, process must have failed.
			cout << "AnalyticStarter:main: Analytics Process " << sAnalyticLocation  <<" Failed........ " << endl;
			exit(-1);
		}

		string reply = AnalyticServerMessage::getStartAnalyticReply(sAnalyticQueueInAddress,sAnalyticQueueOutAddress);
		mq.write(reply);
	}
}

string getQueueAddress(string hostname, string portNumber) {

	string sAnalyticQueueAddress = "tcp://";
	//sAnalyticQueueAddress.append("localhost");
	//sAnalyticQueueAddress.append("192.41.170.182");
	sAnalyticQueueAddress.append(hostname);
	sAnalyticQueueAddress.append(":");
	sAnalyticQueueAddress.append(portNumber);

	return sAnalyticQueueAddress;

}
