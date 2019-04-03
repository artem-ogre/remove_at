# remove_at
Remove multiple elements at selected indices

## Overview
C++ standard library `<algorithm>` provides functions: [`std::remove` and `std::remove_if`](http://en.cppreference.com/w/cpp/algorithm/remove). These functions provide convenient and performant way to remove elements from `std::vector` using [erase-remove idiom](https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom). However only removing elements by value or by a predicate is possible.

Often is is desired to **remove multiple elements from a vector based on a selection**, where selection is a **collection of element indices**. Function **`remove_at`** serves this purpose.

## remove_at

```c++
template <class ForwardIt, class SortUniqIndsFwdIt>
inline ForwardIt remove_at(
    ForwardIt first,
    ForwardIt last,
    SortUniqIndsFwdIt ii_first,
    SortUniqIndsFwdIt ii_last);
``` 
Remove elements in the range `[first; last)` with indices from the sorted unique range `[ii_first, ii_last)`.
 
### Parameters
 |  Parameters | Description |
 | ------------- | ------------- |
 |**`first`**, **`last`** | the range of elements to process |
 |**`index`** | index of element to remove from the range `[first; last)` |
 |**`ii_first`**, **`ii_last`** | the range of sorted element indices to remove from the range `[first; last)` |
 
### Type requirements and preconditions
- `ForwardIt` must meet the requirements of [`ForwardIterator`](http://en.cppreference.com/w/cpp/concept/ForwardIterator).
- The type of dereferenced `ForwardIt` must meet the requirements of [`CopyAssignable`](http://en.cppreference.com/w/cpp/concept/CopyAssignable). 
- Indices in the range `[ii_first, ii_last)` are be sorted in the ascending order and do not contain duplicates.

### Return value

Past-the-end iterator for the new range of values (if this is not end, then it points to an unspecified value, and so do iterators to any values between this iterator and end) 

### Complexity

Linear: O(n), where n is number of indices.

### Exceptions

If the algorithm fails to allocate memory, `std::bad_alloc` is thrown. 

### Implementation

```c++
template <class ForwardIt, class SortUniqIndsFwdIt>
inline ForwardIt remove_at(
    ForwardIt first,
    ForwardIt last,
    SortUniqIndsFwdIt ii_first,
    SortUniqIndsFwdIt ii_last)
{
    if(ii_first == ii_last) // no indices-to-remove are given
        return last;
    typedef typename std::iterator_traits<ForwardIt>::difference_type diff_t;
    typedef typename std::iterator_traits<SortUniqIndsFwdIt>::value_type ind_t;
    ForwardIt destination = first + static_cast<diff_t>(*ii_first);
    while(ii_first != ii_last)
    {
        // advance to an index after a chunk of elements-to-keep
        for(ind_t cur = *ii_first++; ii_first != ii_last; ++ii_first)
        {
            const ind_t nxt = *ii_first;
            if(nxt - cur > 1)
                break;
            cur = nxt;
        }
        // move the chunk of elements-to-keep to new destination
        const ForwardIt source_first =
            first + static_cast<diff_t>(*(ii_first - 1)) + 1;
        const ForwardIt source_last =
            ii_first != ii_last ? first + static_cast<diff_t>(*ii_first) : last;
        std::move(source_first, source_last, destination);
        // std::copy(source_first, source_last, destination) // c++98 version
        destination += source_last - source_first;
    }
    return destination;
}
```

### Example

The following code removes elements at indices `[0, 1, 2, 6, 7, 9]` from a `std::vector<int>` by shifting all non-selected elements to the left and then erasing the extra. This is an example of erase-remove idiom.
```c++
#include "remove_at.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    // vector to remove elements from
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // indices of elements to be removed
    std::vector<int> ii{0, 1, 7, 2, 6, 7, 9};

    // prepare indices
    std::sort(ii.begin(), ii.end());
    ii.erase(std::unique(ii.begin(), ii.end()), ii.end());

    // print: before
    for(size_t i = 0; i < v.size(); ++i)
        std::cout << " " << v[i];

    // remove elements at indices
    v.erase(remove_at(v.begin(), v.end(), ii.begin(), ii.end()), v.end());

    // print: after
    std::cout << std::endl;
    for(size_t i = 0; i < v.size(); ++i)
        std::cout << " " << v[i];
}
```
Output:
```
 0 1 2 3 4 5 6 7 8 9
 3 4 5 8
```
