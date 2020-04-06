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

#include "voxel_world_default.h"

#include "voxel_chunk_default.h"

_FORCE_INLINE_ int VoxelWorldDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void VoxelWorldDefault::set_build_flags(const int flags) {
	_build_flags = flags;
}

Ref<VoxelChunk> VoxelWorldDefault::_create_chunk(int x, int y, int z, Ref<VoxelChunk> chunk) {

	if (!chunk.is_valid()) {
		chunk = Ref<VoxelChunk>(memnew(VoxelChunkDefault));
	}

	Ref<VoxelChunkDefault> vcd = chunk;

	if (vcd.is_valid()) {
		vcd->set_build_flags(_build_flags);
	}

	return VoxelWorld::_create_chunk(x, y, z, chunk);
}

VoxelWorldDefault::VoxelWorldDefault() {
	_build_flags = VoxelChunkDefault::BUILD_FLAG_CREATE_COLLIDER | VoxelChunkDefault::BUILD_FLAG_CREATE_LODS;

	set_data_margin_start(1);
	set_data_margin_end(1);
}

VoxelWorldDefault ::~VoxelWorldDefault() {
}

void VoxelWorldDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelWorldDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelWorldDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");
}