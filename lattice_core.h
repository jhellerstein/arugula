#ifndef LATTICE_CORE_H
#define LATTICE_CORE_H

// Has not been tuned for memory or performance efficiency

#include <iostream>
#include "binexpr.h"


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

  const T& reveal() const { return (val); }
  const Func& merge_op() const { return mrg; }

  friend std::ostream& operator<<(std::ostream& os, const Lattice<T, Func>& l) {
    os << "[" << (l.reveal()) << ", " << l.merge_op() << "]";
    return os;
  }
};

#endif // #ifndef LATTICE_CORE_H