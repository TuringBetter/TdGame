#include "GameManager.h"

#include <iostream>

int GameManager::run(int argc, char** argv)
{
	Uint64 last_counter = SDL_GetPerformanceCounter();
	const Uint64 counter_freq = SDL_GetPerformanceFrequency();

	while (!is_quit)
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
	/* 系统初始化 */
	init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2 初始化失败！");
	init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image 初始化失败！");
	init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer 初始化失败！");
	init_assert(!TTF_Init(), u8"SDL_ttf 初始化失败！");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	/* 初始化游戏资源 */
	auto config = ConfigManager::GetInstance();

	init_assert(config->map.load("map.csv"), u8"加载游戏地图失败");
	init_assert(config->load_level_config("level.json"), u8"加载关卡配置失败");
	init_assert(config->load_game_config("config.json"), u8"加载游戏配置失败");


	window = SDL_CreateWindow(config->window_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		config->window_template.window_width, config->window_template.window_height, SDL_WINDOW_SHOWN);
	init_assert(window, u8"创建游戏窗口失败！");

	/*
	* SDL_RENDERER_ACCELERATED:硬件加速
	* SDL_RENDERER_PRESENTVSYNC:垂直同步
	* SDL_RENDERER_TARGETTEXTURE:先把内容画到一张纹理上
	**/
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	init_assert(render, u8"创建渲染失败！");

	init_assert(ResourceManager::GetInstance()->load_from_file(render), u8"加载游戏资源失败");

	init_assert(generate_tile_map_texture(), u8"渲染地图失败");


	/* 打印map数据 */
	/* 先打印idx_home */
	//std::cout << "map中的idx_home数据:" << std::endl;
	//std::cout << "idx_home.x=:"<< config->map.get_width() <<"  "<< "idx_home.y=:" << config->map.get_height()<<std::endl;
	
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
	static auto config = ConfigManager::GetInstance();
	static SDL_Rect& rect_dst = config->rect_tile_map;
	SDL_RenderCopy(render, tex_tile_map, nullptr, &rect_dst);
}
/**
 * @brief       生成瓦片图纹理
 *				
 * @param[in]
 * @param[out]
 * @return
 */
bool GameManager::generate_tile_map_texture()
{
	const Map& map = ConfigManager::GetInstance()->map;
	const TileMap& tile_map = map.get_tile_map();
	SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
	/* 
	* texture_pool 是常量,如果使用()运算符,map在未查询的情况下会
	* 如果使用()运算符,map在未查询的情况下会新增一项
	*/
	SDL_Texture* tex_tile_set = ResourceManager::GetInstance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

	int width_tex_tile_set, height_tex_tile_set;
	/* 访问纹理格式 */
	SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
	
	/* 
	* tex_tile_set 的宽度为 1200px
	* 即width_tex_tile_set=1200
	* SIZE_TILE=48
	* 因此每行有1200/48=25个瓦片
	*/
	int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);

	/*
	* 计算行列的像素值
	* eg.行宽(像素值)=行瓦片数*瓦片像素值
	*/
	int width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
	int height_tex_tile_map = (int)map.get_height() * SIZE_TILE;

	tex_tile_map = SDL_CreateTexture(render, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);

	if (!tex_tile_map) return false;

	/* 将画布居中 */
	auto config = ConfigManager::GetInstance();
	rect_tile_map.x = (config->window_template.window_width - width_tex_tile_map) / 2;
	rect_tile_map.y = (config->window_template.window_height - height_tex_tile_map) / 2;
	rect_tile_map.h = height_tex_tile_map;
	rect_tile_map.w = width_tex_tile_map;

	/* 设置透明度启动混合渲染 */
	SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
	/* 设置渲染目标 */
	SDL_SetRenderTarget(render,tex_tile_map);

	for (int y = 0; y < map.get_height(); y++)
	{
		for (int x = 0; x < map.get_width(); x++)
		{
			/* 绘制地图 */
			SDL_Rect rect_src;
			const Tile& tile = tile_map[y][x];

			const SDL_Rect& rect_dst =
			{
				x * SIZE_TILE,y * SIZE_TILE,
				SIZE_TILE,SIZE_TILE
			};

			rect_src =
			{
				(tile.terrian % num_tile_single_line) * SIZE_TILE,
				(tile.terrian / num_tile_single_line) * SIZE_TILE,
				SIZE_TILE,SIZE_TILE
			};

			SDL_RenderCopy(render, tex_tile_set, &rect_src, &rect_dst);

			/* 绘制装饰物 */
			if(tile.decoration>=0)
			{
				rect_src =
				{
					(tile.decoration % num_tile_single_line) * SIZE_TILE,
					(tile.decoration / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE,SIZE_TILE
				};
				SDL_RenderCopy(render, tex_tile_set, &rect_src, &rect_dst);
			}
		}
	}
	/* 绘制基地图片 */
	/* 获取基地位置 */
	const SDL_Point& idx_home = map.get_home_point();

	const SDL_Rect rect_dst =
	{
		idx_home.x * SIZE_TILE,
		idx_home.y * SIZE_TILE,
		SIZE_TILE,SIZE_TILE
	};

	SDL_RenderCopy(render, ResourceManager::GetInstance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

	/* 
	* 有头有尾，前文切换了渲染目标，则后文需要切换回来 
	* 渲染目标置空，即将渲染目标设置为原窗口
	*/
	SDL_SetRenderTarget(render, nullptr);

	return true;

}
