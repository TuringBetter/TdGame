#include "AxeBullet.h"

AxeBullet::AxeBullet()
{
	/* 先拿到动画纹理图 */
	static SDL_Texture* tex_axe = ResourceManager::GetInstance()
		->get_texture_pool().find(ResID::Tex_BulletAxe)->second;

	static const std::vector<int> idx_list = { 0,1,2,3,4,5,6,7,8 };

	animation.set_loop(true);
	animation.set_interval(0.1);
	animation.set_frame_data(tex_axe, 4, 2, idx_list);

	can_rotated = false;
	size.x = 48;
	size.y = 48;
}

void AxeBullet::on_collide(Enemy* enemy)
{
	static const ResourceManager::SoundPool sound_pool
		= ResourceManager::GetInstance()->get_sound_pool();
	switch (rand()%3)
	{
	case 0:
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_1)->second, 0);
		break;
	case 1:
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_2)->second, 0);
		break;
	case 2:
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_3)->second, 0);
		break;
	default:
		break;
	}
	Bullet::on_collide(enemy);

}
