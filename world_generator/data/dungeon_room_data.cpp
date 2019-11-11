#include "dungeon_room_data.h"

String DungeonRoomData::get_target_class_name() {
	return _target_class_name;
}
void DungeonRoomData::set_target_class_name(String name) {
	_target_class_name = name;
}

Ref<Script> DungeonRoomData::get_target_script() {
	return _target_script;
}
void DungeonRoomData::set_target_script(Ref<Script> script) {
	_target_script = script;
}

Vector2 DungeonRoomData::get_level_range() {
	return _level_range;
}
void DungeonRoomData::set_level_range(Vector2 value) {
	_level_range = value;
}

//Min Size
int DungeonRoomData::get_min_sizex() {
	return _min_sizex;
}
void DungeonRoomData::set_min_sizex(int value) {
	_min_sizex = value;
}

int DungeonRoomData::get_min_sizey() {
	return _min_sizey;
}
void DungeonRoomData::set_min_sizey(int value) {
	_min_sizey = value;
}

int DungeonRoomData::get_min_sizez() {
	return _min_sizez;
}
void DungeonRoomData::set_min_sizez(int value) {
	_min_sizez = value;
}

//Max Size
int DungeonRoomData::get_max_sizex() {
	return _max_sizex;
}
void DungeonRoomData::set_max_sizex(int value) {
	_max_sizex = value;
}

int DungeonRoomData::get_max_sizey() {
	return _max_sizey;
}
void DungeonRoomData::set_max_sizey(int value) {
	_max_sizey = value;
}

int DungeonRoomData::get_max_sizez() {
	return _max_sizez;
}
void DungeonRoomData::set_max_sizez(int value) {
	_max_sizez = value;
}

//Props
Ref<WorldGeneratorPropData> DungeonRoomData::get_prop_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _prop_datas.size(), Ref<WorldGeneratorPropData>());

	return _prop_datas.get(index);
}
void DungeonRoomData::set_prop_data(const int index, const Ref<WorldGeneratorPropData> prop_data) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.set(index, prop_data);
}
void DungeonRoomData::add_prop_data(const Ref<WorldGeneratorPropData> prop_data) {
	_prop_datas.push_back(prop_data);
}
void DungeonRoomData::remove_prop_data(const int index) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.remove(index);
}
int DungeonRoomData::get_prop_data_count() const {
	return _prop_datas.size();
}

Vector<Variant> DungeonRoomData::get_prop_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _prop_datas.size(); i++) {
		r.push_back(_prop_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonRoomData::set_prop_datas(const Vector<Variant> &prop_datas) {
	_prop_datas.clear();
	for (int i = 0; i < prop_datas.size(); i++) {
		Ref<WorldGeneratorPropData> prop_data = Ref<WorldGeneratorPropData>(prop_datas[i]);

		_prop_datas.push_back(prop_data);
	}
}

//Environments
Ref<EnvironmentData> DungeonRoomData::get_environment_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _environment_datas.size(), Ref<EnvironmentData>());

	return _environment_datas.get(index);
}
void DungeonRoomData::set_environment_data(const int index, const Ref<EnvironmentData> environment_data) {
	ERR_FAIL_INDEX(index, _environment_datas.size());

	_environment_datas.set(index, environment_data);
}
void DungeonRoomData::add_environment_data(const Ref<EnvironmentData> environment_data) {
	_environment_datas.push_back(environment_data);
}
void DungeonRoomData::remove_environment_data(const int index) {
	ERR_FAIL_INDEX(index, _environment_datas.size());

	_environment_datas.remove(index);
}
int DungeonRoomData::get_environment_data_count() const {
	return _environment_datas.size();
}

