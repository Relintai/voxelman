#include "DungeonGenerator.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			ArrayND<char, 2>* DungeonGenerator::getDungeon(){
				return this->dungeon;
			}
			void DungeonGenerator::Awake()
			{
				this->rooms = new List_T<DungeonRoom>();
				this->corridors = new List_T<DungeonCorridor>();
				this->dungeon = new ArrayND<char>(this->DUNGEON_Size_X, this->DUNGEON_Size_Y);
			}
			void DungeonGenerator::Generate(int level)
			{
				UnityEngine::Random::InitState(BrokenSeals::Game->Seed);
				if (!CxNet::IsServer) {
					this->SpawnMobs = false;
				}
				this->GenerateDungeon();
				this->GenerateCorridors();
				this->PreprocessDungeonSimple();
				this->GeneratePlayers(level);
			}
			void DungeonGenerator::GenerateDungeon()
			{
				for (int i = 0; i < this->ROOM_PLACEMENT_TRIES; i += 1) {
					int x = UnityEngine::Random::Range(0, this->DUNGEON_Size_X - this->ROOM_MAX_SIZE);
					int y = UnityEngine::Random::Range(0, this->DUNGEON_Size_Y - this->ROOM_MAX_SIZE);
					int width = UnityEngine::Random::Range(this->ROOM_MIN_SIZE, this->ROOM_MAX_SIZE);
					int height = UnityEngine::Random::Range(this->ROOM_MIN_SIZE, this->ROOM_MAX_SIZE);
					if (this->CanPlaceRoom(x, y, width, height)) {
						if (this->rooms->Count > 0) {
							DungeonRoom* dungeonRoom = new DungeonRoom(x, y, width, height);
							dungeonRoom->WriteRoom(this->dungeon);
							this->corridors->Add(new DungeonCorridor(this->CORRIDOR_SIZE, this->rooms->GetData(this->rooms->Count - 1), dungeonRoom));
							this->rooms->Add(dungeonRoom);
						}
						else {
							DungeonStartRoom* dungeonStartRoom = new DungeonStartRoom(x, y, width, height);
							dungeonStartRoom->WriteRoom(this->dungeon);
							dungeonStartRoom->GenerateSpawnPoint(this->startPositionPrefab);
							this->rooms->Add(dungeonStartRoom);
						}
					}
				}
				DungeonRoom* dungeonRoom2 = this->rooms->GetData(this->rooms->Count - 1);
				this->rooms->Remove(dungeonRoom2);
				this->corridors->RemoveAt(this->corridors->Count - 1);
				DungeonEndRoom* dungeonEndRoom = new DungeonEndRoom(dungeonRoom2->getX(), dungeonRoom2->getY(), dungeonRoom2->getWidth(), dungeonRoom2->getHeight());
				this->corridors->Add(new DungeonCorridor(this->CORRIDOR_SIZE, this->rooms->GetData(this->rooms->Count - 1), dungeonEndRoom));
				this->rooms->Add(dungeonEndRoom);
				dungeonEndRoom->WriteRoom(this->dungeon);
			}
			void DungeonGenerator::GenerateCorridors()
			{
				for (int i = 0; i < this->corridors->Count; i += 1) {
					this->corridors->GetData(i)->GenerateCorridor(this->dungeon);
				}
			}
			void DungeonGenerator::GeneratePlayers(int level)
			{
				if (!this->SpawnMobs) {
					return;
				}
				for (int i = 0; i < this->rooms->Count; i += 1) {
					this->rooms->GetData(i)->GenerateAIPlayers(level);
				}
				for (int j = 0; j < this->corridors->Count; j += 1) {
					this->corridors->GetData(j)->GenerateAIPlayers(level);
				}
			}
			void DungeonGenerator::PreprocessDungeonSimple()
			{
				for (int i = 1; i < this->DUNGEON_Size_X; i += 1) {
					for (int j = 1; j < this->DUNGEON_Size_Y; j += 1) {
						this->dungeon->GetData(i, j);
						if (TileConsts::isWall(this->dungeon->GetData(i, j))) {
							bool arg_68_0 = *(this->dungeon->GetData(i, j - 1)) == 0;
							bool flag = *(this->dungeon->GetData(i - 1, j)) == 0;
							bool flag2 = *(this->dungeon->GetData(i - 1, j - 1)) == 0;
							if ((arg_68_0 | flag) | flag2) {
								this->dungeon->SetData(i, j, 12);
							}
						}
					}
				}
			}
			void DungeonGenerator::PreprocessDungeon()
			{
				for (int i = 1; i < this->DUNGEON_Size_X; i += 1) {
					for (int j = 1; j < this->DUNGEON_Size_Y; j += 1) {
						char b = this->dungeon->GetData(i, j);
						if (((b != 0) && (b != 1)) && (b != 20)) {
							bool flag = TileConsts::isWall(this->dungeon->GetData(i, j + 1));
							bool flag2 = TileConsts::isWall(this->dungeon->GetData(i, j - 1));
							bool flag3 = TileConsts::isWall(this->dungeon->GetData(i - 1, j));
							bool flag4 = TileConsts::isWall(this->dungeon->GetData(i + 1, j));
							bool flag5 = TileConsts::isWall(this->dungeon->GetData(i + 1, j + 1));
							bool flag6 = TileConsts::isWall(this->dungeon->GetData(i - 1, j + 1));
							bool flag7 = TileConsts::isWall(this->dungeon->GetData(i + 1, j - 1));
							bool flag8 = TileConsts::isWall(this->dungeon->GetData(i - 1, j - 1));
							if (!flag & flag2) {
								if (((!flag4 & flag) & flag3) && !flag2) {
									this->dungeon->SetData(i, j, 17);
								}
								else {
									if (((!flag4 && !flag) & flag3) & flag2) {
										this->dungeon->SetData(i, j, 18);
									}
									else {
										if (((!flag3 && !flag) & flag4) & flag2) {
											this->dungeon->SetData(i, j, 17);
										}
										else {
											if (((((((flag3 && !flag8) && !flag2) && !flag7) && !flag4) && !flag5) & flag) && !flag6) {
												this->dungeon->SetData(i, j, 23);
											}
											else {
												if ((!flag3 & flag) & flag4) {
												}
												if (((((((flag3 && !flag8) & flag2) && !flag7) && !flag4) && !flag5) && !flag) && !flag6) {
													this->dungeon->SetData(i, j, 22);
												}
												else {
													if (((((((!flag3 && !flag8) && !flag2) && !flag7) & flag4) && !flag5) & flag) && !flag6) {
														this->dungeon->SetData(i, j, 21);
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
			String* DungeonGenerator::StringifyDungeon()
			{
				this->sb = new StringBuilder();
				for (int i = 0; i < this->DUNGEON_Size_Y; i += 1) {
					for (int j = 0; j < this->DUNGEON_Size_X; j += 1) {
						if (*(this->dungeon->GetData(j, i)) != 0) {
							this->sb->Append(j);
							this->sb->Append(new String(";"));
							this->sb->Append(i);
							this->sb->Append(new String(";"));
							this->sb->Append(this->dungeon->GetData(j, i));
							this->sb->Append(new String("|"));
						}
					}
				}
				return this->sb->ToString();
			}
			void DungeonGenerator::GenerateDebugDungeon()
			{
				this->dungeon = new ArrayND<char>(this->DUNGEON_Size_X, this->DUNGEON_Size_Y);
				for (int i = 0; i < this->DUNGEON_Size_Y; i += 1) {
					for (int j = 0; j < this->DUNGEON_Size_X; j += 1) {
						this->dungeon->SetData(j, i, 1);
					}
				}
				for (int k = 0; k < 300; k += 1) {
					this->dungeon->SetData(UnityEngine::Random::Range(0, this->DUNGEON_Size_X), UnityEngine::Random::Range(0, this->DUNGEON_Size_Y), 2);
				}
			}
			bool DungeonGenerator::CanPlaceRoom(int x, int y, int width, int height)
			{
				for (int i = y; i < (y + height); i += 1) {
					for (int j = x; j < (x + width); j += 1) {
						if (*(this->dungeon->GetData(j, i)) != 0) {
							return false;
						}
					}
				}
				return true;
			}
			void DungeonGenerator::Reset()
			{
				if (this->rooms == null) {
					return;
				}
				this->rooms->Clear();
				this->corridors->Clear();
				for (int i = 0; i < this->DUNGEON_Size_Y; i += 1) {
					for (int j = 0; j < this->DUNGEON_Size_X; j += 1) {
						this->dungeon->SetData(j, i, 0);
					}
				}
			}
			DungeonGenerator::DungeonGenerator()
			{
				SpawnMobs = true;
				DUNGEON_Size_X = 200;
				DUNGEON_Size_Y = 200;
				ROOM_MIN_SIZE = 20;
				ROOM_MAX_SIZE = 26;
				ROOM_PLACEMENT_TRIES = 100;
				CORRIDOR_SIZE = 4;
			}

		}
	}
}
