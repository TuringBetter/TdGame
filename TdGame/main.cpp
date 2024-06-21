#define SDL_MAIN_HANDLED
#include "GameManager.h"

/**
 * @brief 
 *
 * @param[in] 
 * @param[out]
 * @return
 */

int main(int argc, char** argv)
{
	return GameManager::GetInstance()->run(argc,argv);
}