Vector<Variant> DungeonRoomData::get_environment_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _environment_datas.size(); i++) {
		r.push_back(_environment_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonRoomData::set_environment_datas(const Vector<Variant> &environment_datas) {
	_environment_datas.clear();
	for (int i = 0; i < environment_datas.size(); i++) {
		Ref<EnvironmentData> environment_data = Ref<EnvironmentData>(environment_datas[i]);

		_environment_datas.push_back(environment_data);
	}
}

//Entities
Ref<EntityData> DungeonRoomData::get_entity_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _entity_datas.size(), Ref<EntityData>());

	return _entity_datas.get(index);
}
void DungeonRoomData::set_entity_data(const int index, const Ref<EntityData> entity_data) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.set(index, entity_data);
}
void DungeonRoomData::add_entity_data(const Ref<EntityData> entity_data) {
	_entity_datas.push_back(entity_data);
}
void DungeonRoomData::remove_entity_data(const int index) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.remove(index);
}
int DungeonRoomData::get_entity_data_count() const {
	return _entity_datas.size();
}

Vector<Variant> DungeonRoomData::get_entity_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _entity_datas.size(); i++) {
		r.push_back(_entity_datas[i].get_ref_ptr());
	}
	return r;
}
void DungeonRoomData::set_entity_datas(const Vector<Variant> &entity_datas) {
	_entity_datas.clear();
	for (int i = 0; i < entity_datas.size(); i++) {
		Ref<EntityData> entity_data = Ref<EntityData>(entity_datas[i]);

		_entity_datas.push_back(entity_data);
	}
}

////    Surfaces    ////
Ref<VoxelSurface> DungeonRoomData::get_voxel_surface(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>());

	return _voxel_surfaces.get(index);
}
void DungeonRoomData::set_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface) {
	ERR_FAIL_INDEX(index, _voxel_surfaces.size());

	_voxel_surfaces.set(index, voxel_surface);
}
void DungeonRoomData::add_voxel_surface(const Ref<VoxelSurface> voxel_surface) {
	_voxel_surfaces.push_back(voxel_surface);
}
void DungeonRoomData::remove_voxel_surface(const int index) {
	ERR_FAIL_INDEX(index, _voxel_surfaces.size());

	_voxel_surfaces.remove(index);
}
int DungeonRoomData::get_voxel_surface_count() const {
	return _voxel_surfaces.size();
}

Vector<Variant> DungeonRoomData::get_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		r.push_back(_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}
void DungeonRoomData::set_voxel_surfaces(const Vector<Variant> &voxel_surfaces) {
	_voxel_surfaces.clear();
	for (int i = 0; i < voxel_surfaces.size(); i++) {
		Ref<EnvironmentData> voxel_surface = Ref<EnvironmentData>(voxel_surfaces[i]);

		_voxel_surfaces.push_back(voxel_surface);
	}
}

////    Liquid Surfaces    ////
Ref<VoxelSurface> DungeonRoomData::get_liquid_voxel_surface(const int index) const {
	ERR_FAIL_INDEX_V(index, _liquid_voxel_surfaces.size(), Ref<VoxelSurface>());

	return _liquid_voxel_surfaces.get(index);
}
void DungeonRoomData::set_liquid_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface) {
	ERR_FAIL_INDEX(index, _liquid_voxel_surfaces.size());

	_liquid_voxel_surfaces.set(index, voxel_surface);
}
void DungeonRoomData::add_liquid_voxel_surface(const Ref<VoxelSurface> voxel_surface) {
	_liquid_voxel_surfaces.push_back(voxel_surface);
}
void DungeonRoomData::remove_liquid_voxel_surface(const int index) {
	ERR_FAIL_INDEX(index, _liquid_voxel_surfaces.size());

	_liquid_voxel_surfaces.remove(index);
}
int DungeonRoomData::get_liquid_voxel_surface_count() const {
	return _liquid_voxel_surfaces.size();
}

