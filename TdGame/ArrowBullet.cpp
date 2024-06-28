#include "ArrowBullet.h"

ArrowBullet::ArrowBullet()
{
	/* ���õ���������ͼ */
	static SDL_Texture* tex_arrow = ResourceManager::GetInstance()
		->get_texture_pool().find(ResID::Tex_BulletArrow)->second;

	static const std::vector<int> idx_list = { 0,1 };

	animation.set_loop(true);
	animation.set_interval(0.1);
	animation.set_frame_data(tex_arrow, 2, 1, idx_list);

	can_rotated = true;
	size.x = 48;
	size.y = 48;
}

void ArrowBullet::on_collide(Enemy* enemy)
{
	static const ResourceManager::SoundPool sound_pool
		= ResourceManager::GetInstance()->get_sound_pool();
	switch (rand()%3)
	{
	case 0:
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_1)->second, 0);
		break;
	case 1:
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_2)->second, 0);
		break;
	case 2:
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_3)->second, 0);
		break;
	default:
		break;
	}
	enemy->slow_down();
	Bullet::on_collide(enemy);
}
