#ifndef DUNGEON_H
#define DUNGEON_H

#include "core/reference.h"

#include "dungeon_room.h"
#include "dungeon_corridor.h"

#include "../../world/voxel_chunk.h"
#include "../../world/voxel_structure.h"
#include "../../world/environment_data.h"

#include "../../../entity_spell_system/entities/data/entity_data.h"

#include "../data/dungeon_data.h"
#include "../../library/voxelman_library.h"

class DungeonData;

class Dungeon : public Reference {
	GDCLASS(Dungeon, Reference);
	
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

	//Room Count
	int get_room_count();
	void set_room_count(int value);

	//Environment
	Ref<EnvironmentData> get_environment();
	void set_environment(Ref<EnvironmentData> value);

	Ref<DungeonData> get_data();
	void set_data(Ref<DungeonData> value);

	//Rooms
	Ref<DungeonRoom> get_dungeon_room(const int index) const;
	void set_dungeon_room(const int index, const Ref<DungeonRoom> dungeon_room);
	void add_dungeon_room(const Ref<DungeonRoom> dungeon_room);
	void remove_dungeon_room(const int index);
	int get_dungeon_room_count() const;

	//Start Rooms
	Ref<DungeonRoom> get_dungeon_start_room(const int index) const;
	void set_dungeon_start_room(const int index, const Ref<DungeonRoom> dungeon_start_room);
	void add_dungeon_start_room(const Ref<DungeonRoom> dungeon_start_room);
	void remove_dungeon_start_room(const int index);
	int get_dungeon_start_room_count() const;

	//End Rooms
	Ref<DungeonRoom> get_dungeon_end_room(const int index) const;
	void set_dungeon_end_room(const int index, const Ref<DungeonRoom> dungeon_end_room);
	void add_dungeon_end_room(const Ref<DungeonRoom> dungeon_end_room);
	void remove_dungeon_end_room(const int index);
	int get_dungeon_end_room_count() const;

	//Corridors
	Ref<DungeonCorridor> get_dungeon_corridor(const int index) const;
	void set_dungeon_corridor(const int index, const Ref<DungeonCorridor> dungeon_corridors);
	void add_dungeon_corridor(const Ref<DungeonCorridor> dungeon_corridors);
	void remove_dungeon_corridor(const int index);
	int get_dungeon_corridor_count() const;

	//Entities
	Ref<EntityData> get_entity_data(const int index) const;
	void set_entity_data(const int index, const Ref<EntityData> entity_datas);
	void add_entity_data(const Ref<EntityData> entity_datas);
	void remove_entity_data(const int index);
	int get_entity_data_count() const;

	void setup();
	void setup_library(Ref<VoxelmanLibrary> library);
	void _setup_library(Ref<VoxelmanLibrary> library);

	void generate_chunk(VoxelChunk *chunk, bool spawn_mobs);
	void generate_chunk_bind(Node *chunk, bool spawn_mobs);
	void generate_structure(Ref<VoxelStructure> structure, bool spawn_mobs);

	Ref<Image> generate_map();

	Dungeon();
	~Dungeon();

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

	int _room_count;

	Ref<EnvironmentData> _environment;
	Ref<DungeonData> _data;
	Vector<Ref<DungeonRoom> > _dungeon_rooms;
	Vector<Ref<DungeonRoom> > _dungeon_start_rooms;
	Vector<Ref<DungeonRoom> > _dungeon_end_rooms;
	Vector<Ref<DungeonCorridor> > _dungeon_corridors;
	Vector<Ref<EntityData> > _entity_datas;
};

#endif
