#ifndef COMMON_H
#define COMMON_H

#include <cassert>
#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <iostream>

#include <lemon/list_graph.h>

namespace cam {

using std::vector;
using std::tuple;
using std::get;
using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::make_tuple;
using std::make_shared;

using Graph = lemon::ListGraph;
using lemon::INVALID;

static size_t const NOT_SET = static_cast<size_t>(-1);

}

#endif // COMMON_H
