// Copyright (c) the Dviglo project
// License: MIT

#pragma once

#include "texture.hpp"

#include <unordered_map>


namespace dviglo
{

class TextureCache
{
private:
    // Инициализируется в конструкторе
    inline static TextureCache* instance_ = nullptr;

    std::unordered_map<StrUtf8, Texture*> umap_storage_;
    std::vector<Texture*> vec_storage_;

public:
    static TextureCache* instance() { return instance_; }

    TextureCache();
    ~TextureCache();

    Texture* get(const StrUtf8& file_path);
    void add(Texture* texture);
};

#define DV_TEXTURE_CACHE (dviglo::TextureCache::instance())

} // namespace dviglo
