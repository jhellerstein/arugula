#ifndef CAUSAL_MRG_H
#define CAUSAL_MRG_H

#include "vector_clock_mrg.hpp"

typedef struct CausalMergeStruct {
  template <typename T, typename F,
            template <typename, typename> class L>
  auto operator()(const L<std::tuple<VectorClock, 
                                     Lattice<T, F> >, struct CausalMergeStruct> &left, 
                  const L<std::tuple<VectorClock, 
                                     Lattice<T, F> >, struct CausalMergeStruct> &right) const {
    VectorClock vl = std::get<0>(left.reveal());
    VectorClock vr = std::get<0>(right.reveal());
    VectorClock prev = vl;

    vl += vr;

    Lattice<T, F> value(std::get<1>(right.reveal())); // set to 
    if (vl == vr) {
      // already initialized
      // value = std::get<1>(right.reveal());
    } else if (!(vl == prev)) {
      auto l = std::get<1>(left.reveal());
      auto r = std::get<1>(right.reveal());
      value = l + r;
    }
    std::tuple<VectorClock,Lattice<T,F> > v(vl, value);
    struct CausalMergeStruct m;
    return Lattice(v, m);
  }
  friend std::ostream &operator<<(std::ostream &os, const struct CausalMergeStruct m) {
    os << "CausalMerge";
    return (os);
  }
} CausalMerge;

#endif // CAUSAL_MRG_H