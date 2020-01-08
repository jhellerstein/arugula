#ifndef VECTOR_OP_H
#define VECTOR_OP_H

#include <algorithm>
#include "../utils/ptr_compare.h"

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

#endif // VECTOR_OP_H