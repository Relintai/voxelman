#include "biome_data.h"

String BiomeData::get_target_class_name() {
	return _target_class_name;
}
void BiomeData::set_target_class_name(String name) {
	_target_class_name = name;
}

Ref<Script> BiomeData::get_target_script() {
	return _target_script;
}
void BiomeData::set_target_script(Ref<Script> script) {
	_target_script = script;
}

Vector2 BiomeData::get_level_range() {
	return _level_range;
}
void BiomeData::set_level_range(Vector2 value) {
	_level_range = value;
}

Vector2 BiomeData::get_humidity_range() {
	return _humidity_range;
}
void BiomeData::set_humidity_range(Vector2 range) {
	_humidity_range = range;
}

Vector2 BiomeData::get_temperature_range() {
	return _temperature_range;
}
void BiomeData::set_temperature_range(Vector2 range) {
	_temperature_range = range;
}

////    DungeonData    ////

Ref<DungeonData> BiomeData::get_dungeon_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeon_datas.size(), Ref<DungeonData>());

	return _dungeon_datas.get(index);
}
void BiomeData::set_dungeon_data(const int index, const Ref<DungeonData> dungeon_data) {
	ERR_FAIL_INDEX(index, _dungeon_datas.size());

	_dungeon_datas.set(index, dungeon_data);
}
void BiomeData::add_dungeon_data(const Ref<DungeonData> dungeon_data) {
	_dungeon_datas.push_back(dungeon_data);
}
void BiomeData::remove_dungeon_data(const int index) {
	ERR_FAIL_INDEX(index, _dungeon_datas.size());

	_dungeon_datas.remove(index);
}

int BiomeData::get_dungeon_data_count() const {
	return _dungeon_datas.size();
}

Vector<Variant> BiomeData::get_dungeon_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _dungeon_datas.size(); i++) {
		r.push_back(_dungeon_datas[i].get_ref_ptr());
	}
	return r;
}
void BiomeData::set_dungeon_datas(const Vector<Variant> &dungeon_datas) {
	_dungeon_datas.clear();
	for (int i = 0; i < dungeon_datas.size(); i++) {
		Ref<DungeonData> dungeon_data = Ref<DungeonData>(dungeon_datas[i]);

		_dungeon_datas.push_back(dungeon_data);
	}
}

////    PROP DATA    ////

Ref<WorldGeneratorPropData> BiomeData::get_prop_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _prop_datas.size(), Ref<WorldGeneratorPropData>());

	return _prop_datas.get(index);
}
void BiomeData::set_prop_data(const int index, const Ref<WorldGeneratorPropData> prop_data) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.set(index, prop_data);
}
void BiomeData::add_prop_data(const Ref<WorldGeneratorPropData> prop_data) {
	_prop_datas.push_back(prop_data);
}
void BiomeData::remove_prop_data(const int index) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.remove(index);
}

int BiomeData::get_prop_data_count() const {
	return _prop_datas.size();
}

Vector<Variant> BiomeData::get_prop_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _prop_datas.size(); i++) {
		r.push_back(_prop_datas[i].get_ref_ptr());
	}
	return r;
}
void BiomeData::set_prop_datas(const Vector<Variant> &prop_datas) {
	_prop_datas.clear();
	for (int i = 0; i < prop_datas.size(); i++) {
		Ref<WorldGeneratorPropData> prop_data = Ref<WorldGeneratorPropData>(prop_datas[i]);

		_prop_datas.push_back(prop_data);
	}
}

//Entities

Ref<EntityData> BiomeData::get_entity_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _entity_datas.size(), Ref<EntityData>());

	return _entity_datas.get(index);
}
void BiomeData::set_entity_data(const int index, const Ref<EntityData> entity_data) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.set(index, entity_data);
}
void BiomeData::add_entity_data(const Ref<EntityData> entity_data) {
	_entity_datas.push_back(entity_data);
}
void BiomeData::remove_entity_data(const int index) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.remove(index);
}

