#pragma once
#include <string>
#include "GlobalValue.h"
#include "Printer.h"

namespace NSLibrary {
//------------------------------------------------------------------------------
// declaration
//------------------------------------------------------------------------------

class CArticulate {};
class CSilent {};

//------------------------------------------------------------------------------
// CCTorSettings declaration
//------------------------------------------------------------------------------

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor>
class CPrintMode {
};
//------------------------------------------------------------------------------
// Predefined Settings declaration
//------------------------------------------------------------------------------
//
// These are predefined settings for CCTorCheck
// they cover the most interesting use cases
// 1) CSilentMode -- do not print any messages
// 2) CArticulateMode -- print messages from any constructor and assignment operator
// 3) CDefTorOnlyMode -- print messages from default constructor of CCTorCheck
// If SomeClass is a derived class of CCTorCheck<SomeClass, TPrintMode>
// the default constructor for CCTorCheck will be called from any constructor 
// of SomeClass, so we print message from any constructor of SomeClass
// that is neither copy nor move constructor
// 4) CCopyTorOnlyMode -- print messages from copy constructors only
// 5) CMoveTorOnlyMode -- print messages from move constructors only
// 6) CCopyAsgOnlyMode -- print messages from copy assignment operators only
// 7) CMoveAsgOnlyMode -- print messages from move assignment operators only
// 8) CDestructorOnlyMode -- print messages from destructors only
// 9) CCopyMode -- print messages from copy constructors and copy assignment operators
// 10) CMoveMode -- print messages from move constructors and move assignment operators
//------------------------------------------------------------------------------

using CSilentMode = CPrintMode<CSilent, CSilent, CSilent, CSilent, CSilent, CSilent>;
using CArticulateMode = CPrintMode<CArticulate, CArticulate, CArticulate, CArticulate, CArticulate, CArticulate>;
using CDefTorOnlyMode = CPrintMode<CArticulate, CSilent, CSilent, CSilent, CSilent, CSilent>;
using CCopyTorOnlyMode = CPrintMode<CSilent, CArticulate, CSilent, CSilent, CSilent, CSilent>;
using CMoveTorOnlyMode = CPrintMode<CSilent, CSilent, CArticulate, CSilent, CSilent, CSilent>;
using CCopyAsgOnlyMode = CPrintMode<CSilent, CSilent, CSilent, CArticulate, CSilent, CSilent>;
using CMoveAsgOnlyMode = CPrintMode<CSilent, CSilent, CSilent, CSilent, CArticulate, CSilent>;
using CDestructorOnlyMode = CPrintMode<CSilent, CSilent, CSilent, CSilent, CSilent, CArticulate>;
using CCopyMode = CPrintMode<CSilent, CArticulate, CSilent, CArticulate, CSilent, CSilent>;
using CMoveMode = CPrintMode<CSilent, CSilent, CArticulate, CSilent, CArticulate, CSilent>;
//------------------------------------------------------------------------------
// CCTorCheckBase declaration
//------------------------------------------------------------------------------

template<class TPrintMode, class TCTorCheck>
class CCTorCheckBase;

template<class TDefTor, class TCopyTor, class TMoveTor,
  class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
class CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck> {
protected:
  using CPrinter = CPrinter<TCTorCheck>;
  using CString = typename CPrinter::string;

  inline static void print_def_ctor_msg();
  inline static void print_copy_ctor_msg();
  inline static void print_move_ctor_msg();
  inline static void print_copy_asgmnt_msg();
  inline static void print_move_asgmnt_msg();
  inline static void print_destructor_msg();
private:
  template<class T>
  struct CPrintMsg;

