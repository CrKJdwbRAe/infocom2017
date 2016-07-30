#ifndef REC_H
#define REC_H

#include "cam.h"
#include "solution.h"
#include "chunk_set.h"

namespace cam {

struct REC {
public:
    static auto solve(CAM const& cam) -> Solution {
        REC rec{cam};
        auto finish = rec.solve(cam.t(), INVALID);
        rec.solution_.add_action(CAMAction(finish));
        return rec.solution_;
    }

private:
    REC(CAM const& cam) 
        : cam_{cam}, chunks_{cam}, solution_{cam}, initial_chunks_{cam.g()} {
        for (auto c : chunks_.chunks()) {
            initial_chunks_[c.second.v()].emplace_back(c.first);
        }
    }

    size_t solve(Graph::Node n, Graph::Node parent) {
        auto merged_here = initial_chunks_[n];
        for (Graph::IncEdgeIt e(cam_.g(), n); e != INVALID; ++e) {
            if (cam_.g().runningNode(e) != parent) {
                auto sub_root = solve(cam_.g().runningNode(e), n);
                if (sub_root != NOT_SET) {
                    merged_here.emplace_back(sub_root);
                }
            }
        }
        if (merged_here.empty()) {
            return NOT_SET;
        }
        auto result = merged_here[0];
        for (auto i = 1u; i < merged_here.size(); i++) {
            auto action = CAMAction(result, merged_here[i], n); 
            solution_.add_action(action);
            result = chunks_.apply(action);
        }

        return result;
    }

private:
    CAM const& cam_;

    ChunkSet chunks_;
    Solution solution_;

    Graph::NodeMap<vector<size_t>> initial_chunks_;
};

}

#endif // REC_H
