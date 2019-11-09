#include "world_generator.h"

Ref<PlanetData> WorldGenerator::get_planet_data(const int index) const {
	ERR_FAIL_INDEX_V(index, _planet_datas.size(), Ref<PlanetData>());

	return _planet_datas.get(index);
}
void WorldGenerator::set_planet_data(const int index, const Ref<PlanetData> planet_data) {
	ERR_FAIL_INDEX(index, _planet_datas.size());

	_planet_datas.set(index, planet_data);
}
void WorldGenerator::add_planet_data(const Ref<PlanetData> planet_data) {
	_planet_datas.push_back(planet_data);
}
void WorldGenerator::remove_planet_data(const int index) {
	ERR_FAIL_INDEX(index, _planet_datas.size());

	_planet_datas.remove(index);
}

int WorldGenerator::get_planet_data_count() const {
	return _planet_datas.size();
}

Vector<Variant> WorldGenerator::get_planet_datas() {
	Vector<Variant> r;
	for (int i = 0; i < _planet_datas.size(); i++) {
		r.push_back(_planet_datas[i].get_ref_ptr());
	}
	return r;
}
void WorldGenerator::set_planet_datas(const Vector<Variant> &planet_datas) {
	_planet_datas.clear();
	for (int i = 0; i < planet_datas.size(); i++) {
		Ref<PlanetData> planet_data = Ref<PlanetData>(planet_datas[i]);

		_planet_datas.push_back(planet_data);
	}
}

Ref<Planet> WorldGenerator::setup_planet(int seed) {
	if (has_method("_setup_planet")) {
		return call("_setup_planet");
	}

	return Ref<Planet>(NULL);
}

WorldGenerator::WorldGenerator() {

}
WorldGenerator::~WorldGenerator() {
	_planet_datas.clear();
}

void WorldGenerator::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "planet", PROPERTY_HINT_RESOURCE_TYPE, "Planet"), "_setup_planet", PropertyInfo(Variant::INT, "seed")));

	ClassDB::bind_method(D_METHOD("setup_planet", "seed"), &WorldGenerator::setup_planet);

	ClassDB::bind_method(D_METHOD("get_planet_data", "index"), &WorldGenerator::get_planet_data);
	ClassDB::bind_method(D_METHOD("set_planet_data", "index", "data"), &WorldGenerator::set_planet_data);
	ClassDB::bind_method(D_METHOD("add_planet_data", "planet_data"), &WorldGenerator::add_planet_data);
	ClassDB::bind_method(D_METHOD("remove_planet_data", "index"), &WorldGenerator::remove_planet_data);

	ClassDB::bind_method(D_METHOD("get_planet_data_count"), &WorldGenerator::get_planet_data_count);

	ClassDB::bind_method(D_METHOD("get_planet_datas"), &WorldGenerator::get_planet_datas);
	ClassDB::bind_method(D_METHOD("set_planet_datas", "planet_datas"), &WorldGenerator::set_planet_datas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "planet_datas", PROPERTY_HINT_NONE, "17/17:PlanetData", PROPERTY_USAGE_DEFAULT, "PlanetData"), "set_planet_datas", "get_planet_datas");
}
