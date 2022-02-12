#pragma once

#include <entt/entt.hpp>

#include <iostream>
#include <optional>
#include <vector>

struct Tile
{
    entt::entity entity = entt::null;
};

class TileMap
{
public:
    TileMap(int width, int height)
        : width(width), height(height), tiles(width * height) {}

    auto tile_at(int x, int y) -> Tile& { return tiles[y * width + x]; }
    auto tile_at(int x, int y) const -> const Tile& { return tiles[y * width + x]; }

    constexpr auto get_width() -> int { return width; }
    constexpr auto get_height() -> int { return height; }

private:
    int width, height;
    std::vector<Tile> tiles;
};