#include "GameManager.h"

#include <iostream>

int GameManager::run(int argc, char** argv)
{
	//TowerManager::GetInstance()->place_tower(TowerType::Archer, { 6,9 });

	Mix_FadeInMusic(ResourceManager::GetInstance()->get_music_pool().find(ResID::Music_BGM)->second, -1, 1500);

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
	delete banner;

	delete upgrade_panel;
	delete place_panel;

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


	status_bar.set_position(15, 15);
	
	place_panel = new PlacePanel();
	upgrade_panel = new UpgradePanel();

	banner = new Banner();
}

void GameManager::init_assert(bool flag, const char* err_msg)
{
	if (flag) return;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��Ϸ����ʧ��", err_msg, window);
	exit(-1);
}

void GameManager::on_input()
{
	static SDL_Point pos_center;
	static SDL_Point idx_tile_selected;
	static auto instance = ConfigManager::GetInstance();

	switch (event.type)
	{
	case SDL_QUIT:
		is_quit = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (instance->is_game_over)return;
		if(get_cursor_idx_tile(idx_tile_selected,event.button.x,event.motion.y))
		{
			get_selected_tile_center_pos(pos_center, idx_tile_selected);

			if(check_home(idx_tile_selected))
			{
				upgrade_panel->set_idx_tile(idx_tile_selected);
				upgrade_panel->set_center_pos(pos_center);
				upgrade_panel->show();
			}
			else if (can_place_tower(idx_tile_selected))
			{
				place_panel->set_idx_tile(idx_tile_selected);
				place_panel->set_center_pos(pos_center);
				place_panel->show();
			}
		}
		break;
	default:
		break;
	}

	if(!instance->is_game_over)
	{
		place_panel->on_input(event);
		upgrade_panel->on_input(event);
		PlayerManager::GetInstance()->on_input(event);
	}
}

void GameManager::on_update(double delta)
{
	/* ��Ϸ�����ǲ��Ž�����Ч������Ҫ��ȡ��Ϸ����ʱ��ǰһ֡ */
	static bool is_game_over_last_tick = false;

	static auto instance = ConfigManager::GetInstance();
	if(!instance->is_game_over)
	{
		status_bar.on_update(render);
		place_panel->on_update(render);
		upgrade_panel->on_update(render);
		WaveManager::GetInstance()->on_update(delta);
		EnemyManager::GetInstance()->on_update(delta);
		BulletManager::GetInstance()->on_update(delta);
		TowerManager::GetInstance()->on_update(delta);
		PlayerManager::GetInstance()->on_update(delta);
		CoinManager::GetInstance()->on_update(delta);
		return;
	}

	if(!is_game_over_last_tick&&instance->is_game_over)
	{
		static const ResourceManager::SoundPool& sound_pool
			= ResourceManager::GetInstance()->get_sound_pool();

		Mix_FadeOutMusic(1500);
		Mix_PlayChannel(-1, sound_pool.find(instance->is_game_win?ResID::Sound_win:ResID::Sound_Loss)->second, 0);
	}

	is_game_over_last_tick = instance->is_game_over;

	banner->on_update(delta);
	if(banner->check_end_display())
		is_quit = true;
}

void GameManager::on_render()
{

	static auto config = ConfigManager::GetInstance();
	static SDL_Rect& rect_dst = config->rect_tile_map;
	SDL_RenderCopy(render, tex_tile_map, nullptr, &rect_dst);

	EnemyManager::GetInstance()->on_render(render);
	BulletManager::GetInstance()->on_render(render);
	TowerManager::GetInstance()->on_render(render);
	PlayerManager::GetInstance()->on_render(render);
	CoinManager::GetInstance()->on_render(render);
	if (!config->is_game_over)
	{
		place_panel->on_render(render);
		upgrade_panel->on_render(render);
		status_bar.on_render(render);

		return;
	}

	int width_screen, height_screen; 
	SDL_GetWindowSizeInPixels(window, &width_screen, &height_screen); 
	banner->set_center_position({ (double)width_screen / 2,(double)height_screen / 2 });
	banner->on_render(render);
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

/**
 * @brief       ���������λ��ת��Ϊ��ͼ����
 *
 * @param[in]	screen_x: ���x��
 * @param[in]	screen_y: ����
 * @param[out]  idx_tile_selected����ͼ����
 * 
 * @return		true  :  ��ǰ����е����ͼ
 *				false �� ��ǰ���û�е����ͼ
 */
bool GameManager::get_cursor_idx_tile(SDL_Point& idx_tile_selected, int screen_x, int screen_y)
{
	static const Map& map = ConfigManager::GetInstance()->map;
	static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;

	/* ���ǵ���ͼû�������������ڵ���� */
	if (screen_x<rect_tile_map.x || screen_x>rect_tile_map.x + rect_tile_map.w
		|| screen_y<rect_tile_map.y || screen_y>rect_tile_map.y + rect_tile_map.h)
		return false;
	/* 
	* int�������Զ�����С�����ּ��ɵ�ͼ����
	* ����Ҫ���ǵ��ҡ��±߽�
	*/
	idx_tile_selected.x = std::min((screen_x - rect_tile_map.x) / SIZE_TILE, (int)map.get_width() - 1);
	idx_tile_selected.y = std::min((screen_y - rect_tile_map.y) / SIZE_TILE, (int)map.get_height() - 1);

	return true;
}

bool GameManager::can_place_tower(const SDL_Point& idx_tile_selected) const
{
	static const Map& map = ConfigManager::GetInstance()->map;
	const Tile& tile = map.get_tile_map()[idx_tile_selected.y][idx_tile_selected.x];

	/*
	* 1����Ƭ��û��װ����
	* 2�����ڹ����н�·����
	* 3����Ƭ��û�з�����
	*/
	return (tile.decoration < 0 && tile.direction == Tile::Direction::None && !tile.has_tower);
}


void GameManager::get_selected_tile_center_pos(SDL_Point& pos, const SDL_Point& idx_tile_selected)
{
	static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;

	pos.x = rect_tile_map.x + idx_tile_selected.x * SIZE_TILE + SIZE_TILE / 2;
	pos.y = rect_tile_map.y + idx_tile_selected.y * SIZE_TILE + SIZE_TILE / 2;
}

bool GameManager::check_home(const SDL_Point& idx_tile_selected)
{
	static const Map& map = ConfigManager::GetInstance()->map;
	static const SDL_Point& idx_home = map.get_home_point();

	return (idx_tile_selected.x == idx_home.x) && (idx_tile_selected.y == idx_home.y);
}
