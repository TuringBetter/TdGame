#include "Timer.h"

void Timer::set_wait_time(double val)
{
	wait_time = val;
}

void Timer::restart()
{
	pass_time = 0;
	shotted = false;
}

void Timer::set_one_shot(bool flag)
{
	one_shot = flag;
}

void Timer::set_callback(std::function<void()> callback)
{
	this->callback = callback;
}

void Timer::pause()
{
	paused = true;
}

void Timer::resume()
{
	paused = false;
}

void Timer::on_update(double delta)
{

	if (paused) return;

	pass_time += delta;
	if(pass_time>=wait_time)
	{
		bool can_shot = !one_shot || (one_shot && !shotted);
		shotted = true;
		
		//��Ϊ������callback,�����ڴ�֮ǰһ��Ҫ�������Ϸ���	
		if (can_shot && callback)
			callback();
		//���¼�����
		pass_time -= wait_time;
	}
}
