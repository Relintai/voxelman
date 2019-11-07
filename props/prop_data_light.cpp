#include "prop_data_light.h"


Color PropDataLight::get_light_color() const {
	return _light_color;
}
void PropDataLight::set_light_color(const Color value) {
	_light_color = value;
}

int PropDataLight::get_light_size() const {
	return _light_size;
}
void PropDataLight::set_light_size(const int value) {
	_light_size = value;
}

PropDataLight::PropDataLight() {
	_light_size = 5;
}
PropDataLight::~PropDataLight() {
}

void PropDataLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_light_color"), &PropDataLight::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &PropDataLight::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_size"), &PropDataLight::get_light_size);
	ClassDB::bind_method(D_METHOD("set_light_size", "value"), &PropDataLight::set_light_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size"), "set_light_size", "get_light_size");
}
