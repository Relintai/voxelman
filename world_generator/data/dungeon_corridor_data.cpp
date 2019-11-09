#include "dungeon_corridor_data.h"

int DungeonCorridorData::get_max_connections() {
	return _max_connections;
}
void DungeonCorridorData::set_max_connections(int value) {
	_max_connections = value;
}

Ref<DungeonCorridor> DungeonCorridorData::setup_corridor(int seed) {
	if (has_method("_setup_corridor")) {
		return call("_setup_corridor", seed);
	}

	return Ref<DungeonCorridor>();
}

DungeonCorridorData::DungeonCorridorData() {
	_max_connections = 2;
}
DungeonCorridorData::~DungeonCorridorData() {

}

void DungeonCorridorData::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "corridor", PROPERTY_HINT_RESOURCE_TYPE, "DungeonCorridor"), "_setup_corridor", PropertyInfo(Variant::INT, "seed")));

	ClassDB::bind_method(D_METHOD("setup_corridor", "seed"), &DungeonCorridorData::setup_corridor);

	ClassDB::bind_method(D_METHOD("get_max_connections"), &DungeonCorridorData::get_max_connections);
	ClassDB::bind_method(D_METHOD("set_max_connections", "value"), &DungeonCorridorData::set_max_connections);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_connections"), "set_max_connections", "get_max_connections");
}
