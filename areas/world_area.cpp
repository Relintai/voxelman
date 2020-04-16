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

#include "world_area.h"

AABB WorldArea::get_aabb() const {
	return _aabb;
}
void WorldArea::set_aabb(const AABB value) {
	_aabb = value;
}

Ref<Texture> WorldArea::get_map_texture() const {
	return _map_texture;
}
void WorldArea::set_map_texture(const Ref<Texture> &value) {
	_map_texture = value;
}

Ref<Texture> WorldArea::get_fov_texture() const {
	return _fov_texture;
}
void WorldArea::set_fov_texture(const Ref<Texture> &value) {
	_fov_texture = value;
}

String WorldArea::get_name() const {
	return _name;
}
void WorldArea::set_name(const String &value) {
	_name = value;
}

int WorldArea::get_level() const {
	return _level;
}
void WorldArea::set_level(const int level) {
	_level = level;
}

WorldArea::WorldArea() {
	_level = 0;
}

WorldArea::~WorldArea() {
}

void WorldArea::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_aabb"), &WorldArea::get_aabb);
	ClassDB::bind_method(D_METHOD("set_aabb"), &WorldArea::set_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "aabb"), "set_aabb", "get_aabb");

	ClassDB::bind_method(D_METHOD("get_map_texture"), &WorldArea::get_map_texture);
	ClassDB::bind_method(D_METHOD("set_map_texture"), &WorldArea::set_map_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "map_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_map_texture", "get_map_texture");

	ClassDB::bind_method(D_METHOD("get_fov_texture"), &WorldArea::get_fov_texture);
	ClassDB::bind_method(D_METHOD("set_fov_texture"), &WorldArea::set_fov_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fov_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_fov_texture", "get_fov_texture");

	ClassDB::bind_method(D_METHOD("get_name"), &WorldArea::get_name);
	ClassDB::bind_method(D_METHOD("set_name"), &WorldArea::set_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("get_level"), &WorldArea::get_level);
	ClassDB::bind_method(D_METHOD("set_level"), &WorldArea::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}
