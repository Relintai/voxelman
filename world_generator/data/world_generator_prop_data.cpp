#include "world_generator_prop_data.h"

bool WorldGeneratorPropData::can_spawn(int seed) {
	if (has_method("_can_spawn")) {
		return call("_can_spawn", seed);
	}

	return false;
}

Ref<PropData> WorldGeneratorPropData::get_prop(int seed) {
	if (has_method("_get_prop")) {
		return call("_get_prop", seed);
	}

	return Ref<PropData>(NULL);
}

WorldGeneratorPropData::WorldGeneratorPropData() {

}
WorldGeneratorPropData::~WorldGeneratorPropData() {

}

void WorldGeneratorPropData::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "can"), "_can_spawn", PropertyInfo(Variant::INT, "seed")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "_get_prop", PropertyInfo(Variant::INT, "seed")));

	ClassDB::bind_method(D_METHOD("can_spawn", "seed"), &WorldGeneratorPropData::can_spawn);
	ClassDB::bind_method(D_METHOD("get_prop", "seed"), &WorldGeneratorPropData::get_prop);
}
