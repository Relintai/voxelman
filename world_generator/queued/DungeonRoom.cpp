#include "DungeonRoom.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			Array<String>* DungeonRoom::biga;
			Array<String>* DungeonRoom::bigb;
			Array<String>* DungeonRoom::smalla;
			Array<String>* DungeonRoom::smallb;
			int DungeonRoom::getX(){
				return this->x;
			}
			void DungeonRoom::setX(int value)
			{
				this->x = value;
			}
			int DungeonRoom::getY()
			{
				return this->y;
			}
			void DungeonRoom::setY(int value)
			{
				this->y = value;
			}
			int DungeonRoom::getWidth()
			{
				return this->width;
			}
			void DungeonRoom::setWidth(int value)
			{
				this->width = value;
			}
			int DungeonRoom::getHeight()
			{
				return this->height;
			}
			void DungeonRoom::setHeight(int value)
			{
				this->height = value;
			}
			DungeonRoom::DungeonRoom(int x, int y, int width, int height)
			{
				this->setX(x);
				this->setY(y);
				this->setWidth(width);
				this->setHeight(height);
			}
			void DungeonRoom::WriteRoom(ArrayND<char, 2>* dungeon)
			{
				for (int i = this->getY(); i < (this->getY() + this->getHeight()); i += 1) {
					for (int j = this->getX(); j < (this->getX() + this->getWidth()); j += 1) {
						dungeon->SetData(j, i, 1);
					}
				}
				for (int k = this->getX() + 1; k < ((this->getX() + this->getWidth()) - 1); k += 1) {
					dungeon->SetData(k, (this->getY() + this->getHeight()) - 1, 11);
				}
				for (int l = this->getX() + 1; l < ((this->getX() + this->getWidth()) - 1); l += 1) {
					dungeon->SetData(l, this->getY(), 13);
				}
				for (int m = this->getY() + 1; m < ((this->getY() + this->getHeight()) - 1); m += 1) {
					dungeon->SetData(this->getX(), m, 14);
				}
				for (int n = this->getY() + 1; n < ((this->getY() + this->getHeight()) - 1); n += 1) {
					dungeon->SetData((this->getX() + this->getWidth()) - 1, n, 12);
				}
				dungeon->SetData(this->getX(), this->getY(), 13);
				dungeon->SetData((this->getX() + this->getWidth()) - 1, this->getY(), 13);
				dungeon->SetData(this->getX(), (this->getY() + this->getHeight()) - 1, 13);
				dungeon->SetData((this->getX() + this->getWidth()) - 1, (this->getY() + this->getHeight()) - 1, 13);
			}
			void DungeonRoom::GenerateAIPlayers(int level)
			{
				int num = UnityEngine::Random::Range(3, 5);
				for (int i = 0; i < num; i += 1) {
					int num2 = this->getX() + UnityEngine::Random::Range(1, this->getWidth() - 2);
					int num3 = this->getY() + UnityEngine::Random::Range(1, this->getHeight() - 2);
					Vector3* position = new Vector3((float)(num2) * Voxelizzer::world->voxelScale, Voxelizzer::world->voxelScale, (float)(num3) * Voxelizzer::world->voxelScale);
					Entity::SSpawn(EntityType::Mob, UnityEngine::Random::Range(10000, 10005), level, this->GenerateName(), 0u, null, false, position, );
				}
			}
			String* DungeonRoom::GenerateName()
			{
				UnityEngine::Random::State* state = UnityEngine::Random::state;
				String* text = DungeonRoom::biga->GetData(UnityEngine::Random::Range(0, DungeonRoom::biga->Length));
				bool flag = false;
				int num = UnityEngine::Random::Range(3, 7);
				for (int i = 0; i < num; i += 1) {
					if (flag) {
						*text += DungeonRoom::smallb->GetData(UnityEngine::Random::Range(0, DungeonRoom::smallb->Length));
					}
					else {
						*text += DungeonRoom::smalla->GetData(UnityEngine::Random::Range(0, DungeonRoom::smalla->Length));
					}
					flag = !flag;
				}
				UnityEngine::Random::state = state;
				return text;
			}
			DungeonRoom::DungeonRoom()
			{
				Array<String>* expr_06 = new Array<String>(5);
				expr_06->SetData(0, new String("A"));
				expr_06->SetData(1, new String("E"));
				expr_06->SetData(2, new String("I"));
				expr_06->SetData(3, new String("O"));
				expr_06->SetData(4, new String("U"));
				DungeonRoom::biga = expr_06;
				Array<String>* expr_3A = new Array<String>(19);
				expr_3A->SetData(0, new String("B"));
				expr_3A->SetData(1, new String("C"));
				expr_3A->SetData(2, new String("D"));
				expr_3A->SetData(3, new String("E"));
				expr_3A->SetData(4, new String("F"));
				expr_3A->SetData(5, new String("G"));
				expr_3A->SetData(6, new String("H"));
				expr_3A->SetData(7, new String("J"));
				expr_3A->SetData(8, new String("K"));
				expr_3A->SetData(9, new String("L"));
				expr_3A->SetData(10, new String("M"));
				expr_3A->SetData(11, new String("N"));
				expr_3A->SetData(12, new String("P"));
				expr_3A->SetData(13, new String("Q"));
				expr_3A->SetData(14, new String("R"));
				expr_3A->SetData(15, new String("V"));
				expr_3A->SetData(16, new String("X"));
				expr_3A->SetData(17, new String("Y"));
				expr_3A->SetData(18, new String("Z"));
				DungeonRoom::bigb = expr_3A;
				Array<String>* expr_E7 = new Array<String>(5);
				expr_E7->SetData(0, new String("a"));
				expr_E7->SetData(1, new String("e"));
				expr_E7->SetData(2, new String("i"));
				expr_E7->SetData(3, new String("o"));
				expr_E7->SetData(4, new String("u"));
				DungeonRoom::smalla = expr_E7;
				Array<String>* expr_11B = new Array<String>(19);
				expr_11B->SetData(0, new String("b"));
				expr_11B->SetData(1, new String("c"));
				expr_11B->SetData(2, new String("d"));
				expr_11B->SetData(3, new String("e"));
				expr_11B->SetData(4, new String("f"));
				expr_11B->SetData(5, new String("g"));
				expr_11B->SetData(6, new String("h"));
				expr_11B->SetData(7, new String("j"));
				expr_11B->SetData(8, new String("k"));
				expr_11B->SetData(9, new String("l"));
				expr_11B->SetData(10, new String("m"));
				expr_11B->SetData(11, new String("n"));
				expr_11B->SetData(12, new String("p"));
				expr_11B->SetData(13, new String("q"));
				expr_11B->SetData(14, new String("r"));
				expr_11B->SetData(15, new String("v"));
				expr_11B->SetData(16, new String("x"));
				expr_11B->SetData(17, new String("y"));
				expr_11B->SetData(18, new String("z"));
				DungeonRoom::smallb = expr_11B;
			}

		}
	}
}
