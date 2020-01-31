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

#include "ground_clutter_foliage.h"

int GroundClutterFoliage::get_texture_count() const {
	return _textures.size();
}
Ref<Texture> GroundClutterFoliage::get_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<Texture>());

	return _textures.get(index);
}
void GroundClutterFoliage::remove_texture(const int index) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.remove(index);
}
void GroundClutterFoliage::add_texture(Ref<Texture> texture) {
	_textures.push_back(texture);
}

GroundClutterFoliage::GroundClutterFoliage() {
}

GroundClutterFoliage::~GroundClutterFoliage() {
	_textures.clear();
}

void GroundClutterFoliage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_count"), &GroundClutterFoliage::get_texture_count);
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &GroundClutterFoliage::get_texture);
	ClassDB::bind_method(D_METHOD("remove_texture", "index"), &GroundClutterFoliage::remove_texture);
	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &GroundClutterFoliage::add_texture);
}
