#ifndef LATTICE_MERGE_H
#define LATTICE_MERGE_H

// Inspired by expression templates in:
// https://gieseanw.wordpress.com/2019/10/20/we-dont-need-no-stinking-expression-templates/
// https://wandbox.org/permlink/0byiMsdCFgNw7Bq1

// Has not been tuned for memory or performance efficiency

#include <algorithm>
#include <iostream>
#include <map>
// #include <range/v3/all.hpp>
#include <set>
#include <type_traits>
#include <unordered_set>
#include "ptr_compare.h"

// Bool merge operator for lattices whose domain has operator|| defined
typedef struct OrStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    return left.reveal() || right.reveal();
  }
  friend std::ostream &operator<<(std::ostream &os, const struct OrStruct m) {
    os << "Or";
    return (os);
  }
} Or;

// Bool merge operator for lattices whose domain has operator&& defined
typedef struct AndStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    return left.reveal() && right.reveal();
  }
  friend std::ostream &operator<<(std::ostream &os, const struct AndStruct m) {
    os << "And";
    return (os);
  }
} And;

// Max merge operator for lattices whose domain has operator>= defined
typedef struct MaxStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    return (left.reveal() >= right.reveal()) ? left : right;
  }
  friend std::ostream &operator<<(std::ostream &os, const struct MaxStruct m) {
    os << "Max";
    return (os);
  }
} Max;

// Max merge operator for lattices whose domain has operator<= defined
typedef struct MinStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    return (left.reveal() <= right.reveal()) ? left : right;
  }
  friend std::ostream &operator<<(std::ostream &os, const struct MinStruct m) {
    os << "Min";
    return (os);
  }
} Min;

// Union merge operator for lattices whose domain has .merge() defined
typedef struct UnionStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    T l = left.reveal();
    T r = right.reveal();
    l.merge(std::move(r));
    return (L<T, F>(l));
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const struct UnionStruct m) {
    os << "Union";
    return (os);
  }
} Union;

// Intersect merge operator for lattices whose domain has .merge() defined
// Currently seems pretty expensive: lots of copies!
typedef struct IntersectStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    T l = left.reveal();
    T r = right.reveal();
    T out;
    auto it = std::set_intersection(l.begin(), l.end(), r.begin(), r.end(),
                                    std::inserter(out, out.begin()));
    return (L<T, F>(out));
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const struct IntersectStruct m) {
    os << "Intersect";
    return (os);
  }
} Intersect;

// Union merge operator for lattices whose domain has .merge() defined
typedef struct VectorUnionStruct {
  template <typename E, typename F, template <typename, typename> class L>
  auto operator()(const L<std::vector<E>, F> &left,
                  const L<std::vector<E>, F> &right) const {
    const std::vector<E> &l = left.reveal();
    const std::vector<E> &r = right.reveal();
    std::vector<E> merged;
    merged.insert(merged.end(), std::make_move_iterator(l.begin()),
                  std::make_move_iterator(l.end()));
    merged.insert(merged.end(), std::make_move_iterator(r.begin()),
                  std::make_move_iterator(r.end()));
    std::sort(merged.begin(), merged.end(), pcmp<E>);
    auto last = std::unique(merged.begin(), merged.end(), peq<E>);
    merged.resize(std::distance(merged.begin(), last));
    return (L<std::vector<E>, F>(std::move(merged)));
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const struct VectorUnionStruct m) {
    os << "VectorUnion";
    return (os);
  }
} VectorUnion;

// // Union merge operator for ranges
// typedef struct RangeUnionStruct
// {
//    template<typename T, typename F, template<typename, typename> class L>
//    auto operator()(const L<T,F>& left, const L<T,F>& right) const
//    {
//       return(L<T,F>(ranges::views::concat(left.reveal(), right.reveal())));
//    }
//    friend std::ostream& operator<<(std::ostream& os, const struct
//    RangeUnionStruct m) {
//       os << "RangeUnion";
//       return (os);
//    }
// } RangeUnion;

// MapUnion merge operator for lattice whose domain is of type <Map, Lattice<T>>
typedef struct MapUnionStruct {
  template <typename T, typename F, template <typename, typename> class L>
  auto operator()(const L<T, F> &left, const L<T, F> &right) const {
    auto r = right.reveal();
    auto merged = new std::remove_reference_t<T>(left.reveal());

    for (auto &i : r) {
      // insert makes a copy; emplace would not.
      auto iterflag = merged->insert(i);
      auto iter = iterflag.first;
      auto flag = iterflag.second;
      if (!flag) {
        auto newval = (iter->second + i.second);
        merged->erase(iter);
        merged->emplace(
            std::pair<decltype(i.first), decltype(i.second)>(i.first, newval));
      }
    }

    T retval = T(*merged);
    delete merged;
    return (retval);
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const struct MapUnionStruct m) {
    os << "MapUnion";
    return (os);
  }
} MapUnion;

#endif // LATTICE_MERGE_H