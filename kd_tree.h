//
// Created by Tomer on 12/28/2017.
//

#ifndef KNN_KD_TREE_H
#define KNN_KD_TREE_H

#include <memory>
#include <iterator>
#include <algorithm>
#include <iostream>
#include "nearests.h"

//template <typename InputIterator>
//void print_range(InputIterator begin, InputIterator end) {
//    std::cout << "[";
//    for (auto it = begin; it != end; ++it) {
//        if (it != begin) {
//            std::cout << ", ";
//        }
//        std::cout << *it;
//    }
//    std::cout << "]" << std::endl;
//}

template<typename Kernel>
class KDTree {
public:
    typedef typename Kernel::Point_d Point_d;
    typedef typename Kernel::FT FT;

    // input: d - the dimension
    // iterator to the input points
    // axis - the axis to split by
    template<typename InputIterator>
    KDTree(size_t d, InputIterator begin, InputIterator end, size_t axis = 0);

    // input: const reference to a class whose origin is the query point and that will store the k-nearest-neighbors points
    void find_points(Nearests<Kernel> &nearest_neighbors) const;

    bool is_leaf() const;

    void print(std::ostream &out = std::cout, size_t indent = 0) const;

private:
    template<typename InputIterator>
    InputIterator find_median_point(InputIterator begin, InputIterator end, size_t length) const;

    std::unique_ptr<KDTree<Kernel>> m_left;
    std::unique_ptr<KDTree<Kernel>> m_right;
    size_t m_dimension;
    size_t m_axis;
    FT m_split_value;
    Point_d m_point;
};

template<typename Kernel>
template<typename InputIterator>
KDTree<Kernel>::KDTree(size_t d, InputIterator begin, InputIterator end, size_t axis/* = 0*/)
        : m_left(nullptr), m_right(nullptr), m_dimension(d), m_axis(axis) {
    size_t points_size = std::distance(begin, end);
    // points_size < 0 ?!
    //std::cout << "axis " << m_axis << " with " << points_size << "points." << std::endl;
    //print_range(begin, end);
    if (points_size == 1) {
        // leaf!
        m_point = *begin;
        std::cout << "leaf :" << m_point << std::endl;
    } else {
        // mid point - need to pivot
        auto pivot = find_median_point(begin, end, points_size);
        m_split_value = (*pivot)[m_axis];
        //std::cout << "split value :" << m_split_value << std::endl;
        // go deeper
        auto next_axis = (m_axis + 1) % m_dimension;
        if (pivot != begin) {
            m_left.reset(new KDTree<Kernel>(d, begin, pivot, next_axis));
        }
        if (pivot != end) {
            m_right.reset(new KDTree<Kernel>(d, pivot, end, next_axis));
        }

    }
}

template<typename Kernel>
template<typename InputIterator>
InputIterator KDTree<Kernel>::find_median_point(InputIterator begin, InputIterator end, size_t length) const {
    auto median = begin + length / 2;
    //print_range(begin, end);
    auto compare_by_axis = [&](Point_d p1, Point_d p2) {
        return p1[m_axis] < p2[m_axis];
    };
    std::nth_element(begin, median, end, compare_by_axis);
    //print_range(begin, end);
    auto median_axis_value = (*median)[m_axis];
    if (length % 2 == 0) {
        // even number of elements, find the previous and do average
        std::nth_element(begin, median - 1, end, compare_by_axis);
        median_axis_value += (*(median - 1))[m_axis];
        median_axis_value /= FT(2);
    }
    //std::cout << "median point: "<< *median << " value: " << median_axis_value << std::endl;
    auto split = std::partition(begin, end, [&](Point_d p) { return p[m_axis] <= median_axis_value; });
    // TODO: handle degenerate cases?
    return split;

}

template<typename Kernel>
void KDTree<Kernel>::find_points(Nearests<Kernel> &nearest_neighbors) const {
    if (is_leaf()) {
        // add this point
        nearest_neighbors.add_candidate(m_point);
        return;
    }
    Point_d query_point = nearest_neighbors.get_origin();
    FT distance = query_point[m_axis] - m_split_value;
    KDTree<Kernel> *search_path = nullptr;
    KDTree<Kernel> *other_path = nullptr;
    if (distance <= 0) {
        search_path = m_left.get();
        other_path = m_right.get();
    } else {
        search_path = m_right.get();
        other_path = m_left.get();
    }
    search_path->find_points(nearest_neighbors);
    // should we go examine the other_path?
    // only if not enough neighbors or the furthest neighbor radius intersects the other region
    if (nearest_neighbors.size() < nearest_neighbors.k() ||
        distance * distance <= nearest_neighbors.heap_max().m_sq_distance) {
        other_path->find_points(nearest_neighbors);
    }
}


template<typename Kernel>
bool KDTree<Kernel>::is_leaf() const {
    return m_left == nullptr && m_right == nullptr;
}

// helper function - prints the tree in a json format for debugging
template<typename Kernel>
void KDTree<Kernel>::print(std::ostream &out/* = std::cout*/, size_t indent/* = 0*/) const {
    auto space = std::string(indent, ' ');
    out << space << "{" << std::endl;
    if (is_leaf()) {
        out << space << "leaf: " << m_point << std::endl;
    } else {
        out << space << "split node: { axis: " << m_axis << ", value: " << m_split_value << "}" << std::endl;
        out << space << "left:" << std::endl;
        m_left->print(out, indent + 2);
        out << space << "right:" << std::endl;
        m_right->print(out, indent + 2);
    }
    out << space << "}" << std::endl;

}

#endif
