#pragma once
#include <System/System.h>
#include "UnityEngine.h"
#include "Vector3.h"
#include "Voxelizzer.h"
#include "Entity.h"
#include "EntityType.h"
#include "Quaternion.h"

using namespace UnityEngine;
using namespace VoxelToolbox;
using namespace BS::Player;
using namespace System;
namespace BS {
	namespace Levels {
		namespace Generator {
			class DungeonRoom : public virtual Object
			{
				private:
				static Array<String>* biga;
				private:
				static Array<String>* bigb;
				private:
				static Array<String>* smalla;
				private:
				static Array<String>* smallb;
				private:
					int x;
				private:
					int y;
				private:
					int width;
				private:
					int height;
				public:
					int getX();
				public:
					void setX(int value);
				public:
					int getY();
				public:
					void setY(int value);
				public:
					int getWidth();
				public:
					void setWidth(int value);
				public:
					int getHeight();
				public:
					void setHeight(int value);
				public:
					DungeonRoom(int x, int y, int width, int height);
				public:
				virtual void WriteRoom(ArrayND<char, 2>* dungeon);
				public:
				virtual void GenerateAIPlayers(int level);
				public:
					String* GenerateName();
				static:
					DungeonRoom();
			};
		}
	}
}
