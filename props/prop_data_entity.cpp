#include "prop_data_entity.h"

int PropDataEntity::get_entity_data_id() const {
	return _entity_data_id;
}
void PropDataEntity::set_entity_data_id(const int value) {
	_entity_data_id = value;
}

int PropDataEntity::get_level() const {
	return _level;
}
void PropDataEntity::set_level(const int value) {
	_level = value;
}

PropDataEntity::PropDataEntity() {
	_entity_data_id = 0;
	_level = 1;
}
PropDataEntity::~PropDataEntity() {
}

void PropDataEntity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_data_id"), &PropDataEntity::get_entity_data_id);
	ClassDB::bind_method(D_METHOD("set_entity_data_id", "value"), &PropDataEntity::set_entity_data_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "entity_data_id"), "set_entity_data_id", "get_entity_data_id");

	ClassDB::bind_method(D_METHOD("get_level"), &PropDataEntity::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "value"), &PropDataEntity::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}
