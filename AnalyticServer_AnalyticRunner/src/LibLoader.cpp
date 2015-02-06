/*
 * LibLoader.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: anjana
 */

#include "LibLoader.hpp"

LibLoader::LibLoader() {
	this->libPath = "";
	this->handle = NULL;
}

LibLoader::LibLoader(const string& libPath) {
	this->libPath = libPath;
	this->handle = NULL;
}

LibLoader::~LibLoader() {
	// TODO Auto-generated destructor stub
}

int LibLoader::load() {

	int result = 1;

	//TODO Try to implement with try...catch error handling

	if (libPath.size() != 0 && !libPath.empty()) {
		//Prepare to load the analytics library
		//cout << endl;
		//cout << "AnalyticRunner:LibLoader: Opening " << libPath << "............" << endl;
		//TODO Where are the analytics stored????

		handle = dlopen(libPath.c_str(), RTLD_LAZY);

		if (!handle) {
			cerr << "AnalyticRunner:LibLoader: Cannot open library: " << dlerror() << endl;
			result = -2;
		}

	} else { //Analytic name is not available; thus cannot load it
		result = -1;
	}

	return result;
}

bool LibLoader::init(const map<string, string>& analyticParameters) {
	bool result = true;

	if (handle) {
		// Reset errors
		dlerror();

		//Load the method symbol "init"
		//TODO generalize mechanism of obtaining pointers to the analytic functions
		init_t init_r = (init_t) dlsym(handle, "init");

		const char *dlsym_error = dlerror();
		if (dlsym_error) {

			cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'init': " << dlsym_error << endl;
			dlclose(handle);
			//TODO Is it correct to close the handle here?
			result = false;

		} else {
			//Call the init() method
			result = init_r(analyticParameters);
		}
	} else {
		result = false;
		cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'init' -- Library : " << libPath
				<< " not loaded...." << endl;
	}

	return result;
}

int LibLoader::process(Mat& inputImg, string& resultStr) {
	int result = 0;

	if (handle) {
		// Reset errors
		dlerror();

		//Load the method symbol "process"
		//TODO generalize mechanism of obtaining pointers to the analytic functions
		process_t process_r = (process_t) dlsym(handle, "process");

		const char *dlsym_error = dlerror();
		if (dlsym_error) {

			cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'process': " << dlsym_error
					<< endl;
			dlclose(handle);
			//TODO Is it correct to close the handle here?
			result = -1;

		} else {

			//Call the process() method
			result = process_r(inputImg, resultStr);
		}
	} else {
		result = -2;
		cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'process' -- Library : " << libPath
				<< " not loaded...." << endl;
	}

	return result;

}

string LibLoader::geResult() {
	string result = "";

	if (handle) {
		// Reset errors
		dlerror();

		//Load the method symbol "geResult"
		//TODO generalize mechanism of obtaining pointers to the analytic functions
		results_t result_r = (results_t) dlsym(handle, "geResult");

		const char *dlsym_error = dlerror();
		if (dlsym_error) {

			std::cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'geResult': " << dlsym_error
					<< endl;
			dlclose(handle);
			//TODO Is it correct to close the handle here?
			result = "";

		} else {

			//Call the geResult() method
			result = result_r();
		}
	} else {
		result = "";
		cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'geResult' -- Library : " << libPath
				<< " not loaded...." << endl;
	}

	return result;
}

Mat LibLoader::getOutputImage() {
	Mat resultImg;

	if (handle) {
		// Reset errors
		dlerror();

		//Load the method symbol "getOutputImage"
		//TODO generalize mechanism of obtaining pointers to the analytic functions
		output_t output_r = (output_t) dlsym(handle, "getOutputImage");

		const char *dlsym_error = dlerror();
		if (dlsym_error) {

			cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'getOutputImage': " << dlsym_error
					<< endl;
			dlclose(handle);

		} else {
			//Call the getOutputImage() method
			resultImg = output_r();
		}
	} else {
		cerr << "AnalyticRunner:LibLoader: Cannot load symbol 'getOutputImage' -- Library : " << libPath
				<< " not loaded...." << endl;
	}

	return resultImg;
}
