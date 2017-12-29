#ifndef KNN_KD_TREE
#define KNN_KD_TREE
#include <memory>
namespace knn
{
template <typename Kernel, typename D>
class KDTree
{
    public:
    typedef typename Kernel::Point_d Point_d;
    class KDNode
    {
        private:
        unsigned int m_axis;
        double m_value;
        std::unique_ptr<KDNode> m_left;
        std::unique_ptr<KDNode> m_right;
        std::unique_ptr<Point_d> m_point;
    };
    private:
    std::unique_ptr<KDNode> root;
};
};
#endif