int BiomeData::get_entity_data_count() const {
	return _entity_datas.size();
}

Vector<Variant> BiomeData::get_entity_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _entity_datas.size(); i++) {
		r.push_back(_entity_datas[i].get_ref_ptr());
	}
	return r;
}
void BiomeData::set_entity_datas(const Vector<Variant> &entity_datas) {
	_entity_datas.clear();
	for (int i = 0; i < entity_datas.size(); i++) {
		Ref<EntityData> entity_data = Ref<EntityData>(entity_datas[i]);

		_entity_datas.push_back(entity_data);
	}
}

//Environments
Ref<EnvironmentData> BiomeData::get_environment_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _environment_datas.size(), Ref<EnvironmentData>());

	return _environment_datas.get(index);
}
void BiomeData::set_environment_data(const int index, const Ref<EnvironmentData> environment_data) {
	ERR_FAIL_INDEX(index, _environment_datas.size());

	_environment_datas.set(index, environment_data);
}
void BiomeData::add_environment_data(const Ref<EnvironmentData> environment_data) {
	_environment_datas.push_back(environment_data);
}
void BiomeData::remove_environment_data(const int index) {
	ERR_FAIL_INDEX(index, _environment_datas.size());

	_environment_datas.remove(index);
}
int BiomeData::get_environment_data_count() const {
	return _environment_datas.size();
}

Vector<Variant> BiomeData::get_environment_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _environment_datas.size(); i++) {
		r.push_back(_environment_datas[i].get_ref_ptr());
	}
	return r;
}
void BiomeData::set_environment_datas(const Vector<Variant> &environment_datas) {
	_environment_datas.clear();
	for (int i = 0; i < environment_datas.size(); i++) {
		Ref<EnvironmentData> environment_data = Ref<EnvironmentData>(environment_datas[i]);

		_environment_datas.push_back(environment_data);
	}
}

////    Surfaces    ////
Ref<VoxelSurface> BiomeData::get_voxel_surface(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>());

	return _voxel_surfaces.get(index);
}
void BiomeData::set_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface) {
	ERR_FAIL_INDEX(index, _voxel_surfaces.size());

	_voxel_surfaces.set(index, voxel_surface);
}
void BiomeData::add_voxel_surface(const Ref<VoxelSurface> voxel_surface) {
	_voxel_surfaces.push_back(voxel_surface);
}
void BiomeData::remove_voxel_surface(const int index) {
	ERR_FAIL_INDEX(index, _voxel_surfaces.size());

	_voxel_surfaces.remove(index);
}
int BiomeData::get_voxel_surface_count() const {
	return _voxel_surfaces.size();
}

Vector<Variant> BiomeData::get_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		r.push_back(_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}
void BiomeData::set_voxel_surfaces(const Vector<Variant> &voxel_surfaces) {
	_voxel_surfaces.clear();
	for (int i = 0; i < voxel_surfaces.size(); i++) {
		Ref<EnvironmentData> voxel_surface = Ref<EnvironmentData>(voxel_surfaces[i]);

		_voxel_surfaces.push_back(voxel_surface);
	}
}

////    Liquid Surfaces    ////
Ref<VoxelSurface> BiomeData::get_liquid_voxel_surface(const int index) const {
	ERR_FAIL_INDEX_V(index, _liquid_voxel_surfaces.size(), Ref<VoxelSurface>());

	return _liquid_voxel_surfaces.get(index);
}
void BiomeData::set_liquid_voxel_surface(const int index, const Ref<VoxelSurface> voxel_surface) {
	ERR_FAIL_INDEX(index, _liquid_voxel_surfaces.size());

	_liquid_voxel_surfaces.set(index, voxel_surface);
}
void BiomeData::add_liquid_voxel_surface(const Ref<VoxelSurface> voxel_surface) {
	_liquid_voxel_surfaces.push_back(voxel_surface);
}
void BiomeData::remove_liquid_voxel_surface(const int index) {
	ERR_FAIL_INDEX(index, _liquid_voxel_surfaces.size());

	_liquid_voxel_surfaces.remove(index);
}
int BiomeData::get_liquid_voxel_surface_count() const {
	return _liquid_voxel_surfaces.size();
}

