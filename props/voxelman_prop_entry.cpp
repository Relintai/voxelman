#include "voxelman_prop_entry.h"

Transform VoxelmanPropEntry::get_transform() const {
	return _transform;
}
void VoxelmanPropEntry::set_transform(const Transform value) {
	_transform = value;
}

VoxelmanPropEntry::VoxelmanPropEntry() {

}
VoxelmanPropEntry::~VoxelmanPropEntry() {

}

void VoxelmanPropEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_transform"), &VoxelmanPropEntry::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &VoxelmanPropEntry::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");
}
