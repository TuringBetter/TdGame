#include "Panel.h"

Panel::Panel()
{
	tex_select_cursor = ResourceManager::GetInstance()->get_texture_pool().find(ResID::Tex_UISelectCursor)->second;
}

Panel::~Panel()
{
	SDL_DestroyTexture(tex_text_background);
	SDL_DestroyTexture(tex_text_foreground);
}

void Panel::show()
{
	is_visible = true;
}

void Panel::set_idx_tile(SDL_Point& idx)
{
	idx_tile_selected = idx;
}

void Panel::set_center_pos(SDL_Point& pos)
{
	center_pos = pos;
}

void Panel::on_input(const SDL_Event& event)
{
	if (!is_visible) return;

	switch (event.type)
	{
	case SDL_MOUSEMOTION:
	{
		SDL_Point pos_cursor = { event.motion.x,event.motion.y };
		SDL_Rect rect_target = { 0,0,size_button,size_button };

		rect_target.x = center_pos.x - width / 2 + offset_top.x;
		rect_target.y = center_pos.y - width / 2 + offset_top.y;
		if(SDL_PointInRect(&pos_cursor,&rect_target))
		{
			hovered_target = HoveredTarget::Top;
			return;
		}

		rect_target.x = center_pos.x - width / 2 + offset_left.x;
		rect_target.y = center_pos.y - width / 2 + offset_left.y;
		if (SDL_PointInRect(&pos_cursor, &rect_target))
		{
			hovered_target = HoveredTarget::Left;
			return;
		}

		rect_target.x = center_pos.x - width / 2 + offset_right.x;
		rect_target.y = center_pos.y - width / 2 + offset_right.y;
		if (SDL_PointInRect(&pos_cursor, &rect_target))
		{
			hovered_target = HoveredTarget::Right;
			return;
		}

		hovered_target = HoveredTarget::None;
	}
		break;
	case SDL_MOUSEBUTTONUP:
	{
		switch (hovered_target)
		{
		case Panel::HoveredTarget::Top:
			on_click_top_area();
			break;
		case Panel::HoveredTarget::Left:
			on_click_left_area();
			break;
		case Panel::HoveredTarget::Right:
			on_click_right_area();
			break;
		}
		is_visible = false;
	}
		break;
	default:
		break;
	}
}

void Panel::on_update(SDL_Renderer* render)
{
	static TTF_Font* font
		= ResourceManager::GetInstance()->get_font_pool().find(ResID::Font_Main)->second;

	if (hovered_target == HoveredTarget::None) return;

	int val = 0;

	switch (hovered_target)
	{
	case Panel::HoveredTarget::Top:
		val = val_top;
		break;
	case Panel::HoveredTarget::Left:
		val = val_left;
		break;
	case Panel::HoveredTarget::Right:
		val = val_right;
		break;
	}

	SDL_DestroyTexture(tex_text_background);
	tex_text_background = nullptr;
	SDL_DestroyTexture(tex_text_foreground);
	tex_text_foreground = nullptr;

	std::string str_val = val < 0 ? "max" : std::to_string(val);
	SDL_Surface* suf_text_background = TTF_RenderText_Blended(font, str_val.c_str(), color_text_background);
	SDL_Surface* suf_text_foreground = TTF_RenderText_Blended(font, str_val.c_str(), color_text_foreground);

	width_text = suf_text_background->w;
	height_text = suf_text_background->h;

	tex_text_background = SDL_CreateTextureFromSurface(render, suf_text_background);
	tex_text_foreground = SDL_CreateTextureFromSurface(render, suf_text_foreground);

	SDL_FreeSurface(suf_text_background);
	SDL_FreeSurface(suf_text_foreground);
}

void Panel::on_render(SDL_Renderer* render)
{
	if (!is_visible) return;

	SDL_Rect rect_dst_cursor =
	{
		center_pos.x - SIZE_TILE / 2,
		center_pos.y - SIZE_TILE / 2,
		SIZE_TILE,SIZE_TILE
	};

	SDL_RenderCopy(render, tex_select_cursor, nullptr, &rect_dst_cursor);

	SDL_Rect rect_dst_panel =
	{
		center_pos.x - width / 2,
		center_pos.y - height / 2,
		width,height
	};

	SDL_Texture* tex_panel = nullptr;

	switch (hovered_target)
	{
	case Panel::HoveredTarget::None:
		tex_panel = tex_idle;
		break;
	case Panel::HoveredTarget::Top:
		tex_panel = tex_hovered_top;
		break;
	case Panel::HoveredTarget::Left:
		tex_panel = tex_hovered_left;
		break;
	case Panel::HoveredTarget::Right:
		tex_panel = tex_hovered_right;
		break;
	}

	SDL_RenderCopy(render, tex_panel, nullptr, &rect_dst_panel);

	if (hovered_target == HoveredTarget::None)
		return;  

	/* »æÖÆÎÄ±¾ */
	SDL_Rect rect_dst_text; 
	rect_dst_text.x = center_pos.x - width_text / 2 + offset_shadow.x;
	rect_dst_text.y = center_pos.y + height / 2 + offset_shadow.y; 
	rect_dst_text.w = width_text, rect_dst_text.h = height_text; 

	SDL_RenderCopy(render, tex_text_background, nullptr, &rect_dst_text); 

	rect_dst_text.x -= offset_shadow.x; 
	rect_dst_text.y -= offset_shadow.y; 
	SDL_RenderCopy(render, tex_text_foreground, nullptr, &rect_dst_text);
}
