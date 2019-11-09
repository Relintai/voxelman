#pragma once
#include <System/System.h>

using namespace System;
namespace BS {
	namespace Levels {
		namespace Generator {
			class TileConsts : public virtual Object
			{
				public:
					char Floor;
				public:
					char Floor_Var1;
				public:
					char Floor_Up;
				public:
					char Floor_Left;
				public:
					char Floor_Down;
				public:
					char Floor_Right;
				public:
					char Floor_UpRight;
				public:
					char Floor_UpLeft;
				public:
					char Floor_DownLeft;
				public:
					char Floor_DownRight;
				public:
					char Wall_Up;
				public:
					char Wall_Right;
				public:
					char Wall_Down;
				public:
					char Wall_Left;
				public:
					char Wall_Corner_TopRight;
				public:
					char Wall_Corner_TopLeft;
				public:
					char Wall_Corner_BottomRight;
				public:
					char Wall_Corner_BottomRight_Outside;
				public:
					char Wall_Corner_BottomLeft;
				public:
					char Wall_Corner_BottomLeft_Outside;
				public:
					char Wall_Corner_BottomLeft_Half_Outside;
				public:
					char Roof_Wood_Single;
				public:
					char Stair_Down;
				public:
				static bool isWall(char tile);
				public:
					TileConsts();
			};
		}
	}
}
