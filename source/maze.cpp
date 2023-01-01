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
}

MazeObject& Maze::get_cell(const Node& node) {
    return maze[node.y * width + node.x];
}

std::vector<Maze::Node> Maze::get_neighboors(const Node& node) {
    auto [x, y] = node;
    std::array nodes_to_check = {
        Node{x + 1, y},
        Node{x, y + 1},
        Node{x - 1, y},
        Node{x, y - 1}
    };
    std::vector<Node> res(nodes_to_check.size());

    rng::copy_if(nodes_to_check, res.begin(), [&](const Node& node) {
        return node.x < width && node.y < width;
    });

    return res;
}
