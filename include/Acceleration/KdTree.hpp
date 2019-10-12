#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include <vector>
#include <numeric>

/// provide min implementation
template <typename T> struct KdMin
{
};

/// provide max implementation
template <typename T> struct KdMax
{
};

// provide KdComparator
template <typename T> struct KdComparator
{
};

// types
using KdIndex = long long;
KdIndex operator"" _kdi(unsigned long long int x) { return static_cast<KdIndex>(x); }

size_t operator"" _t(unsigned long long int x) { return x; }

///
///
///
struct KdRange
{
    template <typename T>
    explicit KdRange(const std::vector<T>& v)
        : beg(0)
        , end(v.size())
    {
    }

    explicit KdRange(KdIndex begin = 0_kdi, KdIndex end = 0_kdi)
        : beg(begin)
        , end(end)
    {
    }

    KdIndex Size() const { return end - beg; }
    KdIndex Middle() const { return beg + Size() / 2_kdi; }

    KdRange GetLower() const { return KdRange{beg, Middle() - 1}; }
    KdRange GetUpper() const { return KdRange{Middle(), end}; }

    KdIndex beg;
    KdIndex end;
};

///
///
///
class KdNode
{
public:
    explicit KdNode(const KdRange& range)
        : range_{range}
        , split_{-1}
    {
    }

    const KdRange& GetRange() const { return range_; }

    void SetSplit(KdIndex split) { split_ = split; }
    const KdIndex& GetSplit() const { return split_; }

    void SetChildren(KdNode* lower, KdNode* upper)
    {
        lower_ = lower;
        upper_ = upper;
    }
    const KdNode* Lower() const { return lower_; }
    const KdNode* Upper() const { return upper_; }
    bool IsLeaf() const { return false; }

private:
    KdRange range_; // keeps range of points
    KdIndex split_; // index where split was performed

    KdNode* lower_{};
    KdNode* upper_{};
};

///
///
///
template <typename T, int _Dim> 
struct KdTree
{
    using value_type = T;

    explicit KdTree(const std::vector<T>& entries, size_t max_node_entries = 2)
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
        KdNode* root = AppendNode(KdRange{entries});

        // create stack and push the root
        struct StackEntry
        {
            StackEntry(KdNode* n, size_t d)
                : node(n)
                , dimension(d)
            {
            }

            KdIndex NextDimension() const { return (dimension + 1) % _Dim; }

            KdNode* node;
            KdIndex dimension;
        };
        std::vector<StackEntry> stack;
        stack.reserve(64);
        stack.emplace_back(root, 0);

        // sortable lookup array, filled with indices
        index_lookup_.resize(num_entries);
        std::iota(index_lookup_.begin(), index_lookup_.end(), 0);

        while (!stack.empty())
        {
            // take from the stack
            StackEntry stack_entry = stack.back();
            stack.pop_back();

            KdNode* node = stack_entry.node;
            const KdRange& range = node->GetRange();

            // leaf found, compute bounds
            if (range.Size() <= static_cast<KdIndex>(max_node_entries))
            {
                T range_min = ComputeBound(entries, range, KdMin<T>::value);
                min_ = KdMin<T>::value(min_, range_min);

                T range_max = ComputeBound(entries, range, KdMax<T>::value);
                max_ = KdMax<T>::value(max_, range_max);
                continue;
            }

            // partial sort
            const KdComparator<T> dim_cmp{entries, stack_entry.dimension};
            const KdIndex range_middle = range.Middle();
            std::nth_element(index_lookup_.begin() + range.beg, 
                             index_lookup_.begin() + range_middle,
                             index_lookup_.begin() + range.end, dim_cmp);

            // update split to median
            node->SetSplit(index_lookup_[range_middle]);

            // Create lower and upper child
            KdNode* lower_node = AppendNode(range.GetLower());
            KdNode* upper_node = AppendNode(range.GetUpper());
            node->SetChildren(lower_node, upper_node);

            // append children to the stack
            const KdIndex next_dimension = stack_entry.NextDimension();
            stack.emplace_back(lower_node, next_dimension);
            stack.emplace_back(upper_node, next_dimension);
        }
    }

    const KdNode* Root() const { return nodes_.empty() ? nullptr : &nodes_.front(); }

    const T& GetMin() const { return min_; }
    const T& GetMax() const { return max_; }

private:
    template <typename _UINT> 
    _UINT RoundToPower2(_UINT n) const
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

    template <typename _Func> 
    T ComputeBound(const std::vector<T>& entries, const KdRange& range, _Func func) const
    {
        const auto num_entries = range.Size();
        if (num_entries < 1)
        {
            return T{};
        }

        T bound_entry = entries[range.beg];
        for (auto i = range.beg + 1; i < range.end; ++i)
        {
            bound_entry = func(bound_entry, entries[i]);
        }

        return bound_entry;
    }

    template <typename... _Args> 
    KdNode* AppendNode(_Args&&... args)
    {
        nodes_.emplace_back(args...);
        return &nodes_.back();
    }

    T min_;
    T max_;

    std::vector<KdNode> nodes_;
    std::vector<KdIndex> index_lookup_;
};

#endif // KDTREE_H
