#include "Banner.h"

Banner::Banner()
{
    size_background = { 646,215 };
    size_foreground = { 1282,209 };

    timer_display.set_one_shot(true);
    timer_display.set_wait_time(5);
    timer_display.set_callback(
        [&]()
        {
            is_end_display = true;
        }
    );
}

void Banner::set_center_position(const Vector2& position)
{
    this->pos_center = position;
}

void Banner::on_update(double delta)
{
    timer_display.on_update(delta);

    static const ResourceManager::TexturePool& tex_pool
        = ResourceManager::GetInstance()->get_texture_pool();
    static const auto instance = ConfigManager::GetInstance();

    tex_background = tex_pool.find(ResID::Tex_UIGameOverBar)->second;
    tex_foreground = tex_pool.find(instance->is_game_win ? ResID::Tex_UIWinText : ResID::Tex_UILossText)->second;
}

void Banner::on_render(SDL_Renderer* render)
{
    static SDL_Rect rect_dst; 
    
    rect_dst.x = (int)(pos_center.x - size_background.x / 2); 
    rect_dst.y = (int)(pos_center.y - size_background.y / 2); 
    rect_dst.w = (int)size_background.x, rect_dst.h = (int)size_background.y; 

    SDL_RenderCopy(render, tex_background, nullptr, &rect_dst); 

    rect_dst.x = (int)(pos_center.x - size_foreground.x / 2); 
    rect_dst.y = (int)(pos_center.y - size_foreground.y / 2); 
    rect_dst.w = (int)size_foreground.x, rect_dst.h = (int)size_foreground.y;
    
    SDL_RenderCopy(render, tex_foreground, nullptr, &rect_dst);
}

bool Banner::check_end_display() const
{
    return is_end_display;
}
