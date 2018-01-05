//
// Created by tomer on 1/5/18.
//

#ifndef KNN_NEARESTS_H
#define KNN_NEARESTS_H

#include <vector>
#include <CGAL/constructions_d.h>

template<typename Kernel>
class Nearests {
public:
    typedef typename Kernel::Point_d Point_d;
    typedef typename Kernel::FT FT;

    struct Neighbor {
        Point_d m_point;
        FT m_sq_distance;

        Neighbor(const Point_d &point, const FT &sq_distance) : m_point(point), m_sq_distance(sq_distance) {}

        // compare by distance and break ties by lexicographic order
        bool operator<(const Neighbor &rhs) const;
    };

    // input: const reference to a d dimensional vector which represent a d-point.
    // output: a vector of the indexes of the k-nearest-neighbors points
    Nearests(const Point_d &origin, size_t k);

    // input: possible neighbor
    // output: whether the candidate is one of the k neighbors of the origin point
    // holds a max-heap to save the neighbors
    bool add_candidate(const Point_d &point);

    Point_d &get_origin() const;

    size_t size() const;

    size_t k() const;

    // output: the top of the heap == the furthest neighbor
    Neighbor &heap_max() const;

    // output: a vector of the indexes of the k-nearest-neighbors points
    template<typename OutputIterator>
    OutputIterator get_neighbors(OutputIterator oi) const;

private:
    Point_d m_origin;
    size_t m_k;
    std::vector<Neighbor> m_neighbors;


};

template<typename Kernel>
bool Nearests<Kernel>::Neighbor::operator<(const Neighbor &rhs) const {
    return m_sq_distance < rhs.m_sq_distance ? m_sq_distance < rhs.m_sq_distance : m_point < rhs.m_point;
}

template<typename Kernel>
Nearests<Kernel>::Nearests(const Point_d &origin, size_t k) : m_origin(origin), m_k(k), m_neighbors() {
    m_neighbors.reserve(k);
}

template<typename Kernel>
bool Nearests<Kernel>::add_candidate(const Point_d &point) {
    auto candidate = Neighbor(point, CGAL::squared_distance(m_origin, point));
    if (m_neighbors.size() < m_k) {
        m_neighbors.push_back(candidate);
        std::push_heap(m_neighbors.begin(), m_neighbors.end());
        return true;
    } else {
        // size == k
        // check if the max is worse than the candidate
        if (m_neighbors.front().m_sq_distance > candidate.m_sq_distance) {
            // pop the worst neighbor and replace it
            auto end = m_neighbors.end();
            std::pop_heap(m_neighbors.begin(), m_neighbors.end());
            m_neighbors[m_k - 1] = candidate;
            std::make_heap(m_neighbors.begin(), m_neighbors.end());
            return true;
        }
    }
    return false;
}

template<typename Kernel>
Point_d &Nearests<Kernel>::get_origin() const {
    return m_origin;
}

template<typename Kernel>
size_t Nearests<Kernel>::size() const {
    return m_neighbors.size();
}

template<typename Kernel>
size_t Nearests<Kernel>::k() const {
    return m_k;
}

template<typename Kernel>
Neighbor &Nearests<Kernel>::heap_max() const {
    return m_neighbors.front();
}

template<typename Kernel>
template<typename OutputIterator>
OutputIterator Nearests<Kernel>::get_neighbors(OutputIterator oi) const {
    for (const auto neighbor: m_neighbors) {
        *oi++ = neighbor.m_point;
    }
    return oi;

}


#endif //KNN_NEARESTS_H
