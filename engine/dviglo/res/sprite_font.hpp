// Copyright (c) the Dviglo project
// License: MIT

#pragma once

#include "../gl_utils/texture.hpp"
#include "../math/rect.hpp"
#include "../std_utils/string.hpp"

#include <limits>
#include <unordered_map>


namespace dviglo
{

struct Glyph
{
    IntRect rect; // Область текстуры с глифом
    glm::ivec2 offset{0, 0}; // Смещение от origin (левый верхний угол) при рендеринге
    i32 advance_x = 0; // Расстояние между origin (левый верхний угол) текущего глифа и origin следующего глифа при рендеринге
    i32 page = std::numeric_limits<i32>::max(); // Номер текстуры
};

struct FontSettings
{
    StrUtf8 src_path;
    i32 height; // В пикселях
    i32 blur_radius; // Радиус размытия
    glm::ivec2 texture_size;

    FontSettings(const StrUtf8& src_path, i32 height = 20, i32 blur_radius = 0,
                 glm::ivec2 texture_size = glm::ivec2(1024, 1024))
        : src_path(src_path)
        , height(height)
        , blur_radius(blur_radius)
        , texture_size(texture_size)
    {
    }
};

struct FontSettingsSimple : FontSettings
{
    u32 color; // 0xAABBGGRR

    FontSettingsSimple(const StrUtf8& src_path, i32 height = 20,
                       u32 color = 0xFFFFFFFF, i32 blur_radius = 0,
                       glm::ivec2 texture_size = glm::ivec2(1024, 1024))
        : FontSettings(src_path, height, blur_radius, texture_size)
        , color(color)
    {
    }
};

class SpriteFont
{
private:
    StrUtf8 face_; // Название исходного шрифта (из которого был сгенерирован растровый шрифт)
    i32 size_ = 0; // Размер исходного шрифта
    i32 line_height_ = 0; // Высота растрового шрифта
    std::vector<Texture*> textures_; // Текстурные атласы с символами (хранятся в TextureCache)
    std::unordered_map<c32, Glyph> glyphs_; // Кодовая позиция : изображение

public:
    SpriteFont(const SpriteFont&) = delete;
    SpriteFont& operator=(const SpriteFont&) = delete;

    SpriteFont(const StrUtf8& file_path);

    // Генерирует спрайтовый шрифт из ttf
    SpriteFont(const FontSettingsSimple& settings);

    Texture* texture(size_t index)
    {
        return textures_[index];
    }

    const Glyph& glyph(u32 code_point)
    {
        return glyphs_[code_point];
    }

    i32 line_height() const { return line_height_; }

    void save(const StrUtf8& file_path);
};

} // namespace dviglo
