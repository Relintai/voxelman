#include "dungeon_room.h"

int DungeonRoom::get_seed() {
	return _seed;
}
void DungeonRoom::set_seed(int value) {
	_seed = value;
}

Vector2 DungeonRoom::get_level_range() {
	return _level_range;
}
void DungeonRoom::set_level_range(Vector2 value) {
	_level_range = value;
}

//Position
int DungeonRoom::get_posx() {
	return _posx;
}
void DungeonRoom::set_posx(int value) {
	_posx = value;
}

int DungeonRoom::get_posy() {
	return _posy;
}
void DungeonRoom::set_posy(int value) {
	_posy = value;
}

int DungeonRoom::get_posz() {
	return _posz;
}
void DungeonRoom::set_posz(int value) {
	_posz = value;
}

//Size
int DungeonRoom::get_sizex() {
	return _sizex;
}
void DungeonRoom::set_sizex(int value) {
	_sizex = value;
}

int DungeonRoom::get_sizey() {
	return _sizey;
}
void DungeonRoom::set_sizey(int value) {
	_sizey = value;
}

int DungeonRoom::get_sizez() {
	return _sizez;
}
void DungeonRoom::set_sizez(int value) {
	_sizez = value;
}

Ref<EnvironmentData> DungeonRoom::get_environment() {
	return _environment;
}
void DungeonRoom::set_environment(Ref<EnvironmentData> value) {
	_environment = value;
}

Ref<VoxelStructure> DungeonRoom::get_structure() {
	return _structure;
}
void DungeonRoom::set_structure(Ref<VoxelStructure> structure) {
	_structure = structure;
}

//Props
Ref<WorldGeneratorPropData> DungeonRoom::get_prop_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _prop_datas.size(), Ref<WorldGeneratorPropData>());

	return _prop_datas.get(index);
}
void DungeonRoom::set_prop_data(const int index, const Ref<WorldGeneratorPropData> prop_data) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.set(index, prop_data);
}
void DungeonRoom::add_prop_data(const Ref<WorldGeneratorPropData> prop_data) {
	_prop_datas.push_back(prop_data);
}
void DungeonRoom::remove_prop_data(const int index) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.remove(index);
}

int DungeonRoom::get_prop_data_count() const {
	return _prop_datas.size();
}

//Entities
Ref<EntityData> DungeonRoom::get_entity_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _entity_datas.size(), Ref<EntityData>());

	return _entity_datas.get(index);
}
void DungeonRoom::set_entity_data(const int index, const Ref<EntityData> entity_data) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.set(index, entity_data);
}
void DungeonRoom::add_entity_data(const Ref<EntityData> entity_data) {
	_entity_datas.push_back(entity_data);
}
void DungeonRoom::remove_entity_data(const int index) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.remove(index);
}

int DungeonRoom::get_entity_data_count() const {
	return _entity_datas.size();
}


void DungeonRoom::setup() {
	if (has_method("_setup")) {
		call("_setup");
	}
}

void DungeonRoom::generate_chunk(VoxelChunk *chunk, bool spawn_mobs) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk, spawn_mobs);
	}
}

void DungeonRoom::generate_chunk_bind(Node *chunk, bool spawn_mobs) {
	generate_chunk(Object::cast_to<VoxelChunk>(chunk), spawn_mobs);
}

void DungeonRoom::generate_room(Ref<VoxelStructure> structure, bool spawn_mobs) {
	if (has_method("_generate_room")) {
		call("_generate_room", structure, spawn_mobs);
	}
}

DungeonRoom::DungeonRoom() {
	_seed = 0;

	_posx = 0;
	_posy = 0;
	_posz = 0;

	_sizex = 0;
	_sizey = 0;
	_sizez = 0;
}
DungeonRoom::~DungeonRoom() {
	_environment.unref();
	_prop_datas.clear();
	_entity_datas.clear();
}

