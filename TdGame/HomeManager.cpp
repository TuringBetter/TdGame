#include "HomeManager.h"

double HomeManager::get_current_hp() const
{
	return num_hp;
}

/*
* ��������ֵ
* ���ű�������Ч
*/
void HomeManager::decrease_hp(double val)
{
	num_hp -= val;
	if (num_hp <= 0)
	{
		num_hp = 0;
		ConfigManager::GetInstance()->is_game_over = true;
	}
	static const ResourceManager::SoundPool& sound_pool
		= ResourceManager::GetInstance()->get_sound_pool();

	Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_HomeHurt)->second,0);
}
