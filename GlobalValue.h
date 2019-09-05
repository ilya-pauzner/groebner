#pragma once
#include <type_traits>

namespace NSLibrary {
//------------------------------------------------------------------------------
// CGlobalValue
//------------------------------------------------------------------------------

template<class T, class TTag>
class CGlobalValue {
  static_assert(std::is_integral_v<T>, "Type T must be integral type!");
public:
  static T& value() {
    static T value_ = 0;
    return value_;
  }
};
//------------------------------------------------------------------------------
} // NSLibrary
//------------------------------------------------------------------------------
