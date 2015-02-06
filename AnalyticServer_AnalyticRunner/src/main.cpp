/*
 * main.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: anjana
 */

#include <iostream>
#include <string>
#include "LibExecutor.hpp"

using namespace std;

int main(int argc, char* argv[])
{

	int result = 1;

	if (argc >= 8 ){

		int streamId = atoi(argv[1]);
		int analyticInstId = atoi(argv[2]);
		int analyticId = atoi(argv[3]);
		string analyticLocation = argv[4];
		string imageQueueInPort = argv[5];
		string imageQueueOutPort = argv[6];
		int count = atoi(argv[7]);

		LibExecutor libExecutor(streamId, analyticInstId, analyticId, analyticLocation, imageQueueInPort, imageQueueOutPort, count);
		result = libExecutor.start();

	}else{//Invalid number of arguments
		 cerr<< "AnalyticRunner:main - Cannot run the analytics : Invalid number of arguments...." << endl;
		 result = -1;
	}

	return result;
}


