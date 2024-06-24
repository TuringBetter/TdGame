#include "ResourceManager.h"

const ResourceManager::FontPool& ResourceManager::get_font_pool() const
{
    return font_pool;
    // TODO: 在此处插入 return 语句
}

const ResourceManager::SoundPool& ResourceManager::get_sound_pool() const
{
    return sound_pool;
    // TODO: 在此处插入 return 语句
}

const ResourceManager::MusicPool& ResourceManager::get_music_pool() const
{
    return music_pool;
    // TODO: 在此处插入 return 语句
}

const ResourceManager::TexturePool& ResourceManager::get_texture_pool() const
{
    return texture_pool;
    // TODO: 在此处插入 return 语句
}
