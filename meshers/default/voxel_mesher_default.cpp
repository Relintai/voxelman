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

#include "../../defines.h"

#include visual_server_h
#include mesh_instance_h

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

void VoxelMesherDefault::_bake_colors(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	//if ((get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0)
	//	return;

	if (_vertices.size() == 0)
		return;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector3 vert = vertex.vertex;

		//Is this needed?
		if (vert.x < 0 || vert.y < 0 || vert.z < 0) {
			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _voxel_scale);
		unsigned int y = (unsigned int)(vert.y / _voxel_scale);
		unsigned int z = (unsigned int)(vert.z / _voxel_scale);

		if (chunk->validate_data_position(x, y, z)) {
			Color light = Color(
					chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0) * _ao_strength;
			float rao = chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;

			ao += rao;

			light.r += _base_light_value;
			light.g += _base_light_value;
			light.b += _base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			Color c = vertex.color;
			light.a = c.a;
			vertex.color = light;

			_vertices.set(i, vertex);
		} else {
			vertex.color = base_light;
			_vertices.set(i, vertex);
		}
	}
}

void VoxelMesherDefault::_bake_liquid_colors(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if ((get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0)
		return;

	if (_vertices.size() == 0)
		return;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector3 vert = vertex.vertex;

		//Is this needed?
		if (vert.x < 0 || vert.y < 0 || vert.z < 0) {
			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _voxel_scale);
		unsigned int y = (unsigned int)(vert.y / _voxel_scale);
		unsigned int z = (unsigned int)(vert.z / _voxel_scale);

		if (chunk->validate_data_position(x, y, z)) {
			Color light = Color(
					chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0) * _ao_strength;
			float rao = chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;

			ao += rao;

			light.r += _base_light_value;
			light.g += _base_light_value;
			light.b += _base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			Color c = vertex.color;
			light.a = c.a;
			vertex.color = light;

			_vertices.set(i, vertex);
		} else {
			vertex.color = base_light;
			_vertices.set(i, vertex);
		}
	}
}

VoxelMesherDefault::VoxelMesherDefault() {
	_build_flags = VoxelChunkDefault::BUILD_FLAG_CREATE_COLLIDER | VoxelChunkDefault::BUILD_FLAG_CREATE_LODS;

	_format = VisualServer::ARRAY_FORMAT_NORMAL | VisualServer::ARRAY_FORMAT_TEX_UV;
}

VoxelMesherDefault::~VoxelMesherDefault() {
}

void VoxelMesherDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_bake_colors", "chunk"), &VoxelMesherDefault::_bake_colors);
	ClassDB::bind_method(D_METHOD("_bake_liquid_colors", "chunk"), &VoxelMesherDefault::_bake_liquid_colors);

	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelMesherDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelMesherDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");
}
