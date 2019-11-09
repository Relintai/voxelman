#include "DungeonEndRoom.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			DungeonEndRoom::DungeonEndRoom(int x, int y, int width, int height) : DungeonRoom(x, y, width, height){
			}
			void DungeonEndRoom::WriteRoom(ArrayND<char, 2>* dungeon)
			{
				for (int i = DungeonRoom::getY(); i < (DungeonRoom::getY() + DungeonRoom::getHeight()); i += 1) {
					for (int j = DungeonRoom::getX(); j < (DungeonRoom::getX() + DungeonRoom::getWidth()); j += 1) {
						dungeon->SetData(j, i, 1);
					}
				}
				for (int k = DungeonRoom::getX() + 1; k < ((DungeonRoom::getX() + DungeonRoom::getWidth()) - 1); k += 1) {
					dungeon->SetData(k, (DungeonRoom::getY() + DungeonRoom::getHeight()) - 1, 11);
				}
				for (int l = DungeonRoom::getX() + 1; l < ((DungeonRoom::getX() + DungeonRoom::getWidth()) - 1); l += 1) {
					dungeon->SetData(l, DungeonRoom::getY(), 13);
				}
				for (int m = DungeonRoom::getY() + 1; m < ((DungeonRoom::getY() + DungeonRoom::getHeight()) - 1); m += 1) {
					dungeon->SetData(DungeonRoom::getX(), m, 14);
				}
				for (int n = DungeonRoom::getY() + 1; n < ((DungeonRoom::getY() + DungeonRoom::getHeight()) - 1); n += 1) {
					dungeon->SetData((DungeonRoom::getX() + DungeonRoom::getWidth()) - 1, n, 12);
				}
				dungeon->SetData(DungeonRoom::getX(), DungeonRoom::getY(), 13);
				dungeon->SetData((DungeonRoom::getX() + DungeonRoom::getWidth()) - 1, DungeonRoom::getY(), 13);
				dungeon->SetData(DungeonRoom::getX(), (DungeonRoom::getY() + DungeonRoom::getHeight()) - 1, 13);
				dungeon->SetData((DungeonRoom::getX() + DungeonRoom::getWidth()) - 1, (DungeonRoom::getY() + DungeonRoom::getHeight()) - 1, 13);
				int num = UnityEngine::Random::Range(DungeonRoom::getX() + 1, (DungeonRoom::getX() + DungeonRoom::getWidth()) - 1);
				int num2 = UnityEngine::Random::Range(DungeonRoom::getY() + 1, (DungeonRoom::getY() + DungeonRoom::getHeight()) - 1);
				dungeon->SetData(num, num2, 20);
			}

		}
	}
}
