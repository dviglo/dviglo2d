// Copyright (c) the Dviglo project
// License: MIT

#include "texture_cache.hpp"

#include "../fs/log.hpp"

#include <cassert>


namespace dviglo
{

TextureCache::TextureCache()
{
    assert(!instance_);
    instance_ = this;

    DV_LOG->write_debug("TextureCache constructed");
}

TextureCache::~TextureCache()
{
    instance_ = nullptr;

    for (auto& it : umap_storage_)
        delete it.second;


    for (Texture* texture : vec_storage_)
        delete texture;

    umap_storage_.clear();
    vec_storage_.clear();

    DV_LOG->write_debug("TextureCache destructed");
}

Texture* TextureCache::get(const StrUtf8& file_path)
{
    auto it = umap_storage_.find(file_path);

    if (it != umap_storage_.end())
        return it->second;

    Texture* texture = new Texture(file_path);
    umap_storage_[file_path] = texture;

    return texture;
}

void TextureCache::add(Texture* texture)
{
    vec_storage_.push_back(texture);
}

} // namespace dviglo
