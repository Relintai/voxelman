#include "biome.h"

Vector2 Biome::get_level_range() {
	return _level_range;
}
void Biome::set_level_range(Vector2 value) {
	_level_range = value;
}

Ref<EnvironmentData> Biome::get_environment() {
	return _environment;
}
void Biome::set_environment(Ref<EnvironmentData> value) {
	_environment = value;
}

////    Prop Data    ////
Ref<WorldGeneratorPropData> Biome::get_prop_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _prop_datas.size(), Ref<WorldGeneratorPropData>());

	return _prop_datas.get(index);
}
void Biome::set_prop_data(const int index, const Ref<WorldGeneratorPropData> prop_data) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.set(index, prop_data);
}
void Biome::add_prop_data(const Ref<WorldGeneratorPropData> prop_data) {
	_prop_datas.push_back(prop_data);
}
void Biome::remove_prop_data(const int index) {
	ERR_FAIL_INDEX(index, _prop_datas.size());

	_prop_datas.remove(index);
}

int Biome::get_prop_data_count() const {
	return _prop_datas.size();
}

//Entities
Ref<EntityData> Biome::get_entity_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _entity_datas.size(), Ref<EntityData>());

	return _entity_datas.get(index);
}
void Biome::set_entity_data(const int index, const Ref<EntityData> entity_data) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.set(index, entity_data);
}
void Biome::add_entity_data(const Ref<EntityData> entity_data) {
	_entity_datas.push_back(entity_data);
}
void Biome::remove_entity_data(const int index) {
	ERR_FAIL_INDEX(index, _entity_datas.size());

	_entity_datas.remove(index);
}

int Biome::get_entity_data_count() const {
	return _entity_datas.size();
}

////    Dungeons    ////
Ref<Dungeon> Biome::get_dungeon(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeons.size(), Ref<Dungeon>());

	return _dungeons.get(index);
}
void Biome::set_dungeon(const int index, const Ref<Dungeon> dungeon) {
	ERR_FAIL_INDEX(index, _dungeons.size());

	_dungeons.set(index, dungeon);
}
void Biome::add_dungeon(const Ref<Dungeon> dungeon) {
	_dungeons.push_back(dungeon);
}
void Biome::remove_dungeon(const int index) {
	ERR_FAIL_INDEX(index, _dungeons.size());

	_dungeons.remove(index);
}

int Biome::get_dungeon_count() const {
	return _dungeons.size();
}


void Biome::generate_chunk(VoxelChunk *chunk, bool spawn_mobs) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk, spawn_mobs);
	}
}
void Biome::generate_chunk_bind(Node *chunk, bool spawn_mobs) {
	generate_chunk(Object::cast_to<VoxelChunk>(chunk), spawn_mobs);
}
void Biome::generate_stack(VoxelChunk *chunk, int x, int z, bool spawn_mobs) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_generate_stack")) {
		call("_generate_stack", chunk, x, z, spawn_mobs);
	}
}
void Biome::generate_stack_bind(Node *chunk, int x, int z, bool spawn_mobs) {
	generate_stack(Object::cast_to<VoxelChunk>(chunk), x, z, spawn_mobs);
}

Biome::Biome() {

}
Biome::~Biome() {
	_environment.unref();
	_prop_datas.clear();
	_entity_datas.clear();
	_dungeons.clear();
}

void Biome::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), PropertyInfo(Variant::BOOL, "spawn_mobs")));
	BIND_VMETHOD(MethodInfo("_generate_stack", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "z"), PropertyInfo(Variant::BOOL, "spawn_mobs")));

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk", "spawn_mobs"), &Biome::generate_chunk_bind);
	ClassDB::bind_method(D_METHOD("generate_stack", "chunk", "x", "z", "spawn_mobs"), &Biome::generate_stack_bind);

	ClassDB::bind_method(D_METHOD("get_level_range"), &Biome::get_level_range);
	ClassDB::bind_method(D_METHOD("set_level_range", "value"), &Biome::set_level_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "level_range"), "set_level_range", "get_level_range");

	ClassDB::bind_method(D_METHOD("get_environment"), &Biome::get_environment);
	ClassDB::bind_method(D_METHOD("set_environment", "value"), &Biome::set_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "EnvironmentData"), "set_environment", "get_environment");

	//Props
	ClassDB::bind_method(D_METHOD("get_prop_data", "index"), &Biome::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "index", "data"), &Biome::set_prop_data);
	ClassDB::bind_method(D_METHOD("add_prop_data", "prop_data"), &Biome::add_prop_data);
	ClassDB::bind_method(D_METHOD("remove_prop_data", "index"), &Biome::remove_prop_data);

	ClassDB::bind_method(D_METHOD("get_prop_data_count"), &Biome::get_prop_data_count);

	//Entities
	ClassDB::bind_method(D_METHOD("get_entity_data", "index"), &Biome::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "index", "data"), &Biome::set_entity_data);
	ClassDB::bind_method(D_METHOD("add_entity_data", "entity_data"), &Biome::add_entity_data);
	ClassDB::bind_method(D_METHOD("remove_entity_data", "index"), &Biome::remove_entity_data);

	ClassDB::bind_method(D_METHOD("get_entity_data_count"), &Biome::get_entity_data_count);

	//Dungeons
	ClassDB::bind_method(D_METHOD("get_dungeon", "index"), &Biome::get_dungeon);
	ClassDB::bind_method(D_METHOD("set_dungeon", "index", "data"), &Biome::set_dungeon);
	ClassDB::bind_method(D_METHOD("add_dungeon", "dungeon"), &Biome::add_dungeon);
	ClassDB::bind_method(D_METHOD("remove_dungeon", "index"), &Biome::remove_dungeon);

	ClassDB::bind_method(D_METHOD("get_dungeon_count"), &Biome::get_dungeon_count);
}
