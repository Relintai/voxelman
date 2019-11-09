#include "DungeonStartRoom.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			DungeonStartRoom::DungeonStartRoom(int x, int y, int width, int height) : DungeonRoom(x, y, 8, 8){
			}
			void DungeonStartRoom::GenerateSpawnPoint(GameObject* startPositionPrefab)
			{
				bool flag = false;
				while (!flag) {
					int num = DungeonRoom::getX() + UnityEngine::Random::Range(1, DungeonRoom::getWidth() - 2);
					int num2 = DungeonRoom::getY() + UnityEngine::Random::Range(1, DungeonRoom::getHeight() - 2);
					flag = true;
					UnityEngine::Object::Instantiate<GameObject>(startPositionPrefab, Vector3::zero, Quaternion::identity)->transform->position = new Vector3((float)(num) * Voxelizzer::world->voxelScale, Voxelizzer::world->voxelScale, (float)(num2) * Voxelizzer::world->voxelScale);
				}
			}
			void DungeonStartRoom::GenerateAIPlayers(int level)
			{
			}

		}
	}
}
