#include "planet.h"

int Planet::get_seed() {
	return _seed;
}
void Planet::set_seed(int value) {
	_seed = value;
}

Vector2 Planet::get_level_range() {
	return _level_range;
}
void Planet::set_level_range(Vector2 value) {
	_level_range = value;
}

Ref<EnvironmentData> Planet::get_environment() {
	return _environment;
}
void Planet::set_environment(Ref<EnvironmentData> value) {
	_environment = value;
}

Ref<PlanetData> Planet::get_data() {
	return _data;
}
void Planet::set_data(Ref<PlanetData> value) {
	_data = value;

	setup();
}

Ref<Biome> Planet::get_biome(const int index) const {
	ERR_FAIL_INDEX_V(index, _biomes.size(), Ref<Biome>());

	return _biomes.get(index);
}
void Planet::set_biome(const int index, const Ref<Biome> biome) {
	ERR_FAIL_INDEX(index, _biomes.size());

	_biomes.set(index, biome);
}
void Planet::add_biome(const Ref<Biome> biome) {
	_biomes.push_back(biome);
}
void Planet::remove_biome(const int index) {
	ERR_FAIL_INDEX(index, _biomes.size());

	_biomes.remove(index);
}

int Planet::get_biome_count() const {
	return _biomes.size();
}

////    Dungeons    ////
Ref<Dungeon> Planet::get_dungeon(const int index) const {
	ERR_FAIL_INDEX_V(index, _dungeons.size(), Ref<Dungeon>());

	return _dungeons.get(index);
}
void Planet::set_dungeon(const int index, const Ref<Dungeon> dungeon) {
	ERR_FAIL_INDEX(index, _dungeons.size());

	_dungeons.set(index, dungeon);
}
void Planet::add_dungeon(const Ref<Dungeon> dungeon) {
	_dungeons.push_back(dungeon);
}
void Planet::remove_dungeon(const int index) {
	ERR_FAIL_INDEX(index, _dungeons.size());

	_dungeons.remove(index);
}

int Planet::get_dungeon_count() const {
	return _dungeons.size();
}

void Planet::setup() {
	if (has_method("_setup")) {
		call("_setup");
	}
}

void Planet::generate_chunk(VoxelChunk *chunk, bool spawn_mobs) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk, spawn_mobs);
	}
}

void Planet::generate_chunk_bind(Node *chunk, bool spawn_mobs) {
	generate_chunk(Object::cast_to<VoxelChunk>(chunk), spawn_mobs);
}

Ref<Image> Planet::generate_map() {
	ERR_FAIL_COND_V(!has_method("_generate_map"), Ref<Image>());

	return call("_generate_map");
}

Planet::Planet() {
	_seed = 0;
}
Planet::~Planet() {
	_environment.unref();
	_biomes.clear();
}

void Planet::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_setup"));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), PropertyInfo(Variant::BOOL, "spawn_mobs")));

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk"), &Planet::generate_chunk_bind);
	ClassDB::bind_method(D_METHOD("setup"), &Planet::setup);

	ClassDB::bind_method(D_METHOD("get_seed"), &Planet::get_seed);
	ClassDB::bind_method(D_METHOD("set_seed", "value"), &Planet::set_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");

	ClassDB::bind_method(D_METHOD("get_level_range"), &Planet::get_level_range);
	ClassDB::bind_method(D_METHOD("set_level_range", "value"), &Planet::set_level_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "level_range"), "set_level_range", "get_level_range");

	ClassDB::bind_method(D_METHOD("get_environment"), &Planet::get_environment);
	ClassDB::bind_method(D_METHOD("set_environment", "value"), &Planet::set_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "EnvironmentData"), "set_environment", "get_environment");

	ClassDB::bind_method(D_METHOD("get_data"), &Planet::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &Planet::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "PlanetData"), "set_data", "get_data");

	//biomes
	ClassDB::bind_method(D_METHOD("get_biome", "index"), &Planet::get_biome);
	ClassDB::bind_method(D_METHOD("set_biome", "index", "data"), &Planet::set_biome);
	ClassDB::bind_method(D_METHOD("add_biome", "biome"), &Planet::add_biome);
	ClassDB::bind_method(D_METHOD("remove_biome", "index"), &Planet::remove_biome);

	ClassDB::bind_method(D_METHOD("get_biome_count"), &Planet::get_biome_count);

	//Dungeons
	ClassDB::bind_method(D_METHOD("get_dungeon", "index"), &Planet::get_dungeon);
	ClassDB::bind_method(D_METHOD("set_dungeon", "index", "data"), &Planet::set_dungeon);
	ClassDB::bind_method(D_METHOD("add_dungeon", "dungeon"), &Planet::add_dungeon);
	ClassDB::bind_method(D_METHOD("remove_dungeon", "index"), &Planet::remove_dungeon);

	ClassDB::bind_method(D_METHOD("get_dungeon_count"), &Planet::get_dungeon_count);

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "_generate_map"));

	ClassDB::bind_method(D_METHOD("generate_map"), &Planet::generate_map);
}
