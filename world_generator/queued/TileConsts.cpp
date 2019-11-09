#include "TileConsts.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			bool TileConsts::isWall(char tile){
				switch (tile){
					case 11:
						return true;

					case 12:
						return true;

					case 13:
						return true;

					case 14:
						return true;

					case 15:
						return true;

					case 16:
						return true;

					case 17:
						return true;

					case 18:
						return true;

					case 21:
						return true;

					case 22:
						return true;

					case 23:
						return true;
				}
				return false;
			}
			TileConsts::TileConsts()
			{
				Floor = 1;
				Floor_Var1 = 2;
				Floor_Up = 3;
				Floor_Left = 4;
				Floor_Down = 5;
				Floor_Right = 6;
				Floor_UpRight = 7;
				Floor_UpLeft = 8;
				Floor_DownLeft = 9;
				Floor_DownRight = 10;
				Wall_Up = 11;
				Wall_Right = 12;
				Wall_Down = 13;
				Wall_Left = 14;
				Wall_Corner_TopRight = 15;
				Wall_Corner_TopLeft = 16;
				Wall_Corner_BottomRight = 17;
				Wall_Corner_BottomRight_Outside = 23;
				Wall_Corner_BottomLeft = 18;
				Wall_Corner_BottomLeft_Outside = 21;
				Wall_Corner_BottomLeft_Half_Outside = 22;
				Roof_Wood_Single = 19;
				Stair_Down = 20;
			}

		}
	}
}