Vector<Variant> DungeonRoomData::get_liquid_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _liquid_voxel_surfaces.size(); i++) {
		r.push_back(_liquid_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}
void DungeonRoomData::set_liquid_voxel_surfaces(const Vector<Variant> &voxel_surfaces) {
	_liquid_voxel_surfaces.clear();
	for (int i = 0; i < voxel_surfaces.size(); i++) {
		Ref<EnvironmentData> voxel_surface = Ref<EnvironmentData>(voxel_surfaces[i]);

		_liquid_voxel_surfaces.push_back(voxel_surface);
	}
}

DungeonRoomData::DungeonRoomData() {
	_min_sizex = 0;
	_min_sizey = 0;
	_min_sizez = 0;

	_max_sizex = 0;
	_max_sizey = 0;
	_max_sizez = 0;
}
DungeonRoomData::~DungeonRoomData() {
	_prop_datas.clear();
	_entity_datas.clear();
	_environment_datas.clear();
	_voxel_surfaces.clear();
	_liquid_voxel_surfaces.clear();
}

void DungeonRoomData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_class_name"), &DungeonRoomData::get_target_class_name);
	ClassDB::bind_method(D_METHOD("set_target_class_name", "value"), &DungeonRoomData::set_target_class_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "target_class_name"), "set_target_class_name", "get_target_class_name");

	ClassDB::bind_method(D_METHOD("get_target_script"), &DungeonRoomData::get_target_script);
	ClassDB::bind_method(D_METHOD("set_target_script", "value"), &DungeonRoomData::set_target_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PROPERTY_HINT_RESOURCE_TYPE, "Script"), "set_target_script", "get_target_script");

	ClassDB::bind_method(D_METHOD("get_level_range"), &DungeonRoomData::get_level_range);
	ClassDB::bind_method(D_METHOD("set_level_range", "value"), &DungeonRoomData::set_level_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "level_range"), "set_level_range", "get_level_range");

	//Min Size
	ClassDB::bind_method(D_METHOD("get_min_sizex"), &DungeonRoomData::get_min_sizex);
	ClassDB::bind_method(D_METHOD("set_min_sizex", "value"), &DungeonRoomData::set_min_sizex);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_sizex"), "set_min_sizex", "get_min_sizex");

	ClassDB::bind_method(D_METHOD("get_min_sizey"), &DungeonRoomData::get_min_sizey);
	ClassDB::bind_method(D_METHOD("set_min_sizey", "value"), &DungeonRoomData::set_min_sizey);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_sizey"), "set_min_sizey", "get_min_sizey");

	ClassDB::bind_method(D_METHOD("get_min_sizez"), &DungeonRoomData::get_min_sizez);
	ClassDB::bind_method(D_METHOD("set_min_sizez", "value"), &DungeonRoomData::set_min_sizez);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_sizez"), "set_min_sizez", "get_min_sizez");

	//Max Size
	ClassDB::bind_method(D_METHOD("get_max_sizex"), &DungeonRoomData::get_max_sizex);
	ClassDB::bind_method(D_METHOD("set_max_sizex", "value"), &DungeonRoomData::set_max_sizex);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_sizex"), "set_max_sizex", "get_max_sizex");

	ClassDB::bind_method(D_METHOD("get_max_sizey"), &DungeonRoomData::get_max_sizey);
	ClassDB::bind_method(D_METHOD("set_max_sizey", "value"), &DungeonRoomData::set_max_sizey);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_sizey"), "set_max_sizey", "get_max_sizey");

	ClassDB::bind_method(D_METHOD("get_max_sizez"), &DungeonRoomData::get_max_sizez);
	ClassDB::bind_method(D_METHOD("set_max_sizez", "value"), &DungeonRoomData::set_max_sizez);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_sizez"), "set_max_sizez", "get_max_sizez");

	//Props
	ClassDB::bind_method(D_METHOD("get_prop_data", "index"), &DungeonRoomData::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "index", "data"), &DungeonRoomData::set_prop_data);
	ClassDB::bind_method(D_METHOD("add_prop_data", "prop_data"), &DungeonRoomData::add_prop_data);
	ClassDB::bind_method(D_METHOD("remove_prop_data", "index"), &DungeonRoomData::remove_prop_data);
	ClassDB::bind_method(D_METHOD("get_prop_data_count"), &DungeonRoomData::get_prop_data_count);

	ClassDB::bind_method(D_METHOD("get_prop_datas"), &DungeonRoomData::get_prop_datas);
	ClassDB::bind_method(D_METHOD("set_prop_datas", "prop_datas"), &DungeonRoomData::set_prop_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_datas", PROPERTY_HINT_NONE, "17/17:WorldGeneratorPropData", PROPERTY_USAGE_DEFAULT, "WorldGeneratorPropData"), "set_prop_datas", "get_prop_datas");

	//Entities
	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &DungeonRoomData::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &DungeonRoomData::set_entity_data);
	ClassDB::bind_method(D_METHOD("add_entity_data", "entity_data"), &DungeonRoomData::add_entity_data);
	ClassDB::bind_method(D_METHOD("remove_entity_data", "index"), &DungeonRoomData::remove_entity_data);
	ClassDB::bind_method(D_METHOD("get_entity_data_count"), &DungeonRoomData::get_entity_data_count);

	ClassDB::bind_method(D_METHOD("get_entity_datas"), &DungeonRoomData::get_entity_datas);
	ClassDB::bind_method(D_METHOD("set_entity_datas", "entity_datas"), &DungeonRoomData::set_entity_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "entity_datas", PROPERTY_HINT_NONE, "17/17:EntityData", PROPERTY_USAGE_DEFAULT, "EntityData"), "set_entity_datas", "get_entity_datas");

	//Environments
	ClassDB::bind_method(D_METHOD("get_environment_data", "index"), &DungeonRoomData::get_environment_data);
	ClassDB::bind_method(D_METHOD("set_environment_data", "index", "data"), &DungeonRoomData::set_environment_data);
	ClassDB::bind_method(D_METHOD("add_environment_data", "environment_data"), &DungeonRoomData::add_environment_data);
	ClassDB::bind_method(D_METHOD("remove_environment_data", "index"), &DungeonRoomData::remove_environment_data);
	ClassDB::bind_method(D_METHOD("get_environment_data_count"), &DungeonRoomData::get_environment_data_count);

	ClassDB::bind_method(D_METHOD("get_environment_datas"), &DungeonRoomData::get_environment_datas);
	ClassDB::bind_method(D_METHOD("set_environment_datas", "environment_datas"), &DungeonRoomData::set_environment_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "environment_datas", PROPERTY_HINT_NONE, "17/17:EnvironmentData", PROPERTY_USAGE_DEFAULT, "EnvironmentData"), "set_environment_datas", "get_environment_datas");

	//Surfaces
	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &DungeonRoomData::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "data"), &DungeonRoomData::set_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_voxel_surface", "voxel_surface"), &DungeonRoomData::add_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_voxel_surface", "index"), &DungeonRoomData::remove_voxel_surface);
	ClassDB::bind_method(D_METHOD("get_voxel_surface_count"), &DungeonRoomData::get_voxel_surface_count);

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &DungeonRoomData::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces", "voxel_surfaces"), &DungeonRoomData::set_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_voxel_surfaces", "get_voxel_surfaces");

	//Liquid Surfaces
	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface", "index"), &DungeonRoomData::get_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surface", "index", "data"), &DungeonRoomData::set_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_liquid_voxel_surface", "voxel_surface"), &DungeonRoomData::add_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_liquid_voxel_surface", "index"), &DungeonRoomData::remove_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface_count"), &DungeonRoomData::get_liquid_voxel_surface_count);

	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surfaces"), &DungeonRoomData::get_liquid_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surfaces", "voxel_surfaces"), &DungeonRoomData::set_liquid_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_liquid_voxel_surfaces", "get_liquid_voxel_surfaces");
}
