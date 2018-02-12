#ifndef REMOVE_AT_HPP
#define REMOVE_AT_HPP

#include <iterator>
#include <vector>

/*!
 * Removes the elements in the range [first; last) with indices from the sorted
 * range [indices_fist, indices_last)
 */
template <class ForwardIt, class SortedIndicesForwardIt>
inline ForwardIt remove_at(
    ForwardIt first,
    ForwardIt last,
    SortedIndicesForwardIt indices_fist,
    SortedIndicesForwardIt indices_last)
{
    typedef typename std::vector<bool> Flags;
    // flag elements to keep
    Flags is_keep(
        static_cast<Flags::size_type>(std::distance(first, last)), true);
    for(; indices_fist != indices_last; ++indices_fist)
        is_keep[static_cast<Flags::size_type>(*indices_fist)] = false;
    // move kept elements to beginning
    ForwardIt result = first;
    for(Flags::const_iterator keepIt = is_keep.begin(); first != last;
        ++first, ++keepIt)
    {
        if(*keepIt)
        {
            *result = *first;
            result++;
        }
    }
    return result;
}

#endif // REMOVE_AT_HPP
