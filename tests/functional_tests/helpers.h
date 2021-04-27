// Copyright (c) 2019-2021 Zano Project
// Copyright (c) 2020-2021 sowle (val@zano.org, crypto.sowle@gmail.com)
// Copyright (c) 2006-2013, Andrey N. Sabelnikov, www.sabelnikov.net
// All rights reserved.
//
// Part of this file originates from the epee library.
//
// epee licese:
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// * Neither the name of the Andrey N. Sabelnikov nor the
// names of its contributors may be used to endorse or promote products
// derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER  BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Zano licese:
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include <stdio.h>
#include <iostream>
#include <atomic>

#if defined(_MSC_VER)
#  define WIN32_LEAN_AND_MEAN 1
#  define NOMINMAX
#  include <windows.h>
#  include <io.h>
#else
#  include <unistd.h>
#  include <signal.h>
#endif


#ifdef _DEBUG 
#  if (defined _MSC_VER)
#    define _ASSERTE__(expr)   if(!expr) {__debugbreak();}
#  else
#    define _ASSERTE__(expr)   if(!expr) {raise(SIGTRAP);}
#  endif
#else
#  define _ASSERTE__(expr)  
#endif

#define COMBINE1(X,Y) X##Y
#define COMBINE(X,Y) COMBINE1(X,Y)
#define _STR(X) #X
#define STR(X) _STR(X)

#if defined(_MSC_VER)
#define LOCAL_FUNCTION_DEF__ __FUNCTION__
#else
#define LOCAL_FUNCTION_DEF__ __FUNCTION__ 
#endif 

#define LOCATION_SS "[" << LOCAL_FUNCTION_DEF__ << ("] @ " __FILE__ ":" STR(__LINE__))
#define LOCATION_STR (std::string("[") + LOCAL_FUNCTION_DEF__ + "] @ " __FILE__ ":" STR(__LINE__))

#define LOG_LEVEL_SILENT     -1
#define LOG_LEVEL_0     0
#define LOG_LEVEL_1     1
#define LOG_LEVEL_2     2
#define LOG_LEVEL_3     3
#define LOG_LEVEL_4     4
#define LOG_LEVEL_MIN   LOG_LEVEL_SILENT
#define LOG_LEVEL_MAX   LOG_LEVEL_4

#define CONSOLE_DEFAULT_STREAM std::cout

inline bool is_stdout_a_tty()
{
  static std::atomic<bool> initialized(false);
  static std::atomic<bool> is_a_tty(false);

  if (!initialized.load(std::memory_order_acquire))
  {
#if defined(_MSC_VER)
    is_a_tty.store(0 != _isatty(_fileno(stdout)), std::memory_order_relaxed);
#else
    is_a_tty.store(0 != isatty(fileno(stdout)), std::memory_order_relaxed);
#endif
    initialized.store(true, std::memory_order_release);
  }

  return is_a_tty.load(std::memory_order_relaxed);
}

enum console_colors
{
  console_color_default,
  console_color_white,
  console_color_red,
  console_color_green,
  console_color_blue,
  console_color_cyan,
  console_color_magenta,
  console_color_yellow
};

inline void set_console_color(int color, bool bright)
{
  if (!is_stdout_a_tty())
    return;

  switch (color)
  {
  case console_color_default:
  {
#ifdef _MSC_VER 
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;37m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0m";
#endif
  }
  break;
  case console_color_white:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;37m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;37m";
#endif
  }
  break;
  case console_color_red:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_RED | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;31m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;31m";
#endif
  }
  break;
  case console_color_green:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_GREEN | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;32m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;32m";
#endif
  }
  break;

  case console_color_blue:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_BLUE | FOREGROUND_INTENSITY);//(bright ? FOREGROUND_INTENSITY:0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;34m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;34m";
#endif
  }
  break;

  case console_color_cyan:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_GREEN | FOREGROUND_BLUE | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;36m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;36m";
