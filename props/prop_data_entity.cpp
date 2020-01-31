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
