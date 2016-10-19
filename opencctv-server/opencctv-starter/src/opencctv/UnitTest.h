/*
 * UnitTest.h
 *
 *  Created on: Oct 4, 2016
 *      Author: nhutumdai
 */

#ifndef UNITTEST_H_
#define UNITTEST_H_

#include <cstddef>

using namespace std;
namespace opencctv {

class UnitTest {
public:

	UnitTest();

	UnitTest(char const* s);
	UnitTest(char const* s, size_t length);
	UnitTest(char const* begin, char const* end);
	//UnitTest(string const* s);


	// Access methods
	char const* data() const;
	size_t length() const;
	bool is_empty() const;

	virtual ~UnitTest();

};

} /* namespace opencctv */

#endif /* UNITTEST_H_ */