  template<>
  struct CPrintMsg<CArticulate> {
    inline static void print(const CString& msg) {
      CPrinter::print(msg);
    }
  };
  template<>
  struct CPrintMsg<CSilent> {
    inline static void print(const CString& msg) {}
  };
};
//------------------------------------------------------------------------------
// CCTorCheck declaration
//------------------------------------------------------------------------------
//
// The class messages you if a constructor or assignment is done for the class
//
// WARNING: the class is NOT thread safe. Use in a single thread only!
//
// How to use:
// 1) Let SomeClass be the class you want to analyze
// You shold make it to be a publicly derived class from the class
// CCtorCheck<SomeClass, TPrintMode>
// where TPrintMode is one of these: 
// CSilentMode, CArticulateMode, CDefTorOnlyMode, CCopyTorOnlyMode, CMoveTorOnlyMode,
// CCopyAsgOnlyMode, CMoveAsgOnlyMode, CCopyMode, CMoveMode
// These parameters determine whether we print information about constructors
// and assignment operators or not
// The modes are described above
// 2) Depending on the TPrintMode SomeClass will inform you if any constructor
// or assignment operator is used
// 3) If you want to print some title for the block of code use CPrinter of SomeClass
// Consider the following example:
//   std::vector<TestClass> MyVector;
//   ...
//   auto VecReverseAnchor = SomeClass::CPrinter::print("Vector reverse");
//   std::reverse(MyVector.begin(), MyVector.end());
//   VecReverseAnchor.release();
// Now all messages from constructors and assignment operators inside of the block
// will be indented to the right appropriately
// If you want to indent all messages until the end of the scope, just
// do not use release method
// 4) If you want to compute the amount of, say, copy constructors in a block,
// then use an appropriate function from SomeClass, for example
//   std::vector<TestClass> MyVector;
//   ...
//   auto CopyCtorAnchor = SomeClass::countCopyCTors();
//   std::reverse(MyVector.begin(), MyVector.end());
//   CopyCtorAnchor.release();
// Now at the point of calling release method you will get notification on
// how many copy constructors were used inside of the block of the code
// If you want to compute all the copy constructors for the given scope
// just do not use release method
//------------------------------------------------------------------------------

template<class TClass, class TPrintMode = CSilentMode>
class CCTorCheck : protected CCTorCheckBase<TPrintMode, CCTorCheck<TClass, TPrintMode>> {
  using CSelf = CCTorCheck;
  using CBase = CCTorCheckBase<TPrintMode, CSelf>;
  class CCounter;
public:
  using CPrinter = typename CBase::CPrinter;

  inline CCTorCheck();
  inline CCTorCheck(const CCTorCheck&);
  inline CCTorCheck(CCTorCheck&&) noexcept;
  inline CCTorCheck& operator=(const CCTorCheck&);
  inline CCTorCheck& operator=(CCTorCheck&&) noexcept;
  inline ~CCTorCheck();

  inline static CCounter countDefaultCTors();
  inline static CCounter countCopyCTors();
  inline static CCounter countMoveCTors();
  inline static CCounter countCopyAsgnmts();
  inline static CCounter countMoveAsgnmts();
  inline static CCounter countDestructors();

private:
  using CString = typename CBase::CString;
  //------------------------------------------------------------------------------
  // CCounter declaration
  //------------------------------------------------------------------------------

  class CCounter {
    using CFunction = unsigned int& ();
    using CFunctionPtr = CFunction *;
  public:
    inline CCounter(const CCounter&) = default;
    inline CCounter(CCounter&&) = default;
    CCounter& operator=(const CCounter&) = delete;
    CCounter& operator=(CCounter&&) = delete;

    inline CCounter(CString msg_text, CFunctionPtr get_value);
    inline ~CCounter();

    inline void stop();
  private:
    inline void print_msg();

    CString msg_text_;
    CFunctionPtr get_value_;
    unsigned int value_;
    bool is_valid_;
  };
private:

  class CDefaultCTorTag;
  using CDefaultCTor = CGlobalValue<unsigned int, CDefaultCTorTag>;

  class CCopyCTorTag;
  using CCopyCTor = CGlobalValue<unsigned int, CCopyCTorTag>;

  class CMoveCTorTag;
  using CMoveCTor = CGlobalValue<unsigned int, CMoveCTorTag>;

  class CCopyAsgnmtTag;
  using CCopyAsgnmt = CGlobalValue<unsigned int, CCopyAsgnmtTag>;

  class CMoveAsgnmtTag;
  using CMoveAsgnmt = CGlobalValue<unsigned int, CMoveAsgnmtTag>;

