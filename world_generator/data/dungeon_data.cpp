#include "dungeon_data.h"

String DungeonData::get_target_class_name() {
	return _target_class_name;
}
void DungeonData::set_target_class_name(String name) {
	_target_class_name = name;
}

Ref<Script> DungeonData::get_target_script() {
	return _target_script;
}
void DungeonData::set_target_script(Ref<Script> script) {
	_target_script = script;
}

Vector2 DungeonData::get_level_range() {
	return _level_range;
}
void DungeonData::set_level_range(Vector2 value) {
	_level_range = value;
}

//Min Size
int DungeonData::get_min_sizex() {
	return _min_sizex;
}
void DungeonData::set_min_sizex(int value) {
	_min_sizex = value;
}

int DungeonData::get_min_sizey() {
	return _min_sizey;
}
void DungeonData::set_min_sizey(int value) {
	_min_sizey = value;
}

int DungeonData::get_min_sizez() {
	return _min_sizez;
}
void DungeonData::set_min_sizez(int value) {
	_min_sizez = value;
}

//Max Size
int DungeonData::get_max_sizex() {
	return _max_sizex;
}
void DungeonData::set_max_sizex(int value) {
	_max_sizex = value;
}

int DungeonData::get_max_sizey() {
	return _max_sizey;
}
void DungeonData::set_max_sizey(int value) {
	_max_sizey = value;
}

int DungeonData::get_max_sizez() {
	return _max_sizez;
}
void DungeonData::set_max_sizez(int value) {
	_max_sizez = value;
}

int DungeonData::get_min_room_count() {
	return _min_room_count;
}
void DungeonData::set_min_room_count(int value) {
	_min_room_count = value;
}

int DungeonData::get_max_room_count() {
	return _max_room_count;
}
void DungeonData::set_max_room_count(int value) {
	_max_room_count = value;
}

//Rooms
Ref<DungeonRoomData> DungeonData::get_dungeon_room_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeon_room_datas.size(), Ref<DungeonRoomData>());

	return _dungeon_room_datas.get(index);
}
void DungeonData::set_dungeon_room_data(const int index, const Ref<DungeonRoomData> dungeon_room_data) {
	ERR_FAIL_INDEX(index, _dungeon_room_datas.size());

	_dungeon_room_datas.set(index, dungeon_room_data);
}
void DungeonData::add_dungeon_room_data(const Ref<DungeonRoomData> dungeon_room_data) {
	_dungeon_room_datas.push_back(dungeon_room_data);
}
void DungeonData::remove_dungeon_room_data(const int index) {
	ERR_FAIL_INDEX(index, _dungeon_room_datas.size());

	_dungeon_room_datas.remove(index);
}

int DungeonData::get_dungeon_room_data_count() const {
	return _dungeon_room_datas.size();
}

Vector<Variant> DungeonData::get_dungeon_room_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _dungeon_room_datas.size(); i++) {
		r.push_back(_dungeon_room_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_dungeon_room_datas(const Vector<Variant> &dungeon_room_datas) {
	_dungeon_room_datas.clear();
	for (int i = 0; i < dungeon_room_datas.size(); i++) {
		Ref<DungeonRoomData> dungeon_room_data = Ref<DungeonRoomData>(dungeon_room_datas[i]);

		_dungeon_room_datas.push_back(dungeon_room_data);
	}
}

//Start Rooms
Ref<DungeonRoomData> DungeonData::get_dungeon_start_room_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeon_start_room_datas.size(), Ref<DungeonRoomData>());

	return _dungeon_start_room_datas.get(index);
}
void DungeonData::set_dungeon_start_room_data(const int index, const Ref<DungeonRoomData> dungeon_start_room_data) {
	ERR_FAIL_INDEX(index, _dungeon_start_room_datas.size());

	_dungeon_start_room_datas.set(index, dungeon_start_room_data);
}
void DungeonData::add_dungeon_start_room_data(const Ref<DungeonRoomData> dungeon_start_room_data) {
	_dungeon_start_room_datas.push_back(dungeon_start_room_data);
}
void DungeonData::remove_dungeon_start_room_data(const int index) {
	ERR_FAIL_INDEX(index, _dungeon_start_room_datas.size());

	_dungeon_start_room_datas.remove(index);
}

