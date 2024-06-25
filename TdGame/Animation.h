#pragma once

#include "Timer.h"

#include <SDL.h>
#include <vector>

/*
* 一张哥布林图片资源：
* 前1 前2 前3 前4 前5 前6
* 后1 后2 后3 后4 后5 后6
* 左1 左2 左3 左4 左5 左6
* 右1 右2 右3 右4 右5 右6
*/
class Animation
{
public:
	typedef std::function<void()> PlayCallback;
public:
	Animation();
	~Animation() = default;

	void reset();
	void set_frame_date(SDL_Texture* texture, int num_h, int num_v, const std::vector<int>& idx_list);
	void set_loop(bool flag);
	void set_interval(double interval);
	void set_callback(PlayCallback callback);
	void on_update(double delta);
	void on_render(SDL_Renderer* render, const SDL_Point& pos_dst, double angle = 0)const;
private:
	Timer timer;
	bool is_loop = true;
	/* 动画使用到的纹理 */
	SDL_Texture* texture = nullptr;
	size_t idx_frame = 0;
	PlayCallback on_finish;
	/* 
	* 描述某个身位在原始图片中的位置 
	* eg. 后2在SDL_Rect{x,y,w,h}
	*/
	std::vector<SDL_Rect> rect_src_list;
	/* 每一帧的大小 */
	int width_frame = 0, height_frame = 0;
};