  class CDestructorTag;
  using CDestructor = CGlobalValue<unsigned int, CDestructorTag>;
};
//------------------------------------------------------------------------------
// CCTorCheckBase definition
//------------------------------------------------------------------------------

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
inline void
CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck>::
print_def_ctor_msg() {
  CPrintMsg<TDefTor>::print("deft ctor");
}

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
inline void
CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck>::
print_copy_ctor_msg() {
  CPrintMsg<TCopyTor>::print("copy ctor");
}

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
inline void
CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck>::
print_move_ctor_msg() {
  CPrintMsg<TMoveTor>::print("move ctor");
}

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
inline void
CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck>::
print_copy_asgmnt_msg() {
  CPrintMsg<TCopyAsg>::print("copy asgt");
}

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
inline void
CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck>::
print_move_asgmnt_msg() {
  CPrintMsg<TMoveAsg>::print("move asgt");
}

template<class TDefTor, class TCopyTor, class TMoveTor, class TCopyAsg, class TMoveAsg, class TDestrTor, class TCTorCheck>
inline void
CCTorCheckBase<CPrintMode<TDefTor, TCopyTor, TMoveTor, TCopyAsg, TMoveAsg, TDestrTor>, TCTorCheck>::
print_destructor_msg() {
  CPrintMsg<TDestrTor>::print("destr tor");
}
//-----------------------------------------------------------------------------
// CCTorCheck definition
//------------------------------------------------------------------------------

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>::CCTorCheck() {
  ++CDefaultCTor::value();
  CBase::print_def_ctor_msg();
}

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>::CCTorCheck(const CCTorCheck&) {
  ++CCopyCTor::value();
  CBase::print_copy_ctor_msg();
}

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>::CCTorCheck(CCTorCheck&&) noexcept {
  ++CMoveCTor::value();
  CBase::print_move_ctor_msg();
}

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>& CCTorCheck<TClass, TPrintMode>::operator=(const CCTorCheck&) {
  ++CCopyAsgnmt::value();
  CBase::print_copy_asgmnt_msg();
  return *this;
}

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>& CCTorCheck<TClass, TPrintMode>::operator=(CCTorCheck&&) noexcept {
  ++CMoveAsgnmt::value();
  CBase::print_move_asgmnt_msg();
  return *this;
}

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>::~CCTorCheck() {
  ++CDestructor::value();
  CBase::print_destructor_msg();
}

template<class TClass, class TPrintMode>
inline typename CCTorCheck<TClass, TPrintMode>::CCounter
CCTorCheck<TClass, TPrintMode>::countDefaultCTors() {
  return CCTorCheck::CCounter("deft ctor", &CCTorCheck::CDefaultCTor::value);
}

template<class TClass, class TPrintMode>
inline typename CCTorCheck<TClass, TPrintMode>::CCounter
CCTorCheck<TClass, TPrintMode>::countCopyCTors() {
  return CCTorCheck::CCounter("copy ctor", &CCTorCheck::CCopyCTor::value);
}

template<class TClass, class TPrintMode>
inline typename CCTorCheck<TClass, TPrintMode>::CCounter
CCTorCheck<TClass, TPrintMode>::countMoveCTors() {
  return CCTorCheck::CCounter("move ctor", &CCTorCheck::CMoveCTor::value);
}

template<class TClass, class TPrintMode>
inline typename CCTorCheck<TClass, TPrintMode>::CCounter
CCTorCheck<TClass, TPrintMode>::countCopyAsgnmts() {
  return CCTorCheck::CCounter("copy asgt", &CCTorCheck::CCopyAsgnmt::value);
}

template<class TClass, class TPrintMode>
inline typename CCTorCheck<TClass, TPrintMode>::CCounter
CCTorCheck<TClass, TPrintMode>::countMoveAsgnmts() {
  return CCTorCheck::CCounter("move asgt", &CCTorCheck::CMoveAsgnmt::value);
}

template<class TClass, class TPrintMode>
inline typename CCTorCheck<TClass, TPrintMode>::CCounter
CCTorCheck<TClass, TPrintMode>::countDestructors() {
  return CCTorCheck::CCounter("destr tor", &CCTorCheck::CDestructor::value);
}
//------------------------------------------------------------------------------
// CCTorCheck::CCounter definition
//------------------------------------------------------------------------------

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>::CCounter::
CCounter(CString msg_text, CFunctionPtr get_value)
  : msg_text_(msg_text), get_value_(get_value), value_(get_value()), is_valid_(true) {
}

template<class TClass, class TPrintMode>
inline CCTorCheck<TClass, TPrintMode>::CCounter::~CCounter() {
  if (is_valid_)
    print_msg();
}

template<class TClass, class TPrintMode>
inline void CCTorCheck<TClass, TPrintMode>::CCounter::stop() {
  if (is_valid_) {
    print_msg();
    is_valid_ = false;
  }
}

template<class TClass, class TPrintMode>
inline void CCTorCheck<TClass, TPrintMode>::CCounter::print_msg() {
  CString msg = "amount of " + msg_text_ + " = ";
  msg += std::to_string(get_value_() - value_);
  CPrinter::print(msg);
}
//------------------------------------------------------------------------------
} // NSLibrary
//------------------------------------------------------------------------------
