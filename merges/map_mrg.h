#ifndef MAP_MRG_H
#define MAP_MRG_H

#include <map>

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
    return (Lattice(retval, F{}));
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const struct MapUnionStruct m) {
    os << "MapUnion";
    return (os);
  }
} MapUnion;


#endif // MAP_MRG_H