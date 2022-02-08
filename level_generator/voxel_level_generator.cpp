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

#include "voxel_level_generator.h"

#include "core/version.h"

#include "../world/voxel_chunk.h"

void VoxelLevelGenerator::generate_chunk(Ref<VoxelChunk> chunk) {
#if VERSION_MAJOR < 4
	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk);
	}
#else
	GDVIRTUAL_CALL(_generate_chunk, chunk);
#endif
}

VoxelLevelGenerator::VoxelLevelGenerator() {
}

VoxelLevelGenerator::~VoxelLevelGenerator() {
}

void VoxelLevelGenerator::_bind_methods() {
#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
#else
	GDVIRTUAL_BIND(_generate_chunk, "chunk");
#endif

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk"), &VoxelLevelGenerator::generate_chunk);
}
