#include <lemon/dijkstra.h>

#include "cam.h"
#include "rec.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    cam::CAM cam{argv[1]};

    std::cin >> cam;

    cam::Graph::EdgeMap<bool> tree{cam.g()};
    cam::Graph::NodeMap<cam::Graph::Edge> pred{cam.g()};
    lemon::dijkstra(cam.g(), cam.w()).predMap(pred).run(cam.t());
    for (cam::Graph::NodeIt n(cam.g()); n != cam::INVALID; ++n) {
        if (pred[n] != cam::INVALID) {
            tree[pred[n]] = true;
        }
    }
    for (cam::Graph::EdgeIt e(cam.g()); e != cam::INVALID; ++e) {
        if (!tree[e]) {
            cam.g().erase(e);
        }
    }

    std::cout << cam::REC::solve(cam);
}
