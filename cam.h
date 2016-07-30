#ifndef CAM_H
#define CAM_H

#include <ostream>

#include "chunk.h"
#include "functions.h"

namespace cam {

struct CAM {
public:
    using WeightMap = Graph::EdgeMap<double>;
    using IdxMap = Graph::NodeMap<int>;

public:
    CAM(std::string const& function) 
        : g_{make_unique<Graph>()}, idx_{make_unique<IdxMap>(*g_)}, w_{make_unique<WeightMap>(*g_)}, c_{}, t_{}, mu_{function} {
    }

    auto g() const -> Graph const& {
        return *g_;
    }

    auto g() -> Graph& {
        return *g_;
    }

    auto w() const -> WeightMap const& {
        return *w_;
    }

    auto c() const -> vector<Chunk> const& {
        return c_;
    }
    
    auto t() const -> Graph::Node {
        return t_;
    }

    auto idx(Graph::Node node) const -> int {
        return (*idx_)[node];
    }

    auto mu() const -> Function {
        return mu_;
    }
    
private:    
    friend std::istream& operator>>(std::istream& in, CAM& cam);

private:
    unique_ptr<Graph> g_;
    unique_ptr<IdxMap> idx_;
    unique_ptr<WeightMap> w_;
    vector<Chunk> c_;
    Graph::Node t_;
    Function mu_;
};

template<class T>
inline std::ostream& operator<<(std::ostream& out, CAM const& cam) {
    auto num_nodes = 0;
    vector<tuple<int, int, double>> edges{};
    for (Graph::EdgeIt edge(cam.g()); edge != INVALID; ++edge) {
        edges.emplace_back(
            cam.idx(cam.g().u(edge)),
            cam.idx(cam.g().v(edge)),
            cam.w()[edge]
        );
    }

    out << num_nodes << " " << edges.size() << "\n";

    for (auto edge : edges) {
        out << get<0>(edge) << " " << get<1>(edge) << " " << get<2>(edge) << "\n";
    }

    out << cam.c().size() << "\n";
    for (auto chunk : cam.c()) {
        out << cam.idx(chunk.v()) << " " << chunk.size() << "\n";
    }
    out << cam.idx(cam.t()) << "\n";

    return out;
}

inline std::istream& operator>>(std::istream& in, CAM& cam) {
    int num_nodes, num_edges;
    in >> num_nodes >> num_edges;
    cam.g_->clear();
    vector<Graph::Node> nodes{};
    for (auto i = 0; i < num_nodes; i++) {
        auto node = cam.g_->addNode();
        nodes.emplace_back(node);
        cam.idx_->set(node, i);
    }

    for (auto i = 0; i < num_edges; i++) {
        int u, v; double w;
        in >> u >> v >> w;
        auto e = cam.g_->addEdge(nodes[u], nodes[v]);
        cam.w_->set(e, w);
    }

    int num_chunks;
    in >> num_chunks;
    cam.c_.clear();
    for (auto i = 0; i < num_chunks; i++) {
        int cu; double cw;
        in >> cu >> cw;
        cam.c_.emplace_back(nodes[cu], cw);
    }

    int t; 
    in >> t;
    cam.t_ = nodes[t];
    return in;
}

}

#endif //CAM_H
