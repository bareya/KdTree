# Kd Tree

Fun project with basic implementation of KdTree using templates. Current implementation is based on searching median through `nth_element` call.

To instantiate `KdTree`:

```cpp
#include "KdTree.h"

int main()
{
    std::vector<Entry> entries;
    KdTree<Entry, 3 /*dimensions*/> kd_tree{entries};
}
```

`KdTree` structure expects full specialization of following classes:

Used to calculate lower and upper bound(bounding box):

```cpp
template<> struct KdMin<Entry>
{
    static Entry value(const Entry& l, const Entry& r)
    {
        return // calculate min of two entries
    }
};

template<> struct KdMax<Entry>
{
    static Entry value(const Entry& l, const Entry& r)
    {
        return // calculate max of two entries
    }
};
```

`nth_element` is not used explicitly on the array of entries. Instead lookup index array is being saved. We need to tell `KdTree` how to compare entries based on indices:

```cpp
template <> struct KdComparator<Entry>
{
    KdComparator(const std::vector<Entry>& e, KdIndex d)
        : entries(e)
        , dimension(d)
    {}

    bool operator()(KdIndex l, KdIndex r) const
    {
        // compare based on dimentions
    }

private:
    const std::vector<Entry>& entries;
    const KdIndex dimension;
};
```

![KdTree_v1](images/kdtree_v1.png "KdTree v1")
