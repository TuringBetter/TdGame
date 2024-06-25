#pragma once

#include "Timer.h"

#include <SDL.h>
#include <vector>

/*
* һ�Ÿ粼��ͼƬ��Դ��
* ǰ1 ǰ2 ǰ3 ǰ4 ǰ5 ǰ6
* ��1 ��2 ��3 ��4 ��5 ��6
* ��1 ��2 ��3 ��4 ��5 ��6
* ��1 ��2 ��3 ��4 ��5 ��6
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
	/* ����ʹ�õ������� */
	SDL_Texture* texture = nullptr;
	size_t idx_frame = 0;
	PlayCallback on_finish;
	/* 
	* ����ĳ����λ��ԭʼͼƬ�е�λ�� 
	* eg. ��2��SDL_Rect{x,y,w,h}
	*/
	std::vector<SDL_Rect> rect_src_list;
	/* ÿһ֡�Ĵ�С */
	int width_frame = 0, height_frame = 0;
};

