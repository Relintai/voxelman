#include "prop_data_entry.h"

Transform PropDataEntry::get_transform() const {
	return _transform;
}
void PropDataEntry::set_transform(const Transform value) {
	_transform = value;
}

PropDataEntry::PropDataEntry() {

}
PropDataEntry::~PropDataEntry() {

}

void PropDataEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_transform"), &PropDataEntry::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &PropDataEntry::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");
}
