#pragma once
#include <iostream>
#include <cassert>
#include <string>
#include "GlobalValue.h"

namespace NSLibrary {
//------------------------------------------------------------------------------
// CIndentor declaration
//------------------------------------------------------------------------------

class CIndentor {
public:
  CIndentor(const CIndentor&) = default;
  CIndentor(CIndentor&&) = default;
  CIndentor& operator=(const CIndentor&) = delete;
  CIndentor& operator=(CIndentor&&) = delete;

  inline CIndentor(unsigned int* level);
  inline ~CIndentor();
  inline void release();
private:
  unsigned int* plevel_;
  bool is_active_;
};
//------------------------------------------------------------------------------
// CPrinter declaration
//------------------------------------------------------------------------------

template<class TTag = void>
class CPrinter {
  class CLevelTag {};
  using CLevel = CGlobalValue<unsigned int, CLevelTag>;
public:
  using string = std::string;
  inline static CIndentor print(const string& msg);
  inline static unsigned int& level();
private:
  inline static void print_white_spaces(unsigned int level);
  static constexpr const char* kTabbing_ = "  ";
};
//------------------------------------------------------------------------------
// CIndentor definition
//------------------------------------------------------------------------------

inline CIndentor::CIndentor(unsigned int* level) : plevel_(level), is_active_(true) {
  ++(*plevel_);
}

inline CIndentor::~CIndentor() {
  if (is_active_) {
    assert(*plevel_ > 0);
    --(*plevel_);
    is_active_ = false;
  }
}

inline void CIndentor::release() {
  if (is_active_) {
    assert(*plevel_ > 0);
    --(*plevel_);
    is_active_ = false;
  }
}
//------------------------------------------------------------------------------
// CPrinter definition
//------------------------------------------------------------------------------

template<class TTag>
inline CIndentor CPrinter<TTag>::print(const string& msg) {
  print_white_spaces(CLevel::value());
  std::cout << msg << std::endl;
  return CIndentor(&(CLevel::value()));
}

template<class TTag>
inline unsigned int& CPrinter<TTag>::level() {
  return CLevel::value();
}

template<class TTag>
inline void CPrinter<TTag>::print_white_spaces(unsigned int level) {
  while (level > 0) {
    std::cout << kTabbing_;
    --level;
  }
}
//------------------------------------------------------------------------------
} // NSLibrary
//------------------------------------------------------------------------------