#endif
  }
  break;

  case console_color_magenta:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_BLUE | FOREGROUND_RED | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;35m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;35m";
#endif
  }
  break;

  case console_color_yellow:
  {
#ifdef _MSC_VER
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_stdout, FOREGROUND_RED | FOREGROUND_GREEN | (bright ? FOREGROUND_INTENSITY : 0));
#else
    if (bright)
      CONSOLE_DEFAULT_STREAM << "\033[1;33m";
    else
      CONSOLE_DEFAULT_STREAM << "\033[0;33m";
#endif
  }
  break;

  }
}

inline void reset_console_color() {
  if (!is_stdout_a_tty())
    return;

#ifdef _MSC_VER
  HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(h_stdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
  CONSOLE_DEFAULT_STREAM << "\033[0m";
  CONSOLE_DEFAULT_STREAM.flush();
#endif
}


#define ENDL std::endl

#ifndef LOG_PRINT
#define LOG_PRINT(msg, lvl) do { set_console_color(console_color_white, lvl == LOG_LEVEL_0); std::cout << msg << ENDL; reset_console_color(); } while(0)
#endif

#ifndef LOG_PRINT_L0
#define LOG_PRINT_L0(msg) LOG_PRINT(msg, LOG_LEVEL_0)
#endif

#ifndef LOG_PRINT_RED
#define LOG_PRINT_RED(msg, lvl) do { set_console_color(console_color_red, lvl == LOG_LEVEL_0); std::cout << msg << ENDL; reset_console_color(); } while(0)
#endif

#ifndef LOG_PRINT_RED_L0
#define LOG_PRINT_RED_L0(msg) LOG_PRINT_RED(msg, LOG_LEVEL_0)
#endif

#ifndef LOG_PRINT_GREEN
#define LOG_PRINT_GREEN(msg, lvl) do { set_console_color(console_color_green, lvl == LOG_LEVEL_0); std::cout << msg << ENDL; reset_console_color(); } while(0)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(message) do { LOG_PRINT_RED(ENDL << "ERROR: " << message, LOG_LEVEL_0); _ASSERTE__(0); } while(0)
#endif

#ifndef CHECK_AND_ASSERT_MES
#define CHECK_AND_ASSERT_MES(expr, fail_ret_val, message)   do{if(!(expr)) {LOG_ERROR(message); return fail_ret_val;};}while(0)
#endif

#ifndef CHECK_AND_ASSERT_MES_CUSTOM
#define CHECK_AND_ASSERT_MES_CUSTOM(expr, fail_ret_val, custom_code, message)   do{if(!(expr)) {LOG_ERROR(message); custom_code; return fail_ret_val;};}while(0)
#endif



//
// performance
//
#define TIME_MEASURE_START(var_name)  uint64_t var_name = 0;std::chrono::high_resolution_clock::time_point var_name##_chrono = std::chrono::high_resolution_clock::now();
#define TIME_MEASURE_FINISH(var_name)   var_name = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - var_name##_chrono).count();



//
// other helpers
//
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/locale/encoding_utf.hpp>

#define AUTO_VAL_INIT(v)   boost::value_initialized<decltype(v)>()
#define AUTO_VAL_INIT_T(t)   boost::value_initialized<t>()

inline std::string num_to_string_fast(int64_t val)
{
  return boost::lexical_cast<std::string>(val);
}

inline std::wstring utf8_to_wstring(const std::string& str)
{
  return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}

template<class t_string>
bool save_string_to_file_throw(const t_string& path_to_file, const std::string& str)
{
  boost::filesystem::ofstream fstream;
  fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fstream.open(utf8_to_wstring(path_to_file), std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  fstream << str;
  fstream.close();
  return true;
}

template<class t_string>
bool save_string_to_file(const t_string& path_to_file, const std::string& str)
{
  try
  {
    return save_string_to_file_throw(path_to_file, str);
  }
  catch (...)
  {
    return false;
  }
}
