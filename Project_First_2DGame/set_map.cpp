#include "set_map.h"

#include <time.h>

#include "Ground_tile.h"
#include "Visual_tile.h"
#include "enemy.h"

void Set_map(void) {
	for (int i = 0; i < SNAP_SIZE * 256; i += SNAP_SIZE) {
		if (0 == rand() % 1) {
			SetG_TILE(0, XMFLOAT3(i, SCREEN_HEIGHT - SNAP_SIZE / 2, SNAP_SIZE));
			if (0 == rand() % 5) {
				SetV_tile(0, XMFLOAT3(i, SCREEN_HEIGHT - SNAP_SIZE / 2 - SNAP_SIZE, SNAP_SIZE));
			}
			else if (0 == rand() % 5) {
				SetV_tile(1, XMFLOAT3(i, SCREEN_HEIGHT - SNAP_SIZE / 2 - SNAP_SIZE, SNAP_SIZE));
			}
		}

	}
	Set_Enemy(ENEMY_HUNTRESS, XMFLOAT2(300, 100));
}