Vector<Variant> BiomeData::get_liquid_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _liquid_voxel_surfaces.size(); i++) {
		r.push_back(_liquid_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}
void BiomeData::set_liquid_voxel_surfaces(const Vector<Variant> &voxel_surfaces) {
	_liquid_voxel_surfaces.clear();
	for (int i = 0; i < voxel_surfaces.size(); i++) {
		Ref<EnvironmentData> voxel_surface = Ref<EnvironmentData>(voxel_surfaces[i]);

		_liquid_voxel_surfaces.push_back(voxel_surface);
	}
}

BiomeData::BiomeData() {

}
BiomeData::~BiomeData() {
	_dungeon_datas.clear();
	_prop_datas.clear();
	_entity_datas.clear();

	_environment_datas.clear();
	_voxel_surfaces.clear();
	_liquid_voxel_surfaces.clear();
}

void BiomeData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_class_name"), &BiomeData::get_target_class_name);
	ClassDB::bind_method(D_METHOD("set_target_class_name", "value"), &BiomeData::set_target_class_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "target_class_name"), "set_target_class_name", "get_target_class_name");

	ClassDB::bind_method(D_METHOD("get_target_script"), &BiomeData::get_target_script);
	ClassDB::bind_method(D_METHOD("set_target_script", "value"), &BiomeData::set_target_script);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_script", PROPERTY_HINT_RESOURCE_TYPE, "Script"), "set_target_script", "get_target_script");

	ClassDB::bind_method(D_METHOD("get_level_range"), &BiomeData::get_level_range);
	ClassDB::bind_method(D_METHOD("set_level_range", "value"), &BiomeData::set_level_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "level_range"), "set_level_range", "get_level_range");

	ClassDB::bind_method(D_METHOD("get_humidity_range"), &BiomeData::get_humidity_range);
	ClassDB::bind_method(D_METHOD("set_humidity_range", "value"), &BiomeData::set_humidity_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "humidity_range"), "set_humidity_range", "get_humidity_range");

	ClassDB::bind_method(D_METHOD("get_temperature_range"), &BiomeData::get_temperature_range);
	ClassDB::bind_method(D_METHOD("set_temperature_range", "value"), &BiomeData::set_temperature_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "temperature_range"), "set_temperature_range", "get_temperature_range");

	//DungeonDatas
	ClassDB::bind_method(D_METHOD("get_dungeon_data", "index"), &BiomeData::get_dungeon_data);
	ClassDB::bind_method(D_METHOD("set_dungeon_data", "index", "data"), &BiomeData::set_dungeon_data);
	ClassDB::bind_method(D_METHOD("add_dungeon_data", "dungeon_data"), &BiomeData::add_dungeon_data);
	ClassDB::bind_method(D_METHOD("remove_dungeon_data", "index"), &BiomeData::remove_dungeon_data);
	ClassDB::bind_method(D_METHOD("get_dungeon_data_count"), &BiomeData::get_dungeon_data_count);

	ClassDB::bind_method(D_METHOD("get_dungeon_datas"), &BiomeData::get_dungeon_datas);
	ClassDB::bind_method(D_METHOD("set_dungeon_datas", "dungeon_datas"), &BiomeData::set_dungeon_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dungeon_datas", PROPERTY_HINT_NONE, "17/17:DungeonData", PROPERTY_USAGE_DEFAULT, "DungeonData"), "set_dungeon_datas", "get_dungeon_datas");

	//WorldGeneratorPropData
	ClassDB::bind_method(D_METHOD("get_prop_data", "index"), &BiomeData::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "index", "data"), &BiomeData::set_prop_data);
	ClassDB::bind_method(D_METHOD("add_prop_data", "prop_data"), &BiomeData::add_prop_data);
	ClassDB::bind_method(D_METHOD("remove_prop_data", "index"), &BiomeData::remove_prop_data);
	ClassDB::bind_method(D_METHOD("get_prop_data_count"), &BiomeData::get_prop_data_count);

	ClassDB::bind_method(D_METHOD("get_prop_datas"), &BiomeData::get_prop_datas);
	ClassDB::bind_method(D_METHOD("set_prop_datas", "prop_datas"), &BiomeData::set_prop_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_datas", PROPERTY_HINT_NONE, "17/17:WorldGeneratorPropData", PROPERTY_USAGE_DEFAULT, "WorldGeneratorPropData"), "set_prop_datas", "get_prop_datas");

	//Entities
	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &BiomeData::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &BiomeData::set_entity_data);
	ClassDB::bind_method(D_METHOD("add_entity_data", "entity_data"), &BiomeData::add_entity_data);
	ClassDB::bind_method(D_METHOD("remove_entity_data", "index"), &BiomeData::remove_entity_data);
	ClassDB::bind_method(D_METHOD("get_entity_data_count"), &BiomeData::get_entity_data_count);

	ClassDB::bind_method(D_METHOD("get_entity_datas"), &BiomeData::get_entity_datas);
	ClassDB::bind_method(D_METHOD("set_entity_datas", "entity_datas"), &BiomeData::set_entity_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "entity_datas", PROPERTY_HINT_NONE, "17/17:EntityData", PROPERTY_USAGE_DEFAULT, "EntityData"), "set_entity_datas", "get_entity_datas");

	//Environments
	ClassDB::bind_method(D_METHOD("get_environment_data", "index"), &BiomeData::get_environment_data);
	ClassDB::bind_method(D_METHOD("set_environment_data", "index", "data"), &BiomeData::set_environment_data);
	ClassDB::bind_method(D_METHOD("add_environment_data", "environment_data"), &BiomeData::add_environment_data);
	ClassDB::bind_method(D_METHOD("remove_environment_data", "index"), &BiomeData::remove_environment_data);
	ClassDB::bind_method(D_METHOD("get_environment_data_count"), &BiomeData::get_environment_data_count);

	ClassDB::bind_method(D_METHOD("get_environment_datas"), &BiomeData::get_environment_datas);
	ClassDB::bind_method(D_METHOD("set_environment_datas", "environment_datas"), &BiomeData::set_environment_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "environment_datas", PROPERTY_HINT_NONE, "17/17:EnvironmentData", PROPERTY_USAGE_DEFAULT, "EnvironmentData"), "set_environment_datas", "get_environment_datas");

	//Surfaces
	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &BiomeData::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "data"), &BiomeData::set_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_voxel_surface", "voxel_surface"), &BiomeData::add_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_voxel_surface", "index"), &BiomeData::remove_voxel_surface);
	ClassDB::bind_method(D_METHOD("get_voxel_surface_count"), &BiomeData::get_voxel_surface_count);

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &BiomeData::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces", "voxel_surfaces"), &BiomeData::set_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_voxel_surfaces", "get_voxel_surfaces");

	//Liquid Surfaces
	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface", "index"), &BiomeData::get_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surface", "index", "data"), &BiomeData::set_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_liquid_voxel_surface", "voxel_surface"), &BiomeData::add_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_liquid_voxel_surface", "index"), &BiomeData::remove_liquid_voxel_surface);
	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surface_count"), &BiomeData::get_liquid_voxel_surface_count);

	ClassDB::bind_method(D_METHOD("get_liquid_voxel_surfaces"), &BiomeData::get_liquid_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_liquid_voxel_surfaces", "voxel_surfaces"), &BiomeData::set_liquid_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_liquid_voxel_surfaces", "get_liquid_voxel_surfaces");
}
