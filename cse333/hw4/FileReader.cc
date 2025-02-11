/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to the students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>

extern "C" {
#include "libhw2/FileParser.h"
}

#include "./HttpUtils.h"
#include "./FileReader.h"

using std::string;

namespace hw4 {

bool FileReader::ReadFile(string* const contents) {
  string full_file = basedir_ + "/" + fname_;

  // Read the file into memory, and store the file contents in the
  // output parameter "str."  Be careful to handle binary data
  // correctly; i.e., you probably want to use the two-argument
  // constructor to std::string (the one that includes a length as a
  // second argument).
  //
  // You might find ::ReadFileToString() from HW2 useful here. Remember that
  // this function uses malloc to allocate memory, so you'll need to use
  // free() to free up that memory after copying to the string output
  // parameter.
  //
  // Alternatively, you can use a unique_ptr with a malloc/free
  // deleter to automatically manage this for you; see the comment in
  // HttpUtils.h above the MallocDeleter class for details.

  // STEP 1:

  if (!IsPathSafe(this->basedir_, this->fname_)) {
    // The given path is unsafe, return false
    return false;
  }

  int size;
  char* str = ::ReadFileToString(full_file.c_str(), &size);

  if (str == nullptr) {
    // Read file to string failed, return false.
    return false;
  }

  // Read file sucseeded! Copy the content into the output parameter
  *contents = string(str, size);

  // clean up
  free(str);
  return true;
}

}  // namespace hw4
