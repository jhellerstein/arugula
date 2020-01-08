#ifndef LWW_MRG_H
#define LWW_MRG_H

#include "maxmin_mrg.h"

// LWW merge (from Anna)
// LWW merge operator for lattices of pairs (timestamp, val)
typedef struct LWWStruct {
  template <typename T, typename F,
            template <typename, typename> class L>
  auto operator()(const L<std::tuple<Lattice<unsigned long long, Max>, 
                                     Lattice<T, F> >, struct LWWStruct> &left, 
                  const L<std::tuple<Lattice<unsigned long long, Max>, 
                                     Lattice<T, F> >, struct LWWStruct> &right) const {
      LexicoMax lm;
      return lm(left, right);
    // here's the code I used prior to reusing LexicoMax
    // Lattice<unsigned long long, Max> lTS = std::get<0>(left.reveal());
    // Lattice<unsigned long long, Max> rTS = std::get<0>(right.reveal());
    // unsigned long long maxTS = (lTS + rTS).reveal();
    // unsigned long long leftTS = std::get<0>(left.reveal()).reveal();
    // auto retval = left;
    // if (maxTS != leftTS) {
    //   retval = right;
    // }
    // return retval;
  }
  friend std::ostream &operator<<(std::ostream &os, const struct LWWStruct m) {
    os << "LWWMerge";
    return (os);
  }
} LWWMerge;

#endif // LWW_MRG_H