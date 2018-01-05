#ifndef KNN_KD_TREE
#define KNN_KD_TREE

#include "defs.h"
#include <memory>
#include <iterator>
#include <algorithm>
template <typename Kernel>
class KDTree
{
  public:
    typedef typename Kernel::Point_d Point_d;
    typedef typename Kernel::FT NumberType;

    // input: d - the dimension
    // iterator to the input points
    // axis - the axis to split by
    template <typename InputIterator>
    KDTree(size_t d, InputIterator beginPoints, InputIterator endPoints, size_t axis = 0);

    // input: const reference to a d dimensional vector which represent a d-point.
    // output: a vector of the indexes of the k-nearest-neighbors points
    template <typename OutputIterator>
    OutputIterator find_points(size_t k, const Point_d &it, OutputIterator oi);
    
    bool is_leaf() const;
    void print(std::ostream& out = std::cout, size_t indent = 0) const;
  private:
    std::unique_ptr<KDTree<Kernel>> m_left;
    std::unique_ptr<KDTree<Kernel>> m_right;
    size_t m_dimension;
    size_t m_axis;
    NumberType m_split_value;
    Point_d m_point;
};

template <typename Kernel, typename InputIterator>
KDTree<Kernel>::KDTree(size_t d, InputIterator begin, InputIterator end, size_t axis = 0)
: m_left(nullptr), m_right(nullptr), m_dimension(d), m_axis(axis){
    size_t points_size = std::distance(begin, end);
    if (points_size == 1) {
        // leaf!
        m_point = *begin;
    } else {
        // mid point - need to pivot
        auto pivot = find_median_point(begin, end, points_size);
        m_split_value = *pivot[m_axis];
        // go deeper
        auto next_axis = (m_axis + 1) % m_dimension;
        if (pivot != begin) {
            m_left = std::make_unique<KDTree<Kernel>>(d, begin, pivot, next_axis);
        }
        if (pivot != end) {
            m_right = std::make_unique<KDTree<Kernel>>(d, pivot, end, next_axis);
        }

    }
}

template <typename Kernel, typename InputIterator>
InputIterator KDTree<Kernel>::find_median_point(InputIterator& begin, InputIterator& end, size_t length) {
    auto median = begin + length / 2;
    std::nth_element(begin, median, end, [&](Point_d p1, Point_d p2) {return p1[m_axis] < p2[m_axis];});
    auto median_axis_value = (*median)[m_axis];
    auto split = std::partition(begin, end, [&median_axis_value](auto p) {return p[m_axis] <= median_axis_value;});
    // TODO: handle degenerate cases
    return split;

}

template <typename Kernel, typename OutputIterator>
OutputIterator KDTree<Kernel>::find_points(size_t k, const Point_d &it, OutputIterator oi) {
    return oi;
}



template <typename Kernel>
bool KDTree<Kernel>::is_leaf() const {
    return m_left == nullptr && m_right == nullptr;
}

// helper function - prints the tree in a json format for debugging
template <typename Kernel>
void KDTree<Kernel>::print(std::ostream& out = std::cout, size_t indent = 0) const{
    auto space = std::string(indent, ' ');
    out << space << "{" << std::endl;
    if (is_leaf()) {
        out << space << "leaf: " << m_point << std::endl;
    } else {
        cout << space << "split node: { axis: " << m_axis << ", value: " << m_split_value << "}"<< std::endl;
        cout << space << "left:" << std::endl;
        m_left->print(out, indent + 2);
        cout << space << "right:" << std::endl;
        m_right->print(out, indent + 2);
    }
    out << space << "}" << std::endl;

}
/*
template<typename InputIterator>
KDTree::KDTree(size_t d, InputIterator begin_points, InputIterator end_points)
: m_dimension(d), m_root(nullptr) {
    size_t axis = 0;
    // if only 1 point
    if (begin_points == end_points) {
        m_root = std::make_unique(new KDNode(axis++));
    }
    for (auto it = begin_points; it != end_points; ++it) {
        Point_d point = *it;

    }
}*/
#endif
