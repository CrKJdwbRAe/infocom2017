#include <lemon/kruskal.h>

#include "cam.h"
#include "rec.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    cam::CAM cam{argv[1]};

    std::cin >> cam;

    cam::Graph::EdgeMap<bool> tree{cam.g()};
    lemon::kruskal(cam.g(), cam.w(), tree);
    for (cam::Graph::EdgeIt e(cam.g()); e != cam::INVALID; ++e) {
        if (!tree[e]) {
            cam.g().erase(e);
        }
    }

    std::cout << cam::REC::solve(cam);
}

