#include "GameManager.h"

int GameManager::run(int argc, char** argv)
{
	Uint64 last_counter = SDL_GetPerformanceCounter();
	const Uint64 counter_freq = SDL_GetPerformanceFrequency();

	while(!is_quit)
	{
		while (SDL_PollEvent(&event)) 
			on_input();
		

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / 60)
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));
		

		/* 更新数据 */
		on_update(delta);

		/* 渲染数据 */
		/* 清空上一帧内容 */
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);

		on_render();
		/* 让渲染器渲染的内容更新到屏幕上*/
		SDL_RenderPresent(render);
	}

	return 0;
}

GameManager::~GameManager()
{
	/* 
	* 析构的顺序要与初始化的顺序相反
	*/
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

GameManager::GameManager()
{
	init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2 初始化失败！");
	init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image 初始化失败！");
	init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer 初始化失败！");
	init_assert(!TTF_Init(), u8"SDL_ttf 初始化失败！");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	window = SDL_CreateWindow(u8"村庄保卫站", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	init_assert(window, u8"创建游戏窗口失败！");

	/*
	* SDL_RENDERER_ACCELERATED:硬件加速
	* SDL_RENDERER_PRESENTVSYNC:垂直同步
	* SDL_RENDERER_TARGETTEXTURE:先把内容画到一张纹理上
	**/
	SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	init_assert(render, u8"创建渲染失败！");

}

void GameManager::init_assert(bool flag, const char* err_msg)
{
	if (flag) return;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"游戏启动失败", err_msg, window);
	exit(-1);
}

void GameManager::on_input()
{
}

void GameManager::on_update(double delta)
{
}

void GameManager::on_render()
{
}
