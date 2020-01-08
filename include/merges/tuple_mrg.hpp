#ifndef TUPLE_MRG_H
#define TUPLE_MRG_H

// from https://stackoverflow.com/questions/4832949/c-iterating-over-a-tuple-resolution-of-type-vs-constant-parameters
template< std::size_t I = 0, typename... Ts >
inline typename std::enable_if< I == sizeof... (Ts), std::tuple< Ts... >& >::type operator +=(std::tuple< Ts... >& lhs, const std::tuple< Ts... >& rhs)
{
    return lhs;
}

template< std::size_t I = 0, typename... Ts >
inline typename std::enable_if< I != sizeof... (Ts), std::tuple< Ts... >& >::type operator +=(std::tuple< Ts... >& lhs, const std::tuple< Ts... >& rhs)
{
    std::get< I >(lhs) += std::get< I >(rhs);
    return operator +=< I + 1, Ts... >(lhs, rhs);
}

// Max merge operator for lattices of tuples of lattices
typedef struct TupleMergeStruct {
  template <typename M, typename... Ts,
            template <typename, typename> class L>
  auto operator()(const L<std::tuple<Ts...>, M> &ls, 
                  const L<std::tuple<Ts...>, M> &rs) const {
    auto retval = ls.reveal();
    retval += rs.reveal();

    return Lattice(retval, M{});
  }
  friend std::ostream &operator<<(std::ostream &os, const struct TupleMergeStruct m) {
    os << "TupleMerge";
    return (os);
  }
} TupleMerge;

#endif // TUPLE_MRG_H