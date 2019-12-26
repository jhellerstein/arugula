#ifndef LATTICE_MERGE_H
#define LATTICE_MERGE_H

// Inspired by:
// https://gieseanw.wordpress.com/2019/10/20/we-dont-need-no-stinking-expression-templates/
// https://wandbox.org/permlink/0byiMsdCFgNw7Bq1

// Has not been tuned for memory or performance efficiency

#include <iostream>
#include <type_traits>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>


// Max merge operator for lattices whose domain has operator>= defined
typedef struct MaxStruct
{
   template<class T, class U>
   auto operator()(const T& left, const U& right) const
   {
      return (left.reveal() >= right.reveal()) ? left : right;
   }
   friend std::ostream& operator<<(std::ostream& os, const struct MaxStruct m) {
      os << "Max";
      return (os);
   }
} Max;

// Union merge operator for lattices whose domain has std::set_union, std::begin and std::end defined
typedef struct UnionStruct
{
   template<class T, class U>
   auto operator()(const T& left, const U& right) const
   {
      auto l = left.reveal();
      auto r = right.reveal();
      auto merged = new std::remove_reference_t<decltype(l)>();

      std::set_union(std::begin(l), std::end(l), std::begin(r), std::end(r),
                     std::inserter(*merged, std::begin(*merged)));

      T retval = T(*merged);
      delete merged;
      return (retval);
   }
   friend std::ostream& operator<<(std::ostream& os, const struct UnionStruct m) {
      os << "Union";
      return (os);
   }
} Union;

// MapUnion merge operator for lattice whose domain is of type <Map, Lattice<T>>
typedef struct MapUnionStruct
{
   template<class T, class U>
   auto operator()(const T& left, const U& right) const
   {
      auto l = left.reveal();
      auto r = right.reveal();
      auto merged = new std::remove_reference_t<decltype(l)>(l);

      for (auto &i : r)
      {
         // insert makes a copy; emplace would not.
         auto iterflag = merged->insert(i);
         auto iter = iterflag.first;
         auto flag = iterflag.second;
         if (!flag) {
            auto newval = (iter->second + i.second);
            merged->erase(iter);
            merged->insert(std::pair<decltype(i.first), decltype(i.second)>(i.first, newval));
         }
      }

      T retval = T(*merged);
      delete merged;
      return (retval);
   }
   friend std::ostream& operator<<(std::ostream& os, const struct MapUnionStruct m) {
      os << "MapUnion";
      return (os);
   }
} MapUnion;

#endif // LATTICE_MERGE_H