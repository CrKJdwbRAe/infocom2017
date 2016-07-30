#ifndef CHUNK_SET_H
#define CHUNK_SET_H

#include "chunk.h"
#include "cam.h"
#include "actions.h"

namespace cam {

struct ChunkSet {
    ChunkSet(CAM const& cam) : mu_{cam.mu()}, chunks_{}, idx_{0}, t_{cam.t()} {
        for (auto c : cam.c()) {
            chunks_.emplace(idx_++, c);
        }
    }

    auto chunks() const -> unordered_map<size_t, Chunk> const& {
        return chunks_;
    }

    auto chunk(size_t idx) const -> Chunk {
        return chunks_.at(idx);
    }

    size_t apply(CAMAction const& action) {
        if (action.is_finish()) {
            auto x = chunks_.at(action.x());
            chunks_.erase(action.x());
            chunks_.emplace(idx_, Chunk(t_, x.size()));
        } else {
            auto x = chunks_.at(action.x());
            chunks_.erase(action.x());
            auto y = chunks_.at(action.y());
            chunks_.erase(action.y());
            chunks_.emplace(idx_, Chunk(action.v(), mu_(x.size(), y.size())));
        }
        return idx_++;
    }

private: 
    Function mu_;
    unordered_map<size_t, Chunk> chunks_;
    size_t idx_;
    Graph::Node t_;
};

}

#endif // CHUNK_SET_H
