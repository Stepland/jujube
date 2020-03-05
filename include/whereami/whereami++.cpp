// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "whereami++.hpp"
#include "whereami.h"
#include <utility>


// wai_get*Path returns an incorrect length on Windows (1 more than required)
#ifdef _WIN32
#define WHEREAMI_CPP_MISPADDING 1
#else
#define WHEREAMI_CPP_MISPADDING 0
#endif


using whereami_func_t = int (*)(char * out, int capacity, int * dirname_length);


static std::string whereami_path(whereami_func_t whereami_func) {
	const auto length = whereami_func(nullptr, 0, nullptr);
	if(length == -1)
		return "";

	std::string ret(length - WHEREAMI_CPP_MISPADDING, '\0');
	whereami_func(&ret[0], length - WHEREAMI_CPP_MISPADDING, nullptr);
	return ret;
}

static std::pair<std::string, std::string> whereami_segmented(whereami_func_t whereami_func) {
	const auto length = whereami_func(nullptr, 0, nullptr);
	if(length == -1)
		return {"", ""};

	// Mispadding correction breaks libcode here and is unnecessary because we're constructing via C-string anyway
	int path_length;
	std::string buf(length, '\0');
	whereami_func(&buf[0], length, &path_length);
	return {{buf.c_str(), static_cast<std::size_t>(path_length)}, &buf[path_length + 1]};
}


std::string whereami::executable_path() {
	return whereami_path(wai_getExecutablePath);
}

std::string whereami::module_path() {
	return whereami_path(wai_getModulePath);
}

std::string whereami::executable_name() {
	return whereami_segmented(wai_getExecutablePath).second;
}

std::string whereami::module_name() {
	return whereami_segmented(wai_getModulePath).second;
}

std::string whereami::executable_dir() {
	return whereami_segmented(wai_getExecutablePath).first;
}

std::string whereami::module_dir() {
	return whereami_segmented(wai_getModulePath).first;
}
