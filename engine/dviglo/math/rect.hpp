// Copyright (c) the Dviglo project
// License: MIT

#pragma once

#include "../common/primitive_types.hpp"

#include <glm/glm.hpp>


namespace dviglo
{

struct Rect
{
    glm::vec2 pos;
    glm::vec2 size;

    Rect() = default;

    Rect(glm::vec2 pos, glm::vec2 size)
        : pos(pos)
        , size(size)
    {
    }

    Rect(f32 x, f32 y, f32 width, f32 height)
        : pos(x, y)
        , size(width, height)
    {
    }

    Rect operator+(const glm::vec2 offset) const { return Rect(pos + offset, size); }
    Rect operator-(const glm::vec2 offset) const { return Rect(pos - offset, size); }

    static const Rect zero;
};

inline const Rect Rect::zero(0.f, 0.f, 0.f, 0.f);


struct IntRect
{
    glm::ivec2 pos;
    glm::ivec2 size;

    IntRect() = default;

    IntRect(glm::ivec2 pos, glm::ivec2 size)
        : pos(pos)
        , size(size)
    {
    }

    IntRect(i32 x, i32 y, i32 width, i32 height)
        : pos(x, y)
        , size(width, height)
    {
    }

    IntRect operator+(const glm::ivec2 offset) const { return IntRect(pos + offset, size); }
    IntRect operator-(const glm::ivec2 offset) const { return IntRect(pos - offset, size); }

    static const IntRect zero;
};

inline const IntRect IntRect::zero(0, 0, 0, 0);


/// Axis-aligned bounding box
struct Aabb
{
    glm::vec2 min;
    glm::vec2 max; ///< Не включительно

    Aabb() = default;

    Aabb(glm::vec2 min, glm::vec2 max)
        : min(min)
        , max(max)
    {
    }

    Aabb(f32 min_x, f32 min_y, f32 max_x, f32 max_y)
        : min(min_x, min_y)
        , max(max_x, max_y)
    {
    }

    void merge(glm::vec2 point)
    {
        if (point.x < min.x)
            min.x = point.x;

        if (point.y < min.y)
            min.y = point.y;

        if (point.x > max.x)
            max.x = point.x;

        if (point.y > max.y)
            max.y = point.y;
    }

    void merge(const Aabb& other_aabb)
    {
        if (other_aabb.min.x < min.x)
            min.x = other_aabb.min.x;

        if (other_aabb.min.y < min.y)
            min.y = other_aabb.min.y;

        if (other_aabb.max.x > max.x)
            max.x = other_aabb.max.x;

        if (other_aabb.max.y > max.y)
            max.y = other_aabb.max.y;
    }

    Rect to_rect() const
    {
        return Rect(min, max - min);
    }
};

} // namespace dviglo
