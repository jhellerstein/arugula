#ifndef COLLECTION_IO_H
#define COLLECTION_IO_H

// Inspired by:
// https://gieseanw.wordpress.com/2019/10/20/we-dont-need-no-stinking-expression-templates/
// https://wandbox.org/permlink/0byiMsdCFgNw7Bq1

// Has not been tuned for memory or performance efficiency

#include <iostream>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>

// I/O stream for map type
template<class T, class U>
std::ostream& operator<< (std::ostream &out, const std::map<T, U> &s)
{
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it == s.begin())
            out << "(" << (T)(it->first) << "," << (U)(it->second) << ")" ;
        else
            out << ", " << "(" << (T)(it->first) << "," << (U)(it->second) << ")";
    }
    return out;
}

// I/O stream for sets
template<class T>
std::ostream& operator<< (std::ostream &out, const std::set<T> &s)
{
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it == s.begin())
            out << "{" << *it;
        else
            out << "," << *it;
    }
    out << "}";
    return out;
}

// I/O stream for unordered_sets
template<class T>
std::ostream& operator<< (std::ostream &out, const std::unordered_set<T> &s)
{
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it == s.begin())
            out << "{" << *it;
        else
            out << "," << *it;
    }
    out << "}";
    return out;
}

// // I/O stream for container types
// // Causes problems -- string types are container types, so leads to ambiguity on printing strings
// template<class T, template <typename> class Container>
// std::ostream& operator<< (std::ostream &out, const Container<T> &s)
// {
//  for (auto it=s.begin(); it != s.end(); ++it) {
//      if (it == s.begin())
//          out << *it;
//      else
//          out << ", " << *it;
//     }
//     return out;
// }

#endif // COLLECTION_IO_H