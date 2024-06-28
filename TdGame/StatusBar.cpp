#include "StatusBar.h"

void StatusBar::set_position(int x, int y)
{
	position.x = x;
	position.y = y;
}

void StatusBar::on_update(SDL_Renderer* render)
{
	static TTF_Font* font 
		= ResourceManager::GetInstance()->get_font_pool().find(ResID::Font_Main)->second;

	/*
	* 在每一帧都要生成一个文字纹理的操作
	* 则需要先清空上一帧的文字纹理
	*/
	SDL_DestroyTexture(tex_text_background);
	tex_text_background = nullptr;
	SDL_DestroyTexture(tex_text_foreground);
	tex_text_foreground = nullptr;

	std::string str_val = std::to_string((int)(CoinManager::GetInstance()->get_current_coin_num()));
	SDL_Surface* suf_text_background = TTF_RenderText_Blended(font, str_val.c_str(), color_text_background);
	SDL_Surface* suf_text_foreground = TTF_RenderText_Blended(font, str_val.c_str(), color_text_foreground);

	width_text = suf_text_background->w;
	height_text = suf_text_background->h;

	tex_text_background = SDL_CreateTextureFromSurface(render, suf_text_background);
	tex_text_foreground = SDL_CreateTextureFromSurface(render, suf_text_foreground);

	/* 生成的两个suf在渲染的时候是不需要的 */
	SDL_FreeSurface(suf_text_background);
	SDL_FreeSurface(suf_text_foreground);
}

void StatusBar::on_render(SDL_Renderer* render)
{
	static SDL_Rect rect_dst;
	static const ResourceManager::TexturePool& tex_pool 
		= ResourceManager::GetInstance()->get_texture_pool();
	static SDL_Texture* tex_coin = tex_pool.find(ResID::Tex_UICoin)->second;
	static SDL_Texture* tex_heart = tex_pool.find(ResID::Tex_UIHeart)->second;
	static SDL_Texture* tex_home_avatar = tex_pool.find(ResID::Tex_UIHomeAvatar)->second;
	static SDL_Texture* tex_player_avatar = tex_pool.find(ResID::Tex_UIPlayerAvatar)->second;

	rect_dst.x = position.x;
	rect_dst.y = position.y;
	rect_dst.w = 78;
	rect_dst.h = 78;
	SDL_RenderCopy(render, tex_home_avatar, nullptr, &rect_dst);

	int num_hp = (int)HomeManager::GetInstance()->get_current_hp();
	for (int i = 0; i < num_hp; i++)
	{
		/*
		* 渲染生命值心
		* 78：头像宽度
		* 15：头像与心的间隔
		* 32+2：心的宽度加上心与心的间隔
		*/
		rect_dst.x = position.x + 78 + 15 + i * (32 + 2);
		rect_dst.y = position.y;
		rect_dst.w = 32;
		rect_dst.h = 32;
		SDL_RenderCopy(render, tex_heart, nullptr, &rect_dst);
	}

	rect_dst.x = position.x + 78 + 15;
	rect_dst.y = position.y + 78 - 32;
	rect_dst.w = 32;
	rect_dst.h = 32;
	SDL_RenderCopy(render, tex_coin, nullptr, &rect_dst);

	rect_dst.x += 32 + 10 + offset_shadow.x;
	rect_dst.y += (32 - height_text) / 2 + offset_shadow.y;
	rect_dst.w = width_text;
	rect_dst.h = height_text;
	SDL_RenderCopy(render, tex_text_background, nullptr, &rect_dst);
	
	rect_dst.x -= offset_shadow.x;
	rect_dst.y -= offset_shadow.y;
	rect_dst.w = width_text;
	rect_dst.h = height_text;
	SDL_RenderCopy(render, tex_text_foreground, nullptr, &rect_dst);

	rect_dst.x = position.x + (78 - 65) / 2; 
	rect_dst.y = position.y + 78 + 5; 
	rect_dst.w = 65; rect_dst.h = 65; 
	SDL_RenderCopy(render, tex_player_avatar, nullptr, &rect_dst);

	rect_dst.x = position.x + 78 + 15; 
	rect_dst.y += 10; 
	roundedBoxRGBA(render, rect_dst.x, rect_dst.y, rect_dst.x + width_mp_bar, rect_dst.y + height_mp_bar, 4, 
		color_mp_bar_background.r, color_mp_bar_background.g, color_mp_bar_background.b, color_mp_bar_background.a);

	rect_dst.x += width_border_mp_bar; 
	rect_dst.y += width_border_mp_bar;
	rect_dst.w = width_mp_bar - 2 * width_border_mp_bar; 
	rect_dst.h = height_mp_bar - 2 * width_border_mp_bar; 
	double process = PlayerManager::GetInstance()->get_current_mp() / 100;
	roundedBoxRGBA(render, rect_dst.x, rect_dst.y, rect_dst.x + (int)(rect_dst.w*process), rect_dst.y + rect_dst.h, 2,
		color_mp_bar_foredground.r, color_mp_bar_foredground.g, color_mp_bar_foredground.b, color_mp_bar_foredground.a);
}
