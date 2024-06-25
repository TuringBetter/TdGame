#pragma once

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void set_wait_time(double val);
	//void set_pass_time(double val);
	void restart();
	void set_one_shot(bool flag);
	void set_callback(std::function<void()> callback);
	void pause();
	void resume();
	void on_update(double delta);

private:
	/* 
	* ��������֮ǰ��Ҫ��ʼ���ı��� 
	* ��Ҫ�ṩ��Ӧ��set����
	*/
	double wait_time = 0;
	bool one_shot = true;
	bool paused = false;
	std::function<void()> callback;
	/* 
	* ��������ʱ����µı����������ֶ��ⲿ�޸ı���ֵ 
	* �����ṩset����
	*/
	double pass_time = 0;
	bool shotted = false;

};

