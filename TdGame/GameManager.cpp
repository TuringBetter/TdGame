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


		/* �������� */
		on_update(delta);

		/* ��Ⱦ���� */
		/* �����һ֡���� */
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);

		on_render();
		/* ����Ⱦ����Ⱦ�����ݸ��µ���Ļ��*/
		SDL_RenderPresent(render);
	}

	return 0;
}

GameManager::~GameManager()
{
	/*
	* ������˳��Ҫ���ʼ����˳���෴
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
	/* ϵͳ��ʼ�� */
	init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2 ��ʼ��ʧ�ܣ�");
	init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image ��ʼ��ʧ�ܣ�");
	init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer ��ʼ��ʧ�ܣ�");
	init_assert(!TTF_Init(), u8"SDL_ttf ��ʼ��ʧ�ܣ�");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	/* ��ʼ����Ϸ��Դ */
	auto config = ConfigManager::GetInstance();

	init_assert(config->map.load("map.csv"), u8"������Ϸ��ͼʧ��");
	init_assert(config->load_level_config("level.json"), u8"���عؿ�����ʧ��");
	init_assert(config->load_game_config("config.json"), u8"������Ϸ����ʧ��");


	window = SDL_CreateWindow(config->window_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		config->window_template.window_width, config->window_template.window_height, SDL_WINDOW_SHOWN);
	init_assert(window, u8"������Ϸ����ʧ�ܣ�");

	/*
	* SDL_RENDERER_ACCELERATED:Ӳ������
	* SDL_RENDERER_PRESENTVSYNC:��ֱͬ��
	* SDL_RENDERER_TARGETTEXTURE:�Ȱ����ݻ���һ��������
	**/
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	init_assert(render, u8"������Ⱦʧ�ܣ�");

	init_assert(ResourceManager::GetInstance()->load_from_file(render), u8"������Ϸ��Դʧ��");

	init_assert(generate_tile_map_texture(), u8"��Ⱦ��ͼʧ��");


	/* ��ӡmap���� */
	/* �ȴ�ӡidx_home */
	//std::cout << "map�е�idx_home����:" << std::endl;
	//std::cout << "idx_home.x=:"<< config->map.get_width() <<"  "<< "idx_home.y=:" << config->map.get_height()<<std::endl;
	
}

void GameManager::init_assert(bool flag, const char* err_msg)
{
	if (flag) return;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��Ϸ����ʧ��", err_msg, window);
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
 * @brief       ������Ƭͼ����
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
	* texture_pool �ǳ���,���ʹ��()�����,map��δ��ѯ������»�
	* ���ʹ��()�����,map��δ��ѯ������»�����һ��
	*/
	SDL_Texture* tex_tile_set = ResourceManager::GetInstance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

	int width_tex_tile_set, height_tex_tile_set;
	/* ���������ʽ */
	SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
	
	/* 
	* tex_tile_set �Ŀ��Ϊ 1200px
	* ��width_tex_tile_set=1200
	* SIZE_TILE=48
	* ���ÿ����1200/48=25����Ƭ
	*/
	int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);

	/*
	* �������е�����ֵ
	* eg.�п�(����ֵ)=����Ƭ��*��Ƭ����ֵ
	*/
	int width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
	int height_tex_tile_map = (int)map.get_height() * SIZE_TILE;

	tex_tile_map = SDL_CreateTexture(render, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);

	if (!tex_tile_map) return false;

	/* ���������� */
	auto config = ConfigManager::GetInstance();
	rect_tile_map.x = (config->window_template.window_width - width_tex_tile_map) / 2;
	rect_tile_map.y = (config->window_template.window_height - height_tex_tile_map) / 2;
	rect_tile_map.h = height_tex_tile_map;
	rect_tile_map.w = width_tex_tile_map;

	/* ����͸�������������Ⱦ */
	SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
	/* ������ȾĿ�� */
	SDL_SetRenderTarget(render,tex_tile_map);

	for (int y = 0; y < map.get_height(); y++)
	{
		for (int x = 0; x < map.get_width(); x++)
		{
			/* ���Ƶ�ͼ */
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

			/* ����װ���� */
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
	/* ���ƻ���ͼƬ */
	/* ��ȡ����λ�� */
	const SDL_Point& idx_home = map.get_home_point();

	const SDL_Rect rect_dst =
	{
		idx_home.x * SIZE_TILE,
		idx_home.y * SIZE_TILE,
		SIZE_TILE,SIZE_TILE
	};

	SDL_RenderCopy(render, ResourceManager::GetInstance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

	/* 
	* ��ͷ��β��ǰ���л�����ȾĿ�꣬�������Ҫ�л����� 
	* ��ȾĿ���ÿգ�������ȾĿ������Ϊԭ����
	*/
	SDL_SetRenderTarget(render, nullptr);

	return true;

}
