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
    // input: iterators for the points
    // output: an iterator that points to median point, which is guaranteed to hold begin >= it > end.
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
    auto points_size = static_cast<size_t>(std::distance(begin, end));
    if (points_size == 1) {
        // leaf!
        m_point = *begin;
    } else {
        // mid point - need to pivot
        auto pivot = find_median_point(begin, end, points_size);
        // find_median_point returns a valid point.
        m_split_value = (*pivot)[m_axis];
        // continue splitting in the next axis
        auto next_axis = (m_axis + 1) % m_dimension;
        auto left_size = static_cast<size_t>(std::distance(begin, pivot));
        if (left_size > 0) {
            m_left.reset(new KDTree<Kernel>(d, begin, pivot, next_axis));
        }
        auto right_size = static_cast<size_t>(std::distance(pivot, end));
        if (right_size > 0) {
            m_right.reset(new KDTree<Kernel>(d, pivot, end, next_axis));
        }


    }
}

template<typename Kernel>
template<typename InputIterator>
InputIterator KDTree<Kernel>::find_median_point(InputIterator begin, InputIterator end, size_t length) const {
    auto median_ptr = begin + (length - 1) / 2;
    auto compare_by_axis = [&](Point_d p1, Point_d p2) {
        return p1[m_axis] < p2[m_axis];
    };
    std::nth_element(begin, median_ptr, end, compare_by_axis);
    auto median_axis_value = (*median_ptr)[m_axis];
    auto split = std::partition(begin, end, [&](Point_d p) { return p[m_axis] <= median_axis_value; });
    // edge cases - the split node is one of the edges, need to choose an inner point.
    if (split == end) {
        // no elements are greater than the split.
        // find the lower_bound and return their median.
        auto bound = std::lower_bound(begin, end, median_axis_value, [&](Point_d p, FT value) {return p[m_axis] < value;} );
        auto dist = static_cast<size_t>(std::distance(bound, split));
        if (dist > 0) {
            split = bound + ((dist - 1) / 2);
        }
    } else if (split == begin) {
        // no elements are smaller than the split.
        // find the upper_bound and return their median.
        auto bound = std::upper_bound(begin, end, median_axis_value, [&](FT value, Point_d p) {return value < p[m_axis];} );
        auto dist = static_cast<size_t>(std::distance(split, bound));
        if (dist > 0) {
            split = split + ((dist - 1) / 2);
        }
    }
    return split;

}

template<typename Kernel>
void KDTree<Kernel>::find_points(Nearests<Kernel> &nearest_neighbors) const {
    if (is_leaf()) {
        // add this point
        nearest_neighbors.add_candidate(m_point);
        return;
    }
    const Point_d &query_point = nearest_neighbors.get_origin();
    FT distance = query_point[m_axis] - m_split_value;
    KDTree<Kernel> *search_path = nullptr;
    KDTree<Kernel> *other_path = nullptr;
    // choose obvious search path and possible pruned path
    if (distance <= 0) {
        search_path = m_left.get();
        other_path = m_right.get();
    } else {
        search_path = m_right.get();
        other_path = m_left.get();
    }
    if (search_path != nullptr) {
        search_path->find_points(nearest_neighbors);
    }
    // should we go examine the other_path?
    // only if not enough neighbors or the furthest neighbor radius intersects the other region
    if (other_path != nullptr && (nearest_neighbors.size() < nearest_neighbors.k() ||
                                  distance * distance <= nearest_neighbors.heap_max().m_sq_distance)) {
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
        out << " " << space << "leaf: " << m_point << std::endl;
    } else {
        out << " " << space << "split node: { axis: " << m_axis << ", value: " << m_split_value << " }" << std::endl;
        out <<  " " << space << "left:" << std::endl;
        if (m_left != nullptr) {
            m_left->print(out, indent + 2);
        }
        out <<  " " << space << "right:" << std::endl;
        if (m_right != nullptr) {
            m_right->print(out, indent + 2);
        }
    }
    out << space << "}" << std::endl;

}

#endif
