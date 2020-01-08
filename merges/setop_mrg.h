#ifndef SETOP_MRG_H
#define SETOP_MRG_H

// Inspired by expression templates in:
// https://gieseanw.wordpress.com/2019/10/20/we-dont-need-no-stinking-expression-templates/
// https://wandbox.org/permlink/0byiMsdCFgNw7Bq1

// Has not been tuned for memory or performance efficiency

#include <algorithm>

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


#endif // SETOP_MRG_H