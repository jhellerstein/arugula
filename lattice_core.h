#ifndef LATTICE_CORE_H
#define LATTICE_CORE_H

// Has not been tuned for memory or performance efficiency

#include <iostream>
#include "binexpr.h"
#include "is_container.h"
#include "ptr_compare.h"


template<class T, class Func>
struct Lattice
{
private:
  T val;
  const Func mrg;
public:
  // copy constructors, potentially expensive for large values.
  Lattice(const T& _val, Func _func) : val(_val), mrg(_func) {};
  Lattice(const T& _val) : val(_val), mrg(Func{}) {};

  // move constructors
  Lattice(T&& _val, Func _func) : val(std::move(_val)), mrg(_func) {};
  Lattice(T&& _val) : val(std::move(_val)), mrg(Func{}) {};


// This version of operator+ seems lazier, but requires an operator+ in BinaryExpression
// and even when I added that it was only lazy at the topmost level of the expression tree!
  // BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func> merge(Lattice<T, Func> &l2) {
  //  return BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func>(*this, l2, merge_op());
  // }

//  BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func> operator+(Lattice<T, Func> &l2) {
//    return this->merge(l2);
//  }

  // this version of operator+ does eager eval, which maybe we don't want.
  // but it gives us chaining like "x + y + z" WITHOUT requiring an operator+ in BinaryExpression
  Lattice<T, Func> operator+(Lattice<T, Func> &l2) {
    return Lattice<T, Func>(BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func>(*this, std::move(l2), merge_op()));
  }

  BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func> operator+(BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func> be) {
    return BinaryExpression<Lattice<T, Func>, Lattice<T, Func>, Func>(*this, be, merge_op());
  }

  const Func& merge_op() const { return mrg; }
  // a bunch of lattice wrapping and unwrapping here; might be nice to refactor
  // the merge functions to work on revealed value
  void merge(const Lattice<T, Func>& l) { val = merge_op()(*this, l).reveal(); }
  void merge(const T& v) { 
    Lattice<T, Func> l = Lattice<T, Func>(v);
    this->merge(l); 
  };
  void operator+=(const Lattice<T, Func>& l) { merge(l); };
  void operator+=(const T& v) { merge(v); };

  // These operators break lattice semantics!
  const T& reveal() const { return (val); }
  void wrap(T&& v) { val = std::move(v); }
  void assign(const T& v) { val = v; }
  void operator=(const T& v) { assign(v); };
  // assignment here is reveal assignment as in Anna; arguably should be by ref
  void assign(const Lattice<T,Func>& v) { val = v.reveal(); }
  void operator=(const Lattice<T, Func>& l) { assign(l); };
  // Not recommended for complex types!
  bool operator<(const Lattice<T, Func>& rhs) const { return this->reveal() < rhs.reveal(); }

  // equality via reveal as in Anna. Arguably should not be defined, but OK.
  // Even if we declare a container with PointerCmp comparison, operator==
  // runs on pointer equality. So we overload equality to use std::equal
  // with peq<Foo>. Notice some fancy C++ footwork:
  // 1) the use of enable_if_t wasn't working til we added template<class Q=T>
  // 2) use value::type to get the type of the container elements for peq<>! 
  template<class Q = T>
  typename std::enable_if_t<is_stl_container<Q>::value,bool>
  operator==(const Lattice<Q, Func>& rhs) const {
    return(std::equal(this->reveal().begin(), this->reveal().end(),
                      rhs.reveal().begin(), peq<typename Q::value_type>));
  }
  // Not an STL container? Default to standard operator==.
  template<class Q = T>
  typename std::enable_if_t<!is_stl_container<Q>::value,bool>
  operator==(const Lattice<Q, Func>& rhs) const {
      return this->reveal() == rhs.reveal();
  }
  bool operator!=(const Lattice<T, Func>& rhs) const { return !(operator==(rhs)); }

  // friend std::ostream& operator<<(std::ostream& os, const Lattice<T, Func>& l) {
  //   os << "[" << (l.reveal()) << ", " << l.merge_op() << "]";
  //   return os;
  // }
};

#endif // #ifndef LATTICE_CORE_H