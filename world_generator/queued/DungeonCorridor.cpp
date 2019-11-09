#include "DungeonCorridor.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			DungeonRoom* DungeonCorridor::getStartRoom(){
				return this->startRoom;
			}
			void DungeonCorridor::setStartRoom(DungeonRoom* value)
			{
				this->startRoom = value;
			}
			DungeonRoom* DungeonCorridor::getEndRoom()
			{
				return this->endRoom;
			}
			void DungeonCorridor::setEndRoom(DungeonRoom* value)
			{
				this->endRoom = value;
			}
			DungeonCorridor::DungeonCorridor(int size, DungeonRoom* startRoom, DungeonRoom* endRoom)
			{
				size = 4;
				this->size = size;
				this->setStartRoom(startRoom);
				this->setEndRoom(endRoom);
			}
			void DungeonCorridor::GenerateCorridor(ArrayND<char, 2>* dungeon)
			{
				int num = this->getStartRoom()->X + Mathf::RoundToInt((float)(this->getStartRoom()->Width) / (float)2);
				int num2 = this->getStartRoom()->Y + Mathf::RoundToInt((float)(this->getStartRoom()->Height) / (float)2);
				int num3 = this->getEndRoom()->X + Mathf::RoundToInt((float)(this->getEndRoom()->Width) / (float)2);
				int num4 = this->getEndRoom()->Y + Mathf::RoundToInt((float)(this->getEndRoom()->Height) / (float)2);
				if ((num <= num3) && (num2 <= num4)) {
					this->widthir = new IntRect(num, num2, num3 - num, this->size);
					this->heightir = new IntRect((this->widthir->X + this->widthir->Width) - this->size, num2, this->size, num4 - num2);
				}
				else {
					if ((num > num3) && (num2 <= num4)) {
						this->widthir = new IntRect(num3, num2, num - num3, this->size);
						this->heightir = new IntRect(this->widthir->X, num2, this->size, num4 - num2);
					}
					else {
						if ((num <= num3) && (num2 > num4)) {
							this->widthir = new IntRect(num, num4, num3 - num, this->size);
							this->heightir = new IntRect(num, num4, this->size, num2 - num4);
						}
						else {
							if ((num > num3) && (num2 > num4)) {
								this->widthir = new IntRect(num3 - this->size, num2 - this->size, num - num3, this->size);
								this->heightir = new IntRect(this->widthir->X, num4, this->size, num2 - num4);
							}
						}
					}
				}
				this->Write(dungeon, this->widthir);
				this->Write(dungeon, this->heightir);
			}
			void DungeonCorridor::Write(ArrayND<char, 2>* dungeon, IntRect* rect)
			{
				for (int i = rect->getY(); i < (rect->getY() + rect->getHeight()); i += 1) {
					for (int j = rect->getX(); j < (rect->getX() + rect->getWidth()); j += 1) {
						if (((*(dungeon->GetData(j, i)) != 1) && (*(dungeon->GetData(j, i)) != 2)) && (*(dungeon->GetData(j, i)) != 20)) {
							if ((j == rect->getX()) && (i == rect->getY())) {
								dungeon->SetData(j, i, 13);
							}
							else {
								if ((j == rect->getX()) && (i == ((rect->getY() + rect->getHeight()) - 1))) {
									dungeon->SetData(j, i, 13);
								}
								else {
									if ((j == ((rect->getX() + rect->getWidth()) - 1)) && (i == rect->getY())) {
										dungeon->SetData(j, i, 13);
									}
									else {
										if ((j == ((rect->getX() + rect->getWidth()) - 1)) && (i == ((rect->getY() + rect->getHeight()) - 1))) {
											dungeon->SetData(j, i, 13);
										}
										else {
											if (j == rect->getX()) {
												dungeon->SetData(j, i, 14);
											}
											else {
												if (j == ((rect->getX() + rect->getWidth()) - 1)) {
													dungeon->SetData(j, i, 12);
												}
												else {
													if (i == rect->getY()) {
														dungeon->SetData(j, i, 13);
													}
													else {
														if (i == ((rect->getY() + rect->getHeight()) - 1)) {
															dungeon->SetData(j, i, 11);
														}
														else {
															dungeon->SetData(j, i, 1);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			void DungeonCorridor::GenerateAIPlayers(int level)
			{
				this->GenerateAIPlayers(level, this->widthir);
				this->GenerateAIPlayers(level, this->heightir);
			}
			void DungeonCorridor::GenerateAIPlayers(int level, IntRect* rect)
			{
				int num = UnityEngine::Random::Range(0, 3);
				for (int i = 0; i < num; i += 1) {
					int num2 = rect->getX() + UnityEngine::Random::Range(1, rect->getWidth() - 2);
					int num3 = rect->getY() + UnityEngine::Random::Range(1, rect->getHeight() - 2);
					new Vector3(((float)(num2) * Voxelizzer::world->voxelScale) * (float)2, Voxelizzer::world->voxelScale, ((float)(num3) * Voxelizzer::world->voxelScale) * (float)2);
				}
			}

		}
	}
}
