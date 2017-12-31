//
// Created by Tomer on 12/28/2017.
//

#ifndef KD_TREE_KNN_KNN_H
#define KD_TREE_KNN_KNN_H

#include <vector>
#include "kd_tree.h"
template <typename Kernel>
class Knn {
public:
    typedef typename Kernel::Point_d Point_d;
    // input: d - the dimension
    // iterator to the input points
    template <typename InputIterator>
            Knn(size_t d, InputIterator beginPoints, InputIterator endPoints)
	    :m_tree(d)
    {
        std::vector<Point_d> points = std::vector(beginPoints, endPoints);
        // your build code
    }

    // input: const reference to a d dimensional vector which represent a d-point.
    // output: a vector of the indexes of the k-nearest-neighbors points
    template <typename OutputIterator>
    OutputIterator find_points(size_t k,const Point_d &it,OutputIterator oi)
    {
        //your query code
        return oi;
    }

private:
    KDTree<Kernel> m_tree;
};

#endif //KD_TREE_KNN_KNN_H
