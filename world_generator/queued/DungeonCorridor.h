#pragma once
#include <System/System.h>
#include "DungeonRoom.h"
#include "IntRect.h"
#include "Mathf.h"
#include "UnityEngine.h"
#include "Vector3.h"
#include "Voxelizzer.h"

using namespace UnityEngine;
using namespace VoxelToolbox;
using namespace System;
namespace BS {
	namespace Levels {
		namespace Generator {
			class DungeonCorridor : public virtual Object
			{
				private:
					DungeonRoom* startRoom;
				private:
					DungeonRoom* endRoom;
				public:
					int size;
				private:
					IntRect* widthir;
				private:
					IntRect* heightir;
				public:
					DungeonRoom* getStartRoom();
				public:
					void setStartRoom(DungeonRoom* value);
				public:
					DungeonRoom* getEndRoom();
				public:
					void setEndRoom(DungeonRoom* value);
				public:
					DungeonCorridor(int size, DungeonRoom* startRoom, DungeonRoom* endRoom);
				public:
					void GenerateCorridor(ArrayND<char, 2>* dungeon);
				private:
					void Write(ArrayND<char, 2>* dungeon, IntRect* rect);
				public:
				virtual void GenerateAIPlayers(int level);
				private:
					void GenerateAIPlayers(int level, IntRect* rect);
			};
		}
	}
}