int DungeonData::get_dungeon_start_room_data_count() const {
	return _dungeon_start_room_datas.size();
}

Vector<Variant> DungeonData::get_dungeon_start_room_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _dungeon_start_room_datas.size(); i++) {
		r.push_back(_dungeon_start_room_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_dungeon_start_room_datas(const Vector<Variant> &dungeon_start_room_datas) {
	_dungeon_start_room_datas.clear();
	for (int i = 0; i < dungeon_start_room_datas.size(); i++) {
		Ref<DungeonRoomData> dungeon_start_room_data = Ref<DungeonRoomData>(dungeon_start_room_datas[i]);

		_dungeon_start_room_datas.push_back(dungeon_start_room_data);
	}
}

//End Rooms
Ref<DungeonRoomData> DungeonData::get_dungeon_end_room_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeon_end_room_datas.size(), Ref<DungeonRoomData>());

	return _dungeon_end_room_datas.get(index);
}
void DungeonData::set_dungeon_end_room_data(const int index, const Ref<DungeonRoomData> dungeon_end_room_data) {
	ERR_FAIL_INDEX(index, _dungeon_end_room_datas.size());

	_dungeon_end_room_datas.set(index, dungeon_end_room_data);
}
void DungeonData::add_dungeon_end_room_data(const Ref<DungeonRoomData> dungeon_end_room_data) {
	_dungeon_end_room_datas.push_back(dungeon_end_room_data);
}
void DungeonData::remove_dungeon_end_room_data(const int index) {
	ERR_FAIL_INDEX(index, _dungeon_end_room_datas.size());

	_dungeon_end_room_datas.remove(index);
}

int DungeonData::get_dungeon_end_room_data_count() const {
	return _dungeon_end_room_datas.size();
}

Vector<Variant> DungeonData::get_dungeon_end_room_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _dungeon_end_room_datas.size(); i++) {
		r.push_back(_dungeon_end_room_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_dungeon_end_room_datas(const Vector<Variant> &dungeon_end_room_datas) {
	_dungeon_end_room_datas.clear();
	for (int i = 0; i < dungeon_end_room_datas.size(); i++) {
		Ref<DungeonRoomData> dungeon_end_room_data = Ref<DungeonRoomData>(dungeon_end_room_datas[i]);

		_dungeon_end_room_datas.push_back(dungeon_end_room_data);
	}
}

//Corridors
Ref<DungeonRoomData> DungeonData::get_dungeon_corridor_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeon_corridor_datas.size(), Ref<DungeonRoomData>());

	return _dungeon_corridor_datas.get(index);
}
void DungeonData::set_dungeon_corridor_data(const int index, const Ref<DungeonRoomData> dungeon_corridor_data) {
	ERR_FAIL_INDEX(index, _dungeon_corridor_datas.size());

	_dungeon_corridor_datas.set(index, dungeon_corridor_data);
}
void DungeonData::add_dungeon_corridor_data(const Ref<DungeonRoomData> dungeon_corridor_data) {
	_dungeon_corridor_datas.push_back(dungeon_corridor_data);
}
void DungeonData::remove_dungeon_corridor_data(const int index) {
	ERR_FAIL_INDEX(index, _dungeon_corridor_datas.size());

	_dungeon_corridor_datas.remove(index);
}

int DungeonData::get_dungeon_corridor_data_count() const {
	return _dungeon_corridor_datas.size();
}

Vector<Variant> DungeonData::get_dungeon_corridor_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _dungeon_corridor_datas.size(); i++) {
		r.push_back(_dungeon_corridor_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_dungeon_corridor_datas(const Vector<Variant> &dungeon_corridor_datas) {
	_dungeon_corridor_datas.clear();
	for (int i = 0; i < dungeon_corridor_datas.size(); i++) {
		Ref<DungeonRoomData> dungeon_corridor_data = Ref<DungeonRoomData>(dungeon_corridor_datas[i]);

		_dungeon_corridor_datas.push_back(dungeon_corridor_data);
	}
}

//Entities
Ref<EntityData> DungeonData::get_entity_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _entity_datas.size(), Ref<EntityData>());

	return _entity_datas.get(index);
}
void DungeonData::set_entity_data(const int index, const Ref<EntityData> entity_data) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.set(index, entity_data);
}
void DungeonData::add_entity_data(const Ref<EntityData> entity_data) {
	_entity_datas.push_back(entity_data);
}
void DungeonData::remove_entity_data(const int index) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.remove(index);
}
int DungeonData::get_entity_data_count() const {
	return _entity_datas.size();
}

