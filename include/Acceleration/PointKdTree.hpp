#ifndef ACCELERATION_KDTREE_H
#define ACCELERATION_KDTREE_H

#include <Primitive/Types.hpp>
#include <Primitive/Range.hpp>

#include <algorithm>
#include <vector>
#include <numeric>

//
// Indexed Point Kd Comparator has been build to work with Vector2 and Vector3.
// Make full specialization for other types, remember to implement SetDimension().
//
template <typename T> struct IndexedPointKdComparator
{
    IndexedPointKdComparator(const std::vector<T>& entries)
        : entries_(entries)
        , dimension_{}
    {
    }

    void SetDimension(Index d)
    {
        assert(d < T::DIM);

        dimension_ = d;
    }

    bool operator()(Index l, Index r) const
    {
        assert(static_cast<size_t>(l) < entries_.size());
        assert(static_cast<size_t>(r) < entries_.size());

        return entries_[l][dimension_] < entries_[r][dimension_];
    }

private:
    const std::vector<T>& entries_;
    Index dimension_;
};

size_t operator"" _t(unsigned long long int x) { return x; }

///
///
///
class PointKdNode
{
public:
    explicit PointKdNode(const Range& range)
        : range_{range}
        , split_{-1}
    {
    }

    const Range& GetRange() const { return range_; }

    void SetSplit(Index split) { split_ = split; }
    const Index& GetSplit() const { return split_; }

    void SetChildren(PointKdNode* lower, PointKdNode* upper)
    {
        lower_ = lower;
        upper_ = upper;
    }
    const PointKdNode* Lower() const { return lower_; }
    const PointKdNode* Upper() const { return upper_; }
    bool IsLeaf() const { return false; }

private:
    Range range_; // keeps range of points
    Index split_; // index where split was performed

    PointKdNode* lower_{};
    PointKdNode* upper_{};
};


///
///
///
template <typename T, int _Dim> 
struct PointKdTree
{
    using value_type = T;

    explicit PointKdTree(const std::vector<T>& entries, size_t max_node_entries = 2)
    {
        max_node_entries = max_node_entries < 2 ? 2 : max_node_entries;

        const size_t num_entries = entries.size();
        if (num_entries < max_node_entries)
        {
            return;
        }

        // preallocate space of nodes
        const size_t round_power = RoundToPower2(num_entries);
        const size_t max_num_nodes = std::min(std::max(round_power - 1_t, 1_t), 2 * num_entries - round_power / 2 - 1);
        nodes_.reserve(max_num_nodes);

        // create root
        PointKdNode* root = AppendNode(Range{0_i,static_cast<Index>(entries.size())});

        // create stack and push the root
        struct StackEntry
        {
            StackEntry(PointKdNode* n, size_t d)
                : node(n)
                , dimension(d)
            {
            }

            Index NextDimension() const { return (dimension + 1) % _Dim; }

            PointKdNode* node;
            Index dimension;
        };
        std::vector<StackEntry> stack;
        stack.reserve(64);
        stack.emplace_back(root, 0);

        // sortable lookup array, filled with indices
        index_lookup_.resize(num_entries);
        std::iota(index_lookup_.begin(), index_lookup_.end(), 0);

        IndexedPointKdComparator<T> indexed_comparator(entries);

        while (!stack.empty())
        {
            // take from the stack
            StackEntry stack_entry = stack.back();
            stack.pop_back();

            PointKdNode* node = stack_entry.node;
            const auto& range = node->GetRange();

            // leaf found, compute bounds
            if (range.Size() <= static_cast<Index>(max_node_entries))
            {
                T range_min = ComputeBound(entries, range, min);
                min_ = min(min_, range_min);

                T range_max = ComputeBound(entries, range, max);
                max_ = max(max_, range_max);
                continue;
            }

            // partial sort
            //const KdComparator<T> dim_cmp{entries, stack_entry.dimension};
            indexed_comparator.SetDimension(stack_entry.dimension);
            const Index range_middle = range.Center();
            std::nth_element(index_lookup_.begin() + range.Min(), index_lookup_.begin() + range_middle,
                             index_lookup_.begin() + range.Max(), indexed_comparator);

            // update split to median
            node->SetSplit(index_lookup_[range_middle]);

            // Create lower and upper child
            auto split_range = range.Split(0.5_r);
            PointKdNode* lower_node = AppendNode(split_range.first);
            PointKdNode* upper_node = AppendNode(split_range.second);
            node->SetChildren(lower_node, upper_node);

            // append children to the stack
            const Index next_dimension = stack_entry.NextDimension();
            stack.emplace_back(lower_node, next_dimension);
            stack.emplace_back(upper_node, next_dimension);
        }
    }

    const PointKdNode* Root() const { return nodes_.empty() ? nullptr : &nodes_.front(); }

    const T& GetMin() const { return min_; }
    const T& GetMax() const { return max_; }

private:
    template <typename _UINT> _UINT RoundToPower2(_UINT n) const
    {
        // decrement so only less significant bits are filled
        // fill the bits
        // increment, less significant set to 0, most significant to 1
        static_assert(std::is_unsigned<_UINT>::value, "");
        --n;
        for (_UINT i{1}; i < sizeof(_UINT) * CHAR_BIT; i *= 2)
        {
            n |= n >> i;
        }
        return ++n;
    }

    T ComputeBound(const std::vector<T>& entries, const Range& range, T (*func)(const T& l, const T& r)) const
    {
        const auto num_entries = range.Size();
        if (num_entries < 1)
        {
            return T{};
        }

        T bound_entry = entries[range.Min()];
        for (auto i = range.Min() + 1; i < range.Min(); ++i)
        {
            bound_entry = func(bound_entry, entries[i]);
        }

        return bound_entry;
    }

    template <typename... _Args> PointKdNode* AppendNode(_Args&&... args)
    {
        assert(nodes_.size() < nodes_.capacity());

        nodes_.emplace_back(args...);
        return &nodes_.back();
    }

    T min_;
    T max_;

    std::vector<PointKdNode> nodes_;
    std::vector<Index> index_lookup_;
};

#endif // ACCELERATION_KDTREE_H
