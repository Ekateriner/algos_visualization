#include "util.hpp"
#include <iterator>
#include <maze.hpp>
#include <random_utils.hpp>

#include <ranges>
#include <algorithm>

namespace rng = std::ranges;


Maze::Maze(size_t width, size_t height, double wall_prob)
    : width(width)
    , height(height)
    , wall_prob(wall_prob)
    , maze(height * width, MazeObject::space) 
{
    auto& rengine = get_rengine();
    std::uniform_real_distribution<double> distr(0, 1); // 0 - wall, others - empty
    rng::generate(maze, [&]{ return distr(rengine) < wall_prob 
            ? MazeObject::wall 
            : MazeObject::space; });

    // clean up
    for (size_t h = 1; h < height - 1; ++h) {
        for (size_t w = 1; w < width - 1; ++w) {
            if (get_cell({w, h}) != MazeObject::wall ) {
                continue;
            }
            std::array nodes_to_check = {
                Node{w + 1, h},
                Node{w, h + 1},
                Node{w - 1, h},
                Node{w, h - 1}
            };
            if (rng::all_of(nodes_to_check, [&](const auto& node) { return get_cell(node) != MazeObject::wall; })) {
                get_cell({w, h}) = MazeObject::space;
            }
        }
    }
}

MazeObject& Maze::get_cell(const Node& node) {
    auto idx = util::coords_to_idx(node.x, node.y, width);
    return maze[idx];
}

std::vector<Maze::Node> Maze::get_neighboors(const Node& node) {
    auto [x, y] = node;
    std::array nodes_to_check = {
        Node{x + 1, y},
        Node{x, y + 1},
        Node{x - 1, y},
        Node{x, y - 1}
    };
    std::vector<Node> res;
    res.reserve(nodes_to_check.size());

    rng::copy_if(nodes_to_check, std::back_inserter(res), [&](const Node& node) {
        bool valid = node.x < width && node.y < height;
        if (valid) {
            auto index = util::coords_to_idx(node.x, node.y, width);
            return maze[index] != MazeObject::wall;
        }
        return false;
    });

    return res;
}
