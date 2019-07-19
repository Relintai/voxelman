#include "voxelman_prop.h"

Ref<VoxelmanPropData> VoxelmanProp::get_prop(const int index) const {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<VoxelmanPropData>());

	return _props.get(index);
}
void VoxelmanProp::set_prop(const int index, const Ref<VoxelmanPropData> prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.set(index, prop);
}

int VoxelmanProp::get_prop_count() const {
	return _props.size();
}
void VoxelmanProp::set_prop_count(const int size) {
	if (size > MAX_PROPS) {
		_props.resize(MAX_PROPS);
		return;
	}

	_props.resize(size);
}

VoxelmanProp::VoxelmanProp() {
}
VoxelmanProp::~VoxelmanProp() {
	_props.clear();
}

void VoxelmanProp::_validate_property(PropertyInfo &property) const {

	String prop = property.name;
	if (prop.begins_with("Prop_")) {
		int num = prop.get_slicec('/', 0).get_slicec('_', 1).to_int();
		if (num >= _props.size()) {
			property.usage = 0;
		}
	}
}

void VoxelmanProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_count"), &VoxelmanProp::get_prop_count);
	ClassDB::bind_method(D_METHOD("set_prop_count", "value"), &VoxelmanProp::set_prop_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "prop_count", PROPERTY_HINT_RANGE, "0," + itos(MAX_PROPS), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_prop_count", "get_prop_count");

	ClassDB::bind_method(D_METHOD("get_prop", "index"), &VoxelmanProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "index", "spell"), &VoxelmanProp::set_prop);

	for (int i = 0; i < MAX_PROPS; ++i) {
		ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "Prop_" + itos(i), PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanPropData", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_INTERNAL), "set_prop", "get_prop", i);
	}

	BIND_CONSTANT(MAX_PROPS);
}
