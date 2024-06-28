#include "Animation.h"

Animation::Animation()
{
	/* 人物运动动画时循环播放的 */
	timer.set_one_shot(false);
	/* 每次定时器到达，更新下一帧动画 */
	timer.set_callback([&]()
		{
			idx_frame++;
			if (idx_frame >= rect_src_list.size())
			{
				idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
				if (!is_loop && on_finish)
					on_finish();
			}
		});

}

void Animation::reset()
{
	idx_frame = 0;
	timer.restart();
}

void Animation::set_frame_data(SDL_Texture* texture, int num_h, int num_v, const std::vector<int>& idx_list)
{
	int width_tex, height_tex;
	this->texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &width_tex, &height_tex);
	width_frame = width_tex / num_h;
	height_frame = height_tex / num_v;
	rect_src_list.resize(idx_list.size());
	for (size_t i = 0; i < idx_list.size(); i++)
	{
		int idx = idx_list[i];
		SDL_Rect& rect_src = rect_src_list[i];

		rect_src.x = (idx % num_h) * width_frame;
		rect_src.y = (idx / num_h) * height_frame;
		rect_src.w = width_frame;
		rect_src.h = height_frame;
	}
}

void Animation::set_loop(bool flag)
{
	is_loop = flag;
}

void Animation::set_interval(double interval)
{
	timer.set_wait_time(interval);
}

void Animation::set_callback(PlayCallback callback)
{
	this->on_finish = callback;
}

void Animation::on_update(double delta)
{
	timer.on_update(delta);
}

void Animation::on_render(SDL_Renderer* render, const SDL_Point& pos_dst, double angle) const
{
	static SDL_Rect rect_dst;
	rect_dst.x = pos_dst.x;
	rect_dst.y = pos_dst.y;
	rect_dst.h = height_frame;
	rect_dst.w = width_frame;

	/*
	* render 画笔
	* texture 动画图集（囊括前后左右123456的那个）
	* rect_src_list[idx_frame] 要取动画图集的哪一个素材
	* rect_dst 要画到画布的位置
	*/
	SDL_RenderCopyEx(render, texture, &rect_src_list[idx_frame], &rect_dst, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}
