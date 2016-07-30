#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"

namespace cam {

struct Chunk{
    Chunk() 
        : v_{}, size_{-1} {
    }

    Chunk(Graph::Node v, double size)
        : v_{v}, size_{size}{
    }

    auto v() const {
        return v_;
    }
    
    auto size() const {
        return size_;
    }

private:
    Graph::Node v_;
    double const size_;
};

} // namespace cam

#endif // CHUNK_H
