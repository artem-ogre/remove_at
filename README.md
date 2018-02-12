# remove_at
Remove multiple elements at given indices

## Overview
C++ standard library `<algorithm>` provides functions: [`std::remove` and `std::remove_if`](http://en.cppreference.com/w/cpp/algorithm/remove). These functions provide convenient and performant way to remove elements from `std::vector` using [erase-remove idiom](https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom). However only removing elements by value or by a predicate is possible.

Often is is desired to **remove multiple elements from a vector based on a selection**, where selection is a **collection of element indices**. Function **`remove_at`** serves this purpose.

## remove_at
```c++
template <class ForwardIt, class SortedIndicesForwardIt>
inline ForwardIt remove_at(
    ForwardIt first,
    ForwardIt last,
    SortedIndicesForwardIt indices_fist,
    SortedIndicesForwardIt indices_last);
``` 

 Removes the elements in the range `[first; last)` with indices from the sorted range `[indices_fist, indices_last)`.
 
 |  Parameters | |
 | ------------- | ------------- |
 |**`first`**, **`last`** | the range of elements to process |
 |**`indices_fist`**, **`indices_last`** | the range of sorted element indices to remove from the range `[first; last)` |
 
 ### Type requirements and preconditions
- `ForwardIt` must meet the requirements of [`ForwardIterator`](http://en.cppreference.com/w/cpp/concept/ForwardIterator).
- The type of dereferenced `ForwardIt` must meet the requirements of [`CopyAssignable`](http://en.cppreference.com/w/cpp/concept/CopyAssignable). 
- Indices in the range `[indices_fist, indices_last)` should be sorted in the ascending order

### Return value

Past-the-end iterator for the new range of values (if this is not end, then it points to an unspecified value, and so do iterators to any values between this iterator and end) 

### Complexity

Linear: O(n). 

### Exceptions

If the algorithm fails to allocate memory, `std::bad_alloc` is thrown. 

### Implementation

```c++
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
```

### Example

The following code removes elements at indices (6,3,1) from a `std::vector<int>` by shifting all non-selected elements to the left and then erasing the extra. This is an example of erase-remove idiom.
```c++
#include "remove_at.hpp"
#include <algorithm>
#include <iostream>

int main()
{
    std::vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> indices{6, 3, 1};

    for(size_t i = 0; i < vec.size(); i++)
        std::cout << " " << vec[i];

    std::sort(indices.begin(), indices.end());
    vec.erase(
        remove_at(vec.begin(), vec.end(), indices.begin(), indices.end()),
        vec.end());

    std::cout << std::endl;
    for(size_t i = 0; i < vec.size(); i++)
        std::cout << " " << vec[i];
}
```
Output:
```
 0 1 2 3 4 5 6 7 8 9
 0 2 4 5 7 8 9
```
