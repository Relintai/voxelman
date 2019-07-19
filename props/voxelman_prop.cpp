#include "voxelman_prop.h"

Ref<VoxelmanPropData> VoxelmanProp::get_prop(const int index) const {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<VoxelmanPropData>());

	return _props.get(index);
}
void VoxelmanProp::set_prop(const int index, const Ref<VoxelmanPropData> prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.set(index, prop);
}

int VoxelmanProp::get_num_props(const int index) const {
	return _num_props;
}
void VoxelmanProp::set_num_props(const int size) {
	_num_props = size;

	if (_num_props > MAX_PROPS) {
		_num_props = MAX_PROPS;
	}

	_props.resize(size);
}

VoxelmanProp::VoxelmanProp() {
	_num_props = 0;
}
VoxelmanProp::~VoxelmanProp() {
	_props.clear();
}

void VoxelmanProp::_validate_property(PropertyInfo &property) const {

	String prop = property.name;
	if (prop.begins_with("Prop_")) {
		int num = prop.get_slicec('/', 0).get_slicec('_', 1).to_int();
		if (num >= _num_props) {
			property.usage = 0;
		}
	}
}

void VoxelmanProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_num_props"), &VoxelmanProp::get_num_props);
	ClassDB::bind_method(D_METHOD("set_num_props", "value"), &VoxelmanProp::set_num_props);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_props", PROPERTY_HINT_RANGE, "0," + itos(MAX_PROPS), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_num_props", "get_num_props");

	ClassDB::bind_method(D_METHOD("get_prop", "index"), &VoxelmanProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "index", "spell"), &VoxelmanProp::set_prop);

	for (int i = 0; i < MAX_PROPS; ++i) {
		ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "Prop_" + itos(i), PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanPropData", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_prop", "get_prop", i);
	}

	BIND_CONSTANT(MAX_PROPS);
}
