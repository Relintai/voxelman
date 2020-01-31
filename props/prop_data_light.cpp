/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
