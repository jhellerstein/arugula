#ifndef BOOLEAN_MRG_H
#define BOOLEAN_MRG_H

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

#endif // BOOLEAN_MRG_H