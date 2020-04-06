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

#include "voxel_mesher_default.h"

#include "../../world/default/voxel_chunk_default.h"

_FORCE_INLINE_ int VoxelMesherDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void VoxelMesherDefault::set_build_flags(const int flags) {
	_build_flags = flags;

	if ((_build_flags & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		_format |= VisualServer::ARRAY_FORMAT_COLOR;
	} else {
		_format ^= VisualServer::ARRAY_FORMAT_COLOR;
	}
}

VoxelMesherDefault::VoxelMesherDefault() {
	_build_flags = VoxelChunkDefault::BUILD_FLAG_CREATE_COLLIDER | VoxelChunkDefault::BUILD_FLAG_CREATE_LODS;

	_format = VisualServer::ARRAY_FORMAT_NORMAL | VisualServer::ARRAY_FORMAT_TEX_UV;
}

VoxelMesherDefault::~VoxelMesherDefault() {
}

void VoxelMesherDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelMesherDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelMesherDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");
}
