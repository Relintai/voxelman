#ifndef DUNGEON_DATA_H
#define DUNGEON_DATA_H

#include "core/resource.h"
#include "core/vector.h"
#include "core/ustring.h"
#include "core/script_language.h"

#include "../main/dungeon.h"
#include "dungeon_room_data.h"
#include "dungeon_corridor_data.h"
#include "../../world/environment_data.h"

#include "../../../entity_spell_system/entities/data/entity_data.h"
#include "../../library/voxel_surface.h"

class DungeonData : public Resource {
	GDCLASS(DungeonData, Resource);
	
public:
	String get_target_class_name();
	void set_target_class_name(String name);

	Ref<Script> get_target_script();
	void set_target_script(Ref<Script> script);

	Vector2 get_level_range();
	void set_level_range(Vector2 value);

	//Min Size
	int get_min_sizex();
	void set_min_sizex(int value);

	int get_min_sizey();
	void set_min_sizey(int value);

	int get_min_sizez();
	void set_min_sizez(int value);

	//Max Size
	int get_max_sizex();
	void set_max_sizex(int value);

	int get_max_sizey();
	void set_max_sizey(int value);

	int get_max_sizez();
	void set_max_sizez(int value);

	//Room Count
	int get_min_room_count();
	void set_min_room_count(int value);

	int get_max_room_count();
	void set_max_room_count(int value);

	//Environments
	Ref<EnvironmentData> get_environment_data(const int index) const;
	void set_environment_data(const int index, const Ref<EnvironmentData> environment_data);
	void add_environment_data(const Ref<EnvironmentData> environment_data);
	void remove_environment_data(const int index);
	int get_environment_data_count() const;

	Vector<Variant> get_environment_datas();
	void set_environment_datas(const Vector<Variant> &environment_datas);

	//Rooms
	Ref<DungeonRoomData> get_dungeon_room_data(const int index) const;
	void set_dungeon_room_data(const int index, const Ref<DungeonRoomData> dungeon_room_data);
	void add_dungeon_room_data(const Ref<DungeonRoomData> dungeon_room_data);
	void remove_dungeon_room_data(const int index);
	int get_dungeon_room_data_count() const;

	Vector<Variant> get_dungeon_room_datas();
	void set_dungeon_room_datas(const Vector<Variant> &dungeon_room_datas);

	//Start Rooms
	Ref<DungeonRoomData> get_dungeon_start_room_data(const int index) const;
	void set_dungeon_start_room_data(const int index, const Ref<DungeonRoomData> dungeon_start_room_data);
	void add_dungeon_start_room_data(const Ref<DungeonRoomData> dungeon_start_room_data);
	void remove_dungeon_start_room_data(const int index);
	int get_dungeon_start_room_data_count() const;

	Vector<Variant> get_dungeon_start_room_datas();
	void set_dungeon_start_room_datas(const Vector<Variant> &dungeon_start_room_datas);

	//End Rooms
	Ref<DungeonRoomData> get_dungeon_end_room_data(const int index) const;
	void set_dungeon_end_room_data(const int index, const Ref<DungeonRoomData> dungeon_end_room_data);
	void add_dungeon_end_room_data(const Ref<DungeonRoomData> dungeon_end_room_data);
	void remove_dungeon_end_room_data(const int index);
	int get_dungeon_end_room_data_count() const;

	Vector<Variant> get_dungeon_end_room_datas();
	void set_dungeon_end_room_datas(const Vector<Variant> &dungeon_end_room_datas);

	//Corridors
	Ref<DungeonRoomData> get_dungeon_corridor_data(const int index) const;
	void set_dungeon_corridor_data(const int index, const Ref<DungeonRoomData> dungeon_corridor_data);
	void add_dungeon_corridor_data(const Ref<DungeonRoomData> dungeon_corridor_data);
	void remove_dungeon_corridor_data(const int index);
	int get_dungeon_corridor_data_count() const;

	Vector<Variant> get_dungeon_corridor_datas();
	void set_dungeon_corridor_datas(const Vector<Variant> &dungeon_corridor_datas);

	//Entities
	Ref<EntityData> get_entity_data(const int index) const;
	void set_entity_data(const int index, const Ref<EntityData> entity_data);
	void add_entity_data(const Ref<EntityData> entity_data);
	void remove_entity_data(const int index);
	int get_entity_data_count() const;

	Vector<Variant> get_entity_datas();
	void set_entity_datas(const Vector<Variant> &entity_datas);

	//Surfaces
	Ref<VoxelSurface> get_voxel_surface(const int index) const;
	void set_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface);
	void add_voxel_surface(const Ref<VoxelSurface> voxel_surface);
	void remove_voxel_surface(const int index);
	int get_voxel_surface_count() const;

	Vector<Variant> get_voxel_surfaces();
	void set_voxel_surfaces(const Vector<Variant> &voxel_surfaces);

	//Liquid Surfaces
	Ref<VoxelSurface> get_liquid_voxel_surface(const int index) const;
	void set_liquid_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface);
	void add_liquid_voxel_surface(const Ref<VoxelSurface> voxel_surface);
	void remove_liquid_voxel_surface(const int index);
	int get_liquid_voxel_surface_count() const;

	Vector<Variant> get_liquid_voxel_surfaces();
	void set_liquid_voxel_surfaces(const Vector<Variant> &voxel_surfaces);

	DungeonData();
	~DungeonData();

protected:
	static void _bind_methods();

private:
	String _target_class_name;
	Ref<Script> _target_script;

	Vector2 _level_range;

	int _min_sizex;
	int _min_sizey;
	int _min_sizez;

	int _max_sizex;
	int _max_sizey;
	int _max_sizez;

	int _min_room_count;
	int _max_room_count;

	Vector<Ref<EnvironmentData> > _environment_datas;

	Vector<Ref<DungeonRoomData> > _dungeon_room_datas;
	Vector<Ref<DungeonRoomData> > _dungeon_start_room_datas;
	Vector<Ref<DungeonRoomData> > _dungeon_end_room_datas;
	Vector<Ref<DungeonRoomData> > _dungeon_corridor_datas;
	Vector<Ref<EntityData> > _entity_datas;
	Vector<Ref<VoxelSurface> > _voxel_surfaces;
	Vector<Ref<VoxelSurface> > _liquid_voxel_surfaces;
};

#endif
