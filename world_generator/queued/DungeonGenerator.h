#pragma once
#include <System/System.h>
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "DungeonRoom.h"
#include <System/Collections/Generic/List.h>
#include "DungeonCorridor.h"
#include <System/Text/StringBuilder.h>
#include "UnityEngine.h"
#include "BrokenSeals.h"
#include "CxNet.h"
#include "DungeonStartRoom.h"
#include "DungeonEndRoom.h"
#include "TileConsts.h"

using namespace UnityEngine;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace CxNetworking;
using namespace System;
namespace BS {
	namespace Levels {
		namespace Generator {
			class DungeonGenerator : public virtual MonoBehaviour, public virtual Object
			{
				public:
					int Seed;
				public:
					bool SpawnMobs;
				public:
					int DUNGEON_Size_X;
				public:
					int DUNGEON_Size_Y;
				public:
					int ROOM_MIN_SIZE;
				public:
					int ROOM_MAX_SIZE;
				public:
					int ROOM_PLACEMENT_TRIES;
				public:
					int CORRIDOR_SIZE;
				//Attribute: SerializeField*
				private:
					GameObject* startPositionPrefab;
				private:
					ArrayND<char, 2>* dungeon;
				private:
					List_T<DungeonRoom>* rooms;
				private:
					List_T<DungeonCorridor>* corridors;
				private:
					StringBuilder* sb;
				public:
					ArrayND<char, 2>* getDungeon();
					//Ignored empty method declaration
				private:
					void Awake();
				public:
					void Generate(int level);
				public:
					void GenerateDungeon();
				public:
					void GenerateCorridors();
				public:
					void GeneratePlayers(int level);
				public:
					void PreprocessDungeonSimple();
				public:
					void PreprocessDungeon();
				public:
					String* StringifyDungeon();
				private:
					void GenerateDebugDungeon();
				private:
					bool CanPlaceRoom(int x, int y, int width, int height);
				public:
					void Reset();
				public:
					DungeonGenerator();
			};
		}
	}
}
