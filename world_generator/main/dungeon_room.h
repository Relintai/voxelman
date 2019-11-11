#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include "core/reference.h"
#include "core/vector.h"

#include "../../world/voxel_chunk.h"
#include "../../world/voxel_structure.h"
#include "../data/world_generator_prop_data.h"
#include "../../world/environment_data.h"
#include "../../../entity_spell_system/entities/data/entity_data.h"

#include "../data/dungeon_room_data.h"
#include "../../library/voxelman_library.h"

class DungeonRoomData;

class DungeonRoom : public Reference {
	GDCLASS(DungeonRoom, Reference);
	
public:
	int get_current_seed();
	void set_current_seed(int value);

	Vector2 get_level_range();
	void set_level_range(Vector2 value);

	//Pos
	int get_posx();
	void set_posx(int value);

	int get_posy();
	void set_posy(int value);

	int get_posz();
	void set_posz(int value);

	//Size
	int get_sizex();
	void set_sizex(int value);

	int get_sizey();
	void set_sizey(int value);

	int get_sizez();
	void set_sizez(int value);

	//Environment
	Ref<EnvironmentData> get_environment();
	void set_environment(Ref<EnvironmentData> value);

	Ref<DungeonRoomData> get_data();
	void set_data(Ref<DungeonRoomData> value);

	//Structure
	Ref<VoxelStructure> get_structure();
	void set_structure(Ref<VoxelStructure> structure);

	//Props
	Ref<WorldGeneratorPropData> get_prop_data(const int index) const;
	void set_prop_data(const int index, const Ref<WorldGeneratorPropData> prop_data);
	void add_prop_data(const Ref<WorldGeneratorPropData> prop_data);
	void remove_prop_data(const int index);
	int get_prop_data_count() const;

	//Entities
	Ref<EntityData> get_entity_data(const int index) const;
	void set_entity_data(const int index, const Ref<EntityData> entity_data);
	void add_entity_data(const Ref<EntityData> entity_data);
	void remove_entity_data(const int index);
	int get_entity_data_count() const;

	void setup();
	void setup_library(Ref<VoxelmanLibrary> library);
	void _setup_library(Ref<VoxelmanLibrary> library);

	void generate_chunk(VoxelChunk *chunk, bool spawn_mobs);
	void generate_chunk_bind(Node *chunk, bool spawn_mobs);
	void generate_room(Ref<VoxelStructure> structure, bool spawn_mobs);

	DungeonRoom();
	~DungeonRoom();

protected:
	static void _bind_methods();

private:
	int _current_seed;

	Vector2 _level_range;

	int _posx;
	int _posy;
	int _posz;

	int _sizex;
	int _sizey;
	int _sizez;

	Ref<EnvironmentData> _environment;
	Ref<DungeonRoomData> _data;
	Ref<VoxelStructure> _structure;
	Vector<Ref<WorldGeneratorPropData> > _prop_datas;
	Vector<Ref<EntityData> > _entity_datas;
};

#endif
