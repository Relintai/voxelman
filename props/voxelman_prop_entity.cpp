#include "voxelman_prop_entity.h"


int VoxelmanPropEntity::get_entity_data_id() const {
	return _entity_data_id;
}
void VoxelmanPropEntity::set_entity_data_id(const int value) {
	_entity_data_id = value;
}

int VoxelmanPropEntity::get_level() const {
	return _level;
}
void VoxelmanPropEntity::set_level(const int value) {
	_level = value;
}

VoxelmanPropEntity::VoxelmanPropEntity() {
	_entity_data_id = 0;
	_level = 1;
}
VoxelmanPropEntity::~VoxelmanPropEntity() {
}

void VoxelmanPropEntity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_data_id"), &VoxelmanPropEntity::get_entity_data_id);
	ClassDB::bind_method(D_METHOD("set_entity_data_id", "value"), &VoxelmanPropEntity::set_entity_data_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "entity_data_id"), "set_entity_data_id", "get_entity_data_id");

	ClassDB::bind_method(D_METHOD("get_level"), &VoxelmanPropEntity::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "value"), &VoxelmanPropEntity::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}
