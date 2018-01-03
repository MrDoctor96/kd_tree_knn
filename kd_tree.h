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

    template <typename InputIterator>
    KDTree(size_t d, InputIterator beginPoints, InputIterator endPoints, size_t axis = 0);

    template <typename OutputIterator>
    OutputIterator find_points(size_t k, const Point_d &it, OutputIterator oi);

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
    size_t length = std::distance(begin, end);
    if (length == 1) {
        // leaf!
        m_point = *begin;
    } else {
        // mid point - need to pivot
        auto pivot = find_median_pivot(begin, end, length);
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

    size_t median_index = points.size() / 2;
    std::vector<NumberType> projected_axis;
        std::transform(points.begin(), points.end(), std::back_inserter(projected_axis),
                       [](Point_d p) -> NumberType { return p.get<axis>(); });
        NumberType split_value = std::nth_element(projected_axis.begin(), projected_axis.begin() + median_index, projected_axis.end());
        auto smaller = std::find_if();
        // TODO:
        

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
