/*
 * Results_t.hpp
 * Author: Anjana Tissera
 *
 * NOTE : Do not modify this class
 *
 */

#ifndef ANALYTIC_RESULT_T_HPP_
#define ANALYTIC_RESULT_T_HPP_

#include <string>
#include <vector>

namespace analytic {
namespace api {

class Result_t final
{
private:
	//Path to text,yml,.. files produced as analytic results
	std::vector<std::string>* _pVResultsFiles;
	//Path to image(.jpg) files produced as analytic results
	std::vector<std::string>* _pVIamgeFiles;
	//Path to video(.mp4) files produced as analytic results
	std::vector<std::string>* _pVVideoFiles;
	std::string _sTimestamp;
	std::string _sCustomTextResult;

	//Copy constructor must be kept disabled
	Result_t(const Result_t& resultObj);
	//Assignment operator must be kept disabled
	Result_t& operator=( const Result_t& resultObj);

public:
	//Constructor
	Result_t()
	{
		_pVResultsFiles = new std::vector<std::string>;
		_pVIamgeFiles = new std::vector<std::string>;
		_pVVideoFiles = new std::vector<std::string>;
		_sTimestamp = "";
		_sCustomTextResult = "";
	}

	//Destructor
	~Result_t()
	{
		if (_pVResultsFiles){delete _pVResultsFiles; _pVResultsFiles = NULL;}
		if (_pVIamgeFiles){delete _pVIamgeFiles; _pVIamgeFiles = NULL;}
		if (_pVVideoFiles){delete _pVVideoFiles; _pVVideoFiles = NULL;}
	}

	const std::vector<std::string>* getVIamgeFiles() const
	{
		return _pVIamgeFiles;
	}

	const std::vector<std::string>* getVResultsFiles() const
	{
		return _pVResultsFiles;
	}

	const std::vector<std::string>* getVVideoFiles() const
	{
		return _pVVideoFiles;
	}

	const std::string& getCustomTextResult() const
	{
		return _sCustomTextResult;
	}

	void setCustomTextResult(const std::string& customTextResult)
	{
		_sCustomTextResult = customTextResult;
	}

	const std::string& getTimestamp() const
	{
		return _sTimestamp;
	}

	void setTimestamp(const std::string& timestamp)
	{
		_sTimestamp = timestamp;
	}

	void setResultsFile(const std::string& resultsFileAbsolutePath)
	{
		_pVResultsFiles->push_back(resultsFileAbsolutePath);
	}

	void setImageFile(const std::string& imageFileAbsolutePath)
	{
		_pVIamgeFiles->push_back(imageFileAbsolutePath);
	}

	void setVideoFile(const std::string& videoFileAbsolutePath)
	{
		_pVVideoFiles->push_back(videoFileAbsolutePath);
	}
};

} /* namespace api */
} /* namespace analytic */

#endif /* RESULTS_T_HPP_ */
