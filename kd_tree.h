#ifndef KNN_KD_TREE
#define KNN_KD_TREE

#include "defs.h"
#include <memory>
#include <iterator>
#include <algorithm>
template<typename Kernel>
class KDTree {
public:
    typedef typename Kernel::Point_d Point_d;
    typedef typename Kernel::FT NumberType;
    struct KDNode;


    KDTree(size_t d, std::vector<Point_d> points);

    ~KDTree();

    bool insert(Point_d point);

    struct KDNode {
        KDNode(size_t axis)
                : m_axis(axis), m_left(nullptr), m_right(nullptr), m_point(nullptr) {}

        ~KDNode() = default;

        size_t m_axis = 0;
        NumberType m_value = 0;
        std::unique_ptr<KDNode> m_left {nullptr};
        std::unique_ptr<KDNode> m_right {nullptr};
        std::shared_ptr<Point_d> m_point {nullptr};
    };

private:
    size_t m_dimension = 0;
    std::unique_ptr<KDNode> m_root {nullptr};
};

template<typename Kernel>
KDTree<Kernel>::KDTree(size_t d, std::vector<Point_d> points) {
    if (points.size() < 1) {
        return;
    }
    if (points.size() == 1) {
        m_root = new KDNode(0);
        m_root->m_point = points[0];
    } else {
        size_t axis = depth % d;
        size_t median_index = points.size() / 2;
        std::vector<NumberType > projected_axis ;
        std::transform(points.begin(), points.end(), std::back_inserter(projected_axis),
        [](Point_d p) -> NumberType {return p.get<axis>(); });
        NumberType split_value = std::nth_element(projected_axis.begin(), projected_axis.begin() + median_index, projected_axis.end());
        auto smaller = std::find_if()
    }
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
