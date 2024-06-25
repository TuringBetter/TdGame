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
	* 程序运行之前需要初始化的变量 
	* 需要提供相应的set方法
	*/
	double wait_time = 0;
	bool one_shot = true;
	bool paused = false;
	std::function<void()> callback;
	/* 
	* 程序运行时会更新的变量，无需手动外部修改变量值 
	* 无需提供set方法
	*/
	double pass_time = 0;
	bool shotted = false;

};

