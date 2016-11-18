/*
 * TypeDefinitions.hpp
 *
 *  Created on: Jun 1, 2015
 *      Author: anjana
 */

#ifndef TYPEDEFINITIONS_HPP_
#define TYPEDEFINITIONS_HPP_

namespace opencctv {
namespace util {

const std::string MSG_TYPE_START_REQ = "StartRequest";
const std::string MSG_TYPE_STOP_REQ = "StopRequest";
const std::string MSG_TYPE_STATUS_REQ = "StatusRequest";
const std::string MSG_TYPE_RESTART_REQ = "RestartRequest";
const std::string MSG_TYPE_START_ANALYTIC_REQ = "StartAnalytic";
const std::string MSG_TYPE_STOP_ANALYTIC_REQ = "StopAnalytic";
const std::string MSG_TYPE_INVALID = "Invalid";

const std::string SVR_STATUS_UNKNOWN = "Unknown";
const std::string SVR_STATUS_RUNNING = "Running";
const std::string SVR_STATUS_STOPPED = "Stopped";
const std::string SVR_STATUS_RESTARTING = "Restarting";

} /* namespace util */
} /* namespace opencctv */



#endif /* TYPEDEFINITIONS_HPP_ */
