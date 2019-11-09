#pragma once
#include <System/System.h>
#include "DungeonRoom.h"
#include "GameObject.h"
#include "UnityEngine.h"
#include "Vector3.h"
#include "Voxelizzer.h"
#include "Quaternion.h"

using namespace UnityEngine;
using namespace VoxelToolbox;
using namespace System;
namespace BS {
	namespace Levels {
		namespace Generator {
			class DungeonStartRoom : public virtual DungeonRoom, public virtual Object
			{
				public:
					DungeonStartRoom(int x, int y, int width, int height);
				public:
					void GenerateSpawnPoint(GameObject* startPositionPrefab);
				public:
				virtual void GenerateAIPlayers(int level);
			};
		}
	}
}