Vector<Variant> DungeonData::get_entity_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _entity_datas.size(); i++) {
		r.push_back(_entity_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_entity_datas(const Vector<Variant> &entity_datas) {
	_entity_datas.clear();
	for (int i = 0; i < entity_datas.size(); i++) {
		Ref<EntityData> entity_data = Ref<EntityData>(entity_datas[i]);

		_entity_datas.push_back(entity_data);
	}
}

////    Surfaces    ////
Ref<VoxelSurface> DungeonData::get_voxel_surface(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>());

	return _voxel_surfaces.get(index);
}
void DungeonData::set_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface) {
	ERR_FAIL_INDEX(index, _voxel_surfaces.size());

	_voxel_surfaces.set(index, voxel_surface);
}
void DungeonData::add_voxel_surface(const Ref<VoxelSurface> voxel_surface) {
	_voxel_surfaces.push_back(voxel_surface);
}
void DungeonData::remove_voxel_surface(const int index) {
	ERR_FAIL_INDEX(index, _voxel_surfaces.size());

	_voxel_surfaces.remove(index);
}
int DungeonData::get_voxel_surface_count() const {
	return _voxel_surfaces.size();
}

Vector<Variant> DungeonData::get_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		r.push_back(_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_voxel_surfaces(const Vector<Variant> &voxel_surfaces) {
	_voxel_surfaces.clear();
	for (int i = 0; i < voxel_surfaces.size(); i++) {
		Ref<EnvironmentData> voxel_surface = Ref<EnvironmentData>(voxel_surfaces[i]);

		_voxel_surfaces.push_back(voxel_surface);
	}
}

////    Liquid Surfaces    ////
Ref<VoxelSurface> DungeonData::get_liquid_voxel_surface(const int index) const {
	ERR_FAIL_INDEX_V(index, _liquid_voxel_surfaces.size(), Ref<VoxelSurface>());

	return _liquid_voxel_surfaces.get(index);
}
void DungeonData::set_liquid_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface) {
	ERR_FAIL_INDEX(index, _liquid_voxel_surfaces.size());

	_liquid_voxel_surfaces.set(index, voxel_surface);
}
void DungeonData::add_liquid_voxel_surface(const Ref<VoxelSurface> voxel_surface) {
	_liquid_voxel_surfaces.push_back(voxel_surface);
}
void DungeonData::remove_liquid_voxel_surface(const int index) {
	ERR_FAIL_INDEX(index, _liquid_voxel_surfaces.size());

	_liquid_voxel_surfaces.remove(index);
}
int DungeonData::get_liquid_voxel_surface_count() const {
	return _liquid_voxel_surfaces.size();
}

Vector<Variant> DungeonData::get_liquid_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _liquid_voxel_surfaces.size(); i++) {
		r.push_back(_liquid_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_liquid_voxel_surfaces(const Vector<Variant> &voxel_surfaces) {
	_liquid_voxel_surfaces.clear();
	for (int i = 0; i < voxel_surfaces.size(); i++) {
		Ref<EnvironmentData> voxel_surface = Ref<EnvironmentData>(voxel_surfaces[i]);

		_liquid_voxel_surfaces.push_back(voxel_surface);
	}
}

//Environments
Ref<EnvironmentData> DungeonData::get_environment_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _environment_datas.size(), Ref<EnvironmentData>());

	return _environment_datas.get(index);
}
void DungeonData::set_environment_data(const int index, const Ref<EnvironmentData> environment_data) {
	ERR_FAIL_INDEX(index, _environment_datas.size());

	_environment_datas.set(index, environment_data);
}
void DungeonData::add_environment_data(const Ref<EnvironmentData> environment_data) {
	_environment_datas.push_back(environment_data);
}
void DungeonData::remove_environment_data(const int index) {
	ERR_FAIL_INDEX(index, _environment_datas.size());

	_environment_datas.remove(index);
}
int DungeonData::get_environment_data_count() const {
	return _environment_datas.size();
}

