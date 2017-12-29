#include <fstream>
#include <sstream>
#include <ostream>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include "defs.h"
#include "knn.h"


Point_d read_point(size_t d, std::ifstream &is) {

    std::vector <Number_type> point_to_be;
    for (int i = 0; i < d; ++i) {
        Kernel::FT c;
        is >> c;
        point_to_be.push_back(c);
    }
    Point_d point(d, point_to_be.begin(), point_to_be.end());
    return point;
}

//argv: ./a.out dimension input_points_file query_points_file
int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Format: ./a.out dimension input_points_file query_points_file" << std::endl;
        return 1;
    }

    auto d = boost::lexical_cast<size_t>(argv[1]);

    const auto *build_filename = argv[2];
    std::ifstream build_is;
    build_is.open(build_filename);
    if (!build_is.is_open()) {
        std::cerr << "Failed to open " << build_filename << "!" << std::endl;
        return -1;
    }
    size_t bulidn;
    build_is >> bulidn;
    std::vector <Point_d> build_points;
    for (size_t i = 0; i < bulidn; ++i) {
        build_points.push_back(read_point(d, build_is));
    }

    const auto *test_filename = argv[3];
    std::ifstream test_is;
    test_is.open(test_filename);
    if (!test_is.is_open()) {
        std::cerr << "Failed to open " << test_filename << "!" << std::endl;
        return -1;
    }
    size_t testn;
    test_is >> testn;
    std::vector <Point_d> test_points;
    for (auto i = 0; i < testn; ++i) {
        test_points.push_back(read_point(d, test_is));
    }
    Knn<Kernel> knn(d, build_points.begin(), build_points.end());
    for (size_t k = 2; k < 10; ++k) {
        for (auto it = test_points.begin(); it != test_points.end(); ++it) {
            std::vector <Point_d> res;
            res.reserve(k);
            boost::timer timer;
            knn.find_points(k, *it, std::back_inserter(res));
            auto secs = timer.elapsed();
            std::cout << secs << " time" << std::endl;
            //our test to see if the results are right
        }
    }

    return 0;
}

