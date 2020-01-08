#ifndef RANGE_MRG_H
#define RANGE_MRG_H

#include <range/v3/all.hpp>

// Union merge operator for ranges
// NOT YET WORKING
typedef struct RangeUnionStruct
{
   template<typename T, typename F, template<typename, typename> class L>
   auto operator()(const L<T,F>& left, const L<T,F>& right) const
   {
      return(L<T,F>(ranges::views::concat(left.reveal(), right.reveal())));
   }
   friend std::ostream& operator<<(std::ostream& os, const struct
   RangeUnionStruct m) {
      os << "RangeUnion";
      return (os);
   }
} RangeUnion;

#endif // RANGE_MRG_H