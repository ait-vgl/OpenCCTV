/*
 * UnitTest_test.cpp
 *
 *  Created on: Oct 4, 2016
 *      Author: nhutumdai
 */

#include "UnitTest.h"

#define BOOST_TEST_MODULE UnitTest
#include <boost/test/unit_test.hpp>

namespace opencctv {

BOOST_AUTO_TEST_CASE( constructors_test ) {
	UnitTest cs0("");                                                 // 1 //
	BOOST_CHECK_EQUAL(cs0.length(), (size_t)0);

	BOOST_CHECK(cs0.is_empty());

	UnitTest cs01( NULL);                                              // 2 //
	BOOST_CHECK_EQUAL(cs01.length(), (size_t)0);
	BOOST_CHECK(cs01.is_empty());

	UnitTest cs1("test_string");                                      // 3 //
	BOOST_CHECK_EQUAL(std::strcmp( cs1.data(), "test_string" ), 0);
	BOOST_CHECK_EQUAL(cs1.length(), std::strlen("test_string"));

	std::string s("test_string");                                         // 4 //
	//UnitTest cs2(s);
	//BOOST_CHECK_EQUAL(std::strcmp( cs2.data(), "test_string" ), 0);

	UnitTest cs3(cs1);                                                // 5 //
	BOOST_CHECK_EQUAL(std::strcmp( cs1.data(), "test_string" ), 0);

	UnitTest cs4("test_string", 4);                                   // 6 //
	BOOST_CHECK_EQUAL(std::strncmp( cs4.data(), "test", cs4.length() ), 0);

	UnitTest cs5(s.data(), s.data() + s.length());                    // 7 //
	BOOST_CHECK_EQUAL(std::strncmp( cs5.data(), "test_string", cs5.length() ), 0);

	UnitTest cs_array[] = { "str1", "str2" };                           // 8 //
	BOOST_CHECK_EQUAL(cs_array[0], "str1");
	BOOST_CHECK_EQUAL(cs_array[1], "str2");
}

} /* namespace opencctv */

