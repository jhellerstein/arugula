#ifndef MAXMIN_MRG_H
#define MAXMIN_MRG_H

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

// Lexicographic Max: works on tuple types, vectors, etc.
typedef Max LexicoMax;

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

// Lexicographic Min: works on tuple types, vectors, etc.
typedef Min LexicoMin;

#endif // MAXMIN_MRG_H