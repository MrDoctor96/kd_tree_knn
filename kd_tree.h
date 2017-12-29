#ifndef KNN_KD_TREE
#define KNN_KD_TREE

#include "defs.h"
#include <memory>

template<typename Kernel>
class KDTree {
public:
    typedef typename Kernel::Point_d Point_d;
    struct KDNode;

    template<typename InputIterator>
    KDTree(size_t d, InputIterator begin_points, InputIterator end_points);

    bool insert(Point_d point);

    struct KDNode {
        KDNode(size_t axis)
                : m_axis(axis), m_left(nullptr), m_right(nullptr), m_point(nullptr) {}
        size_t m_axis;
        double m_value;
        std::unique_ptr <KDNode> m_left;
        std::unique_ptr <KDNode> m_right;
        std::unique_ptr <Point_d> m_point;
    };

private:
    size_t m_dimension;
    std::unique_ptr <KDNode> m_root;
};


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
}
#endif