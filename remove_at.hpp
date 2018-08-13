#ifndef REMOVE_AT_HPP
#define REMOVE_AT_HPP

#include <iterator>
#include <vector>

//! Remove one element with given index from the range [first; last)
template <typename ForwardIt>
inline ForwardIt remove_at(ForwardIt first, ForwardIt last, const size_t index)
{
    std::advance(first, index);
    for(ForwardIt it = first + 1; it != last; ++it, ++first)
        *first = *it;
    return first;
}

/*!
 * Remove elements in the range [first; last) with indices from the sorted
 * range [indices_first, indices_last)
 */
template <typename ForwardIt, typename SortedIndicesForwardIt>
inline ForwardIt remove_at(
    ForwardIt first,
    ForwardIt last,
    SortedIndicesForwardIt indices_first,
    SortedIndicesForwardIt indices_last)
{
    typedef typename std::vector<bool> flags;
    // flag elements to keep
    flags is_keep(
        static_cast<flags::size_type>(std::distance(first, last)), true);
    for(; indices_first != indices_last; ++indices_first)
        is_keep[static_cast<flags::size_type>(*indices_first)] = false;
    // move kept elements to beginning
    ForwardIt result = first;
    for(flags::const_iterator it = is_keep.begin(); first != last; ++first, ++it)
        if(*it) // keep element
            *result++ = *first; //in c++11 and later use: *result++ = std::move(*first);
    return result;
}

#endif // REMOVE_AT_HPP
