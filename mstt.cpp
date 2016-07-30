#include <vector>
#include <iterator>
#include <iostream>

#include <lemon/dijkstra.h>
#include <lemon/kruskal.h>
#include <lemon/edge_set.h>

#include "cam.h"
#include "rec.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    cam::CAM cam{argv[1]};

    std::cin >> cam;

    using Dijkstra = lemon::Dijkstra<cam::Graph, cam::Graph::EdgeMap<double>>;

    cam::Graph closure{};
    cam::Graph::NodeMap<cam::Graph::Node> to_orig{closure};
    cam::Graph::NodeMap<cam::Graph::Node> to_closure{cam.g(), cam::INVALID};

    auto cl_t = closure.addNode();
    to_closure[cam.t()] = cl_t;
    to_orig[cl_t] = cam.t();
    for (auto c : cam.c()) {
        if (to_closure[c.v()] == cam::INVALID) {
            auto cl_v = closure.addNode();
            to_closure[c.v()] = cl_v;
            to_orig[cl_v] = c.v();
        }
    }

    cam::Graph::NodeMap<std::shared_ptr<Dijkstra>> dij{cam.g()};
    cam::Graph::EdgeMap<double> w{closure};
    for (cam::Graph::NodeIt u(cam.g()); u != cam::INVALID; ++u) {
        if (to_closure[u] == cam::INVALID) {
            continue;
        }

        dij[u] = std::make_shared<Dijkstra>(cam.g(), cam.w());
        dij[u]->run(u);

        for (cam::Graph::NodeIt v(cam.g()); v != cam::INVALID; ++v) {
            if (to_closure[v] == cam::INVALID) {
                continue;
            }
            auto e = closure.addEdge(to_closure[u], to_closure[v]);
            w[e] = dij[u]->dist(v);
        }
    }

    {
        std::vector<cam::Graph::Edge> cl_tree{};
        lemon::kruskal(closure, w, std::back_inserter(cl_tree));

        cam::Graph::EdgeMap<bool> active_edges{cam.g()};

        for (auto e : cl_tree) {
            auto u = to_orig[closure.u(e)];
            auto v = to_orig[closure.v(e)];
            auto p = dij[u]->path(v);
            for (Dijkstra::Path::RevArcIt ep(p); ep != cam::INVALID; ++ep) {
                cam::Graph::Arc x = ep;
                active_edges[x] = true;
            }
        }

        for (cam::Graph::EdgeIt e(cam.g()); e != cam::INVALID; ++e) {
            if (!active_edges[e]) {
                cam.g().erase(e);
            }
        }
    }

    cam::Graph::EdgeMap<bool> tree{cam.g()};
    lemon::kruskal(cam.g(), cam.w(), tree);
    for (cam::Graph::EdgeIt e(cam.g()); e != cam::INVALID; ++e) {
        if (!tree[e]) {
            cam.g().erase(e);
        }
    }

    std::cout << cam::REC::solve(cam);
}
