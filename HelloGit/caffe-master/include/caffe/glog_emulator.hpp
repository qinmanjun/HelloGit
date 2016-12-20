/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_CAFFE_GLOG_EMULATOR_HPP__
#define __OPENCV_CAFFE_GLOG_EMULATOR_HPP__
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>


static int LOG_OCCURRENCES = 0, LOG_OCCURRENCES_MOD_N = 0; 
static int LOG_OCCURRENCE = 0;



class GLogWrapper
{
    const char *file, *func, *type, *cond_str;
    int line;
    bool cond_staus, exit_loop;
    std::stringstream sstream;
   

public:

    GLogWrapper(const char *_file, const char *_func, int _line,
          const char *_type,
          const char *_cond_str = NULL, bool _cond_status = true
    ) :
        file(_file), func(_func), type(_type), cond_str(_cond_str),
        line(_line), cond_staus(_cond_status), exit_loop(true) {}

    std::iostream &stream()
    {
        return sstream;
    }

    bool exit()
    {
        return exit_loop;
    }

    void check()
    {
        exit_loop = false;

        if (cond_str && !cond_staus)
        {
            cv::error(cv::Exception(CV_StsError, "FAILED: " + cv::String(cond_str) + ". " + sstream.str(), func, file, line));
        }
        else if (!cond_str && strcmp(type, "CHECK"))
        {
            if (!strcmp(type, "INFO"))
                std::cout << sstream.str() << std::endl;
            else
                std::cerr << sstream.str() << std::endl;
        }
    }
    void no_check()
    {
      exit_loop = false;
    }


};
template <typename T>
T* CheckNotNull(T* t) {
  if (t == NULL) {
    cv::error(cv::Exception(CV_StsError, "FAILED: The pointer must be no null", CV_Func, __FILE__, __LINE__));
  }
  return t;
}

#if 1

#define CHECK(cond)     for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #cond, cond); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_EQ(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"="#b, ((a) == (b))); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_NE(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"!="#b, ((a) != (b))); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_LE(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"<="#b, ((a) <= (b))); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_LT(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"<"#b, ((a) < (b))); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_GE(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a">="#b, ((a) >= (b))); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_GT(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a">"#b, ((a) > (b))); _logger.exit(); _logger.check()) _logger.stream()
#define CHECK_NOTNULL(a)  CheckNotNull(a)
#define LOG(TYPE)       for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, #TYPE); _logger.exit(); _logger.check()) _logger.stream()

#define LOG_IF(TYPE, condition) \
  if (condition) \
    LOG(TYPE)
    
#define LOG_EVERY_N(TYPE, n) \
  ++LOG_OCCURRENCES; \
  if (++LOG_OCCURRENCES_MOD_N > n) LOG_OCCURRENCES_MOD_N -= n; \
  if (LOG_OCCURRENCES_MOD_N == 1) \
    LOG(TYPE)
#define LOG_FIRST_N(TYPE, n) \
  if (LOG_OCCURRENCE <= n) \
    ++LOG_OCCURRENCE; \
  if (LOG_OCCURRENCE <= n) \
    LOG(TYPE)
#define DLOG(TYPE) LOG(TYPE)
#define DCHECK(cond) CHECK(cond)
#define DCHECK_EQ(a,b) CHECK_EQ(a,b)
#define DCHECK_NE(a,b) CHECK_NE(a,b)
#define DCHECK_LE(a,b) CHECK_LE(a,b)
#define DCHECK_LT(a,b) CHECK_LT(a,b)
#define DCHECK_GE(a,b) CHECK_GE(a,b)
#define DCHECK_GT(a,b) CHECK_GT(a,b)

#else 
#define CHECK(cond)     for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #cond, cond); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_EQ(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"="#b, ((a) == (b))); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_NE(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"!="#b, ((a) != (b))); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_LE(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"<="#b, ((a) <= (b))); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_LT(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a"<"#b, ((a) < (b))); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_GE(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a">="#b, ((a) >= (b))); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_GT(a, b)  for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, "CHECK", #a">"#b, ((a) > (b))); _logger.exit(); _logger.no_check()) _logger.stream()
#define CHECK_NOTNULL(a)  CheckNotNull(a)
#define LOG(TYPE)       for(GLogWrapper _logger(__FILE__, CV_Func, __LINE__, #TYPE); _logger.exit(); _logger.no_check()) _logger.stream()

#define LOG_IF(TYPE, condition) \
  if (condition) \
    LOG(TYPE)
    
#define LOG_EVERY_N(TYPE, n) \
  ++LOG_OCCURRENCES; \
  if (++LOG_OCCURRENCES_MOD_N > n) LOG_OCCURRENCES_MOD_N -= n; \
  if (LOG_OCCURRENCES_MOD_N == 1) \
    LOG(TYPE)
#define LOG_FIRST_N(TYPE, n) \
  if (LOG_OCCURRENCE <= n) \
    ++LOG_OCCURRENCE; \
  if (LOG_OCCURRENCE <= n) \
    LOG(TYPE)
#define DLOG(TYPE) LOG(TYPE)
#define DCHECK(cond) CHECK(cond)
#define DCHECK_EQ(a,b) CHECK_EQ(a,b)
#define DCHECK_NE(a,b) CHECK_NE(a,b)
#define DCHECK_LE(a,b) CHECK_LE(a,b)
#define DCHECK_LT(a,b) CHECK_LT(a,b)
#define DCHECK_GE(a,b) CHECK_GE(a,b)
#define DCHECK_GT(a,b) CHECK_GT(a,b)
#endif



#endif
