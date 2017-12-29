#ifndef KNN_KD_TREE
#define KNN_KD_TREE

#include "defs.h"
#include <memory>

namespace knn {
    template<typename Kernel>
    class KDTree {
    public:
        typedef typename Kernel::Point_d Point_d;

        KDTree(size_t d)
                :m_dimension(d)
        {}
        class KDNode {
        public:

        private:
            unsigned int m_axis;
            double m_value;
            std::unique_ptr <KDNode> m_left;
            std::unique_ptr <KDNode> m_right;
            std::unique_ptr <Point_d> m_point;
        };

    private:
        size_t m_dimension;
        std::unique_ptr <KDNode> root;
    };
};
#endif