void DungeonRoom::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_setup"));
	BIND_VMETHOD(MethodInfo("_generate_room", PropertyInfo(Variant::OBJECT, "structure", PROPERTY_HINT_RESOURCE_TYPE, "VoxelStructure"), PropertyInfo(Variant::BOOL, "spawn_mobs")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), PropertyInfo(Variant::BOOL, "spawn_mobs")));

	ClassDB::bind_method(D_METHOD("setup"), &DungeonRoom::setup);
	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk", "spawn_mobs"), &DungeonRoom::generate_chunk_bind);
	ClassDB::bind_method(D_METHOD("generate_room", "structure", "spawn_mobs"), &DungeonRoom::generate_room);

	ClassDB::bind_method(D_METHOD("get_seed"), &DungeonRoom::get_seed);
	ClassDB::bind_method(D_METHOD("set_seed", "value"), &DungeonRoom::set_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");

	ClassDB::bind_method(D_METHOD("get_level_range"), &DungeonRoom::get_level_range);
	ClassDB::bind_method(D_METHOD("set_level_range", "value"), &DungeonRoom::set_level_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "level_range"), "set_level_range", "get_level_range");

	//Position
	ClassDB::bind_method(D_METHOD("get_posx"), &DungeonRoom::get_posx);
	ClassDB::bind_method(D_METHOD("set_posx", "value"), &DungeonRoom::set_posx);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "posx"), "set_posx", "get_posx");

	ClassDB::bind_method(D_METHOD("get_posy"), &DungeonRoom::get_posy);
	ClassDB::bind_method(D_METHOD("set_posy", "value"), &DungeonRoom::set_posy);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "posy"), "set_posy", "get_posy");

	ClassDB::bind_method(D_METHOD("get_posz"), &DungeonRoom::get_posz);
	ClassDB::bind_method(D_METHOD("set_posz", "value"), &DungeonRoom::set_posz);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "posz"), "set_posz", "get_posz");

	//Size
	ClassDB::bind_method(D_METHOD("get_sizex"), &DungeonRoom::get_sizex);
	ClassDB::bind_method(D_METHOD("set_sizex", "value"), &DungeonRoom::set_sizex);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sizex"), "set_sizex", "get_sizex");

	ClassDB::bind_method(D_METHOD("get_sizey"), &DungeonRoom::get_sizey);
	ClassDB::bind_method(D_METHOD("set_sizey", "value"), &DungeonRoom::set_sizey);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sizey"), "set_sizey", "get_sizey");

	ClassDB::bind_method(D_METHOD("get_sizez"), &DungeonRoom::get_sizez);
	ClassDB::bind_method(D_METHOD("set_sizez", "value"), &DungeonRoom::set_sizez);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sizez"), "set_sizez", "get_sizez");

	ClassDB::bind_method(D_METHOD("get_environment"), &DungeonRoom::get_environment);
	ClassDB::bind_method(D_METHOD("set_environment", "value"), &DungeonRoom::set_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "EnvironmentData"), "set_environment", "get_environment");

	ClassDB::bind_method(D_METHOD("get_structure"), &DungeonRoom::get_structure);
	ClassDB::bind_method(D_METHOD("set_structure", "value"), &DungeonRoom::set_structure);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "structure", PROPERTY_HINT_RESOURCE_TYPE, "VoxelStructure"), "set_structure", "get_structure");

	//Props
	ClassDB::bind_method(D_METHOD("get_prop_data", "index"), &DungeonRoom::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "index", "data"), &DungeonRoom::set_prop_data);
	ClassDB::bind_method(D_METHOD("add_prop_data", "prop_data"), &DungeonRoom::add_prop_data);
	ClassDB::bind_method(D_METHOD("remove_prop_data", "index"), &DungeonRoom::remove_prop_data);

	ClassDB::bind_method(D_METHOD("get_prop_data_count"), &DungeonRoom::get_prop_data_count);

	//Entities
	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &DungeonRoom::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &DungeonRoom::set_entity_data);
	ClassDB::bind_method(D_METHOD("add_entity_data", "entity_data"), &DungeonRoom::add_entity_data);
	ClassDB::bind_method(D_METHOD("remove_entity_data", "index"), &DungeonRoom::remove_entity_data);

	ClassDB::bind_method(D_METHOD("get_entity_data_count"), &DungeonRoom::get_entity_data_count);
}