Vector<Variant> DungeonData::get_environment_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _environment_datas.size(); i++) {
		r.push_back(_environment_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonData::set_environment_datas(const Vector<Variant> &environment_datas) {
	_environment_datas.clear();
	for (int i = 0; i < environment_datas.size(); i++) {
		Ref<EnvironmentData> environment_data = Ref<EnvironmentData>(environment_datas[i]);

		_environment_datas.push_back(environment_data);
	}
}

DungeonData::DungeonData() {
	_min_sizex = 0;
	_min_sizey = 0;
	_min_sizez = 0;

	_max_sizex = 0;
	_max_sizey = 0;
	_max_sizez = 0;

	_min_room_count = 0;
	_max_room_count = 0;
}
DungeonData::~DungeonData() {
	_environment_datas.clear();

	_dungeon_room_datas.clear();
	_dungeon_start_room_datas.clear();
	_dungeon_end_room_datas.clear();
	_dungeon_corridor_datas.clear();
	_entity_datas.clear();

	_entity_datas.clear();
	_voxel_surfaces.clear();
	_liquid_voxel_surfaces.clear();
}

void DungeonData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_class_name"), &DungeonData::get_target_class_name);
	ClassDB::bind_method(D_METHOD("set_target_class_name", "value"), &DungeonData::set_target_class_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "target_class_name"), "set_target_class_name", "get_target_class_name");

	ClassDB::bind_method(D_METHOD("get_target_script"), &DungeonData::get_target_script);
	ClassDB::bind_method(D_METHOD("set_target_script", "value"), &DungeonData::set_target_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PROPERTY_HINT_RESOURCE_TYPE, "Script"), "set_target_script", "get_target_script");

	ClassDB::bind_method(D_METHOD("get_level_range"), &DungeonData::get_level_range);
	ClassDB::bind_method(D_METHOD("set_level_range", "value"), &DungeonData::set_level_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "level_range"), "set_level_range", "get_level_range");

	//Min Size
	ClassDB::bind_method(D_METHOD("get_min_sizex"), &DungeonData::get_min_sizex);
	ClassDB::bind_method(D_METHOD("set_min_sizex", "value"), &DungeonData::set_min_sizex);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_sizex"), "set_min_sizex", "get_min_sizex");

	ClassDB::bind_method(D_METHOD("get_min_sizey"), &DungeonData::get_min_sizey);
	ClassDB::bind_method(D_METHOD("set_min_sizey", "value"), &DungeonData::set_min_sizey);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_sizey"), "set_min_sizey", "get_min_sizey");

	ClassDB::bind_method(D_METHOD("get_min_sizez"), &DungeonData::get_min_sizez);
	ClassDB::bind_method(D_METHOD("set_min_sizez", "value"), &DungeonData::set_min_sizez);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_sizez"), "set_min_sizez", "get_min_sizez");

	//Max Size
	ClassDB::bind_method(D_METHOD("get_max_sizex"), &DungeonData::get_max_sizex);
	ClassDB::bind_method(D_METHOD("set_max_sizex", "value"), &DungeonData::set_max_sizex);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_sizex"), "set_max_sizex", "get_max_sizex");

	ClassDB::bind_method(D_METHOD("get_max_sizey"), &DungeonData::get_max_sizey);
	ClassDB::bind_method(D_METHOD("set_max_sizey", "value"), &DungeonData::set_max_sizey);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_sizey"), "set_max_sizey", "get_max_sizey");

	ClassDB::bind_method(D_METHOD("get_max_sizez"), &DungeonData::get_max_sizez);
	ClassDB::bind_method(D_METHOD("set_max_sizez", "value"), &DungeonData::set_max_sizez);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_sizez"), "set_max_sizez", "get_max_sizez");

	//Room Count
	ClassDB::bind_method(D_METHOD("get_min_room_count"), &DungeonData::get_min_room_count);
	ClassDB::bind_method(D_METHOD("set_min_room_count", "value"), &DungeonData::set_min_room_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_room_count"), "set_min_room_count", "get_min_room_count");

	ClassDB::bind_method(D_METHOD("get_max_room_count"), &DungeonData::get_max_room_count);
	ClassDB::bind_method(D_METHOD("set_max_room_count", "value"), &DungeonData::set_max_room_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_room_count"), "set_max_room_count", "get_max_room_count");

	//Rooms
	ClassDB::bind_method(D_METHOD("get_dungeon_room_data", "index"), &DungeonData::get_dungeon_room_data);
	ClassDB::bind_method(D_METHOD("set_dungeon_room_data", "index", "data"), &DungeonData::set_dungeon_room_data);
	ClassDB::bind_method(D_METHOD("add_dungeon_room_data", "dungeon_room_data"), &DungeonData::add_dungeon_room_data);
	ClassDB::bind_method(D_METHOD("remove_dungeon_room_data", "index"), &DungeonData::remove_dungeon_room_data);
	ClassDB::bind_method(D_METHOD("get_dungeon_room_data_count"), &DungeonData::get_dungeon_room_data_count);

	ClassDB::bind_method(D_METHOD("get_dungeon_room_datas"), &DungeonData::get_dungeon_room_datas);
	ClassDB::bind_method(D_METHOD("set_dungeon_room_datas", "dungeon_room_datas"), &DungeonData::set_dungeon_room_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dungeon_room_datas", PROPERTY_HINT_NONE, "17/17:DungeonRoomData", PROPERTY_USAGE_DEFAULT, "DungeonRoomData"), "set_dungeon_room_datas", "get_dungeon_room_datas");

	//Start Rooms
	ClassDB::bind_method(D_METHOD("get_dungeon_start_room_data", "index"), &DungeonData::get_dungeon_start_room_data);
	ClassDB::bind_method(D_METHOD("set_dungeon_start_room_data", "index", "data"), &DungeonData::set_dungeon_start_room_data);
	ClassDB::bind_method(D_METHOD("add_dungeon_start_room_data", "dungeon_start_room_data"), &DungeonData::add_dungeon_start_room_data);
	ClassDB::bind_method(D_METHOD("remove_dungeon_start_room_data", "index"), &DungeonData::remove_dungeon_start_room_data);
	ClassDB::bind_method(D_METHOD("get_dungeon_start_room_data_count"), &DungeonData::get_dungeon_start_room_data_count);

	ClassDB::bind_method(D_METHOD("get_dungeon_start_room_datas"), &DungeonData::get_dungeon_start_room_datas);
	ClassDB::bind_method(D_METHOD("set_dungeon_start_room_datas", "dungeon_start_room_datas"), &DungeonData::set_dungeon_start_room_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dungeon_start_room_datas", PROPERTY_HINT_NONE, "17/17:DungeonRoomData", PROPERTY_USAGE_DEFAULT, "DungeonRoomData"), "set_dungeon_start_room_datas", "get_dungeon_start_room_datas");

	//End Rooms
	ClassDB::bind_method(D_METHOD("get_dungeon_end_room_data", "index"), &DungeonData::get_dungeon_end_room_data);
	ClassDB::bind_method(D_METHOD("set_dungeon_end_room_data", "index", "data"), &DungeonData::set_dungeon_end_room_data);
	ClassDB::bind_method(D_METHOD("add_dungeon_end_room_data", "dungeon_end_room_data"), &DungeonData::add_dungeon_end_room_data);
	ClassDB::bind_method(D_METHOD("remove_dungeon_end_room_data", "index"), &DungeonData::remove_dungeon_end_room_data);
	ClassDB::bind_method(D_METHOD("get_dungeon_end_room_data_count"), &DungeonData::get_dungeon_end_room_data_count);

	ClassDB::bind_method(D_METHOD("get_dungeon_end_room_datas"), &DungeonData::get_dungeon_end_room_datas);
	ClassDB::bind_method(D_METHOD("set_dungeon_end_room_datas", "dungeon_end_room_datas"), &DungeonData::set_dungeon_end_room_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dungeon_end_room_datas", PROPERTY_HINT_NONE, "17/17:DungeonRoomData", PROPERTY_USAGE_DEFAULT, "DungeonRoomData"), "set_dungeon_end_room_datas", "get_dungeon_end_room_datas");

	//Corridors
	ClassDB::bind_method(D_METHOD("get_dungeon_corridor_data", "index"), &DungeonData::get_dungeon_corridor_data);
	ClassDB::bind_method(D_METHOD("set_dungeon_corridor_data", "index", "data"), &DungeonData::set_dungeon_corridor_data);
	ClassDB::bind_method(D_METHOD("add_dungeon_corridor_data", "dungeon_corridor_data"), &DungeonData::add_dungeon_corridor_data);
	ClassDB::bind_method(D_METHOD("remove_dungeon_corridor_data", "index"), &DungeonData::remove_dungeon_corridor_data);
	ClassDB::bind_method(D_METHOD("get_dungeon_corridor_data_count"), &DungeonData::get_dungeon_corridor_data_count);

	ClassDB::bind_method(D_METHOD("get_dungeon_corridor_datas"), &DungeonData::get_dungeon_corridor_datas);
	ClassDB::bind_method(D_METHOD("set_dungeon_corridor_datas", "dungeon_corridor_datas"), &DungeonData::set_dungeon_corridor_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dungeon_corridor_datas", PROPERTY_HINT_NONE, "17/17:DungeonCorridorData", PROPERTY_USAGE_DEFAULT, "DungeonCorridorData"), "set_dungeon_corridor_datas", "get_dungeon_corridor_datas");

	//Entities
	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &DungeonData::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &DungeonData::set_entity_data);
	ClassDB::bind_method(D_METHOD("add_entity_data", "entity_data"), &DungeonData::add_entity_data);
	ClassDB::bind_method(D_METHOD("remove_entity_data", "index"), &DungeonData::remove_entity_data);
	ClassDB::bind_method(D_METHOD("get_entity_data_count"), &DungeonData::get_entity_data_count);

	ClassDB::bind_method(D_METHOD("get_entity_datas"), &DungeonData::get_entity_datas);
	ClassDB::bind_method(D_METHOD("set_entity_datas", "entity_datas"), &DungeonData::set_entity_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "entity_datas", PROPERTY_HINT_NONE, "17/17:EntityData", PROPERTY_USAGE_DEFAULT, "EntityData"), "set_entity_datas", "get_entity_datas");

	//Environments
	ClassDB::bind_method(D_METHOD("get_environment_data", "index"), &DungeonData::get_environment_data);
	ClassDB::bind_method(D_METHOD("set_environment_data", "index", "data"), &DungeonData::set_environment_data);
	ClassDB::bind_method(D_METHOD("add_environment_data", "environment_data"), &DungeonData::add_environment_data);
	ClassDB::bind_method(D_METHOD("remove_environment_data", "index"), &DungeonData::remove_environment_data);
	ClassDB::bind_method(D_METHOD("get_environment_data_count"), &DungeonData::get_environment_data_count);

	ClassDB::bind_method(D_METHOD("get_environment_datas"), &DungeonData::get_environment_datas);
	ClassDB::bind_method(D_METHOD("set_environment_datas", "environment_datas"), &DungeonData::set_environment_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "environment_datas", PROPERTY_HINT_NONE, "17/17:EnvironmentData", PROPERTY_USAGE_DEFAULT, "EnvironmentData"), "set_environment_datas", "get_environment_datas");

	//Surfaces
	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &DungeonData::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "data"), &DungeonData::set_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_voxel_surface", "voxel_surface"), &DungeonData::add_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_voxel_surface", "index"), &DungeonData::remove_voxel_surface);
	ClassDB::bind_method(D_METHOD("get_voxel_surface_count"), &DungeonData::get_voxel_surface_count);

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &DungeonData::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces", "voxel_surfaces"), &DungeonData::set_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_voxel_surfaces", "get_voxel_surfaces");

	//Liquid Surfaces
	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface", "index"), &DungeonData::get_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surface", "index", "data"), &DungeonData::set_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_liquid_voxel_surface", "voxel_surface"), &DungeonData::add_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_liquid_voxel_surface", "index"), &DungeonData::remove_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface_count"), &DungeonData::get_liquid_voxel_surface_count);

	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surfaces"), &DungeonData::get_liquid_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surfaces", "voxel_surfaces"), &DungeonData::set_liquid_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_liquid_voxel_surfaces", "get_liquid_voxel_surfaces");
}
