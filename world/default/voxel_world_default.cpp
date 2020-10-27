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

#include "../../defines.h"

#include "../../meshers/default/voxel_mesher_default.h"
#include "../jobs/voxel_light_job.h"
#include "../jobs/voxel_prop_job.h"
#include "../jobs/voxel_terrarin_job.h"

_FORCE_INLINE_ int VoxelWorldDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void VoxelWorldDefault::set_build_flags(const int flags) {
	_build_flags = flags;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<VoxelChunkDefault> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_build_flags(_build_flags);
	}
}

float VoxelWorldDefault::get_lod_update_interval() const {
	return _lod_update_interval;
}
void VoxelWorldDefault::set_lod_update_interval(const float value) {
	_lod_update_interval = value;
}

int VoxelWorldDefault::get_num_lods() const {
	return _num_lods;
}
void VoxelWorldDefault::set_num_lods(const int value) {
	_num_lods = value;
}

void VoxelWorldDefault::update_lods() {
	call("_update_lods");
}

int VoxelWorldDefault::get_chunk_lod_falloff() const {
	return _chunk_lod_falloff;
}
void VoxelWorldDefault::set_chunk_lod_falloff(const int value) {
	_chunk_lod_falloff = value;
}

PoolColorArray VoxelWorldDefault::get_vertex_colors(const Transform &transform, const PoolVector3Array &vertices, const float base_light_value, const float ao_strength) {
	PoolColorArray arr;
	arr.resize(vertices.size());

	for (int i = 0; i < vertices.size(); ++i) {
		Vector3 v = transform.xform(vertices[i]);

		Vector3 pos = v / get_voxel_scale();

		//Note: floor is needed to handle negative numbers proiberly
		int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
		int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));
		int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

		int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
		int by = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_y();
		int bz = static_cast<int>(Math::floor(pos.z)) % get_chunk_size_z();

		if (bx < 0) {
			bx += get_chunk_size_x();
		}

		if (by < 0) {
			by += get_chunk_size_y();
		}

		if (bz < 0) {
			bz += get_chunk_size_z();
		}

		Ref<VoxelChunk> chunk = chunk_get(x, y, z);

		if (chunk.is_valid()) {
			Color light = Color(
					chunk->get_voxel(bx, by, bz, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(bx, by, bz, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(bx, by, bz, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(bx, by, bz, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0) * ao_strength;
			float rao = chunk->get_voxel(bx, by, bz, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;

			ao += rao;

			light.r += base_light_value;
			light.g += base_light_value;
			light.b += base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			arr.set(i, light);
		} else {
			arr.set(i, Color(base_light_value, base_light_value, base_light_value));
		}
	}

	return arr;
}

void VoxelWorldDefault::_update_lods() {
	if (!get_player() || !INSTANCE_VALIDATE(get_player())) {
		return;
	}

	if (_num_lods <= 1)
		return;

	Vector3 ppos = get_player()->get_transform().origin;

	int ppx = int(ppos.x / get_chunk_size_x() / get_voxel_scale());
	int ppy = int(ppos.y / get_chunk_size_y() / get_voxel_scale());
	int ppz = int(ppos.z / get_chunk_size_z() / get_voxel_scale());

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<VoxelChunkDefault> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		int dx = Math::abs(ppx - c->get_position_x());
		int dy = Math::abs(ppy - c->get_position_y());
		int dz = Math::abs(ppz - c->get_position_z());

		int mr = MAX(MAX(dx, dy), dz);

		mr -= _chunk_lod_falloff;

		//Todo 3 should be _num_lod, but it's NYI, because chunk can only handle 3 lod levels for now -> FQMS needs to be fixed
		mr = CLAMP(mr, 0, _num_lods - 1);

		if (c->get_current_lod_level() != mr)
			c->set_current_lod_level(mr);
	}
}

Ref<VoxelChunk> VoxelWorldDefault::_create_chunk(int x, int y, int z, Ref<VoxelChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<VoxelChunk>(memnew(VoxelChunkDefault));
	}

	if (chunk->job_get_count() == 0) {
		Ref<VoxelTerrarinJob> tj;
		tj.instance();

		Ref<VoxelLightJob> lj;
		lj.instance();

		Ref<VoxelPropJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<VoxelMesher>(memnew(VoxelMesherDefault)));

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	Ref<VoxelChunkDefault> vcd = chunk;

	if (vcd.is_valid()) {
		vcd->set_build_flags(_build_flags);
		vcd->set_lod_num(_num_lods);
	}

	return VoxelWorld::_create_chunk(x, y, z, chunk);
}

void VoxelWorldDefault::_chunk_added(Ref<VoxelChunk> chunk) {
	Ref<VoxelChunkDefault> c = chunk;

	if (c.is_valid()) {
		c->set_build_flags(_build_flags);
	}
}

int VoxelWorldDefault::_get_channel_index_info(const VoxelWorld::ChannelTypeInfo channel_type) {
	switch (channel_type) {
		case CHANNEL_TYPE_INFO_TYPE:
			return VoxelChunkDefault::DEFAULT_CHANNEL_TYPE;
		case CHANNEL_TYPE_INFO_ISOLEVEL:
			return VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL;
		case CHANNEL_TYPE_INFO_LIQUID_FLOW:
			return VoxelChunkDefault::DEFAULT_CHANNEL_LIQUID_FLOW;
		default:
			return -1;
	}
}

VoxelWorldDefault::VoxelWorldDefault() {
	_chunk_lod_falloff = 2;
	_lod_update_timer = 0;
	_lod_update_interval = 0.5;
	_build_flags = VoxelChunkDefault::BUILD_FLAG_CREATE_COLLIDER | VoxelChunkDefault::BUILD_FLAG_CREATE_LODS;
	_num_lods = 4;

	set_data_margin_start(1);
	set_data_margin_end(1);
}

VoxelWorldDefault ::~VoxelWorldDefault() {
}

/*
void VoxelWorldDefault::_notification(int p_what) {
	VoxelWorld::_notification(p_what);

	switch (p_what) {
		case NOTIFICATION_INTERNAL_PROCESS: {
			if ((get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_LODS) == 0)
				return;

			if (!get_player()) {
				return;
			}

			if (!INSTANCE_VALIDATE(get_player())) {
				set_player(NULL);
				return;
			}

			_lod_update_timer += get_process_delta_time();

			if (_lod_update_timer >= _lod_update_interval) {
				_lod_update_timer = 0;

				update_lods();
			}
		} break;
	}
}*/

void VoxelWorldDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_chunk_added", "chunk"), &VoxelWorldDefault::_chunk_added);

	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelWorldDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelWorldDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_lod_update_interval"), &VoxelWorldDefault::get_lod_update_interval);
	ClassDB::bind_method(D_METHOD("set_lod_update_interval", "value"), &VoxelWorldDefault::set_lod_update_interval);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lod_update_interval"), "set_lod_update_interval", "get_lod_update_interval");

	ClassDB::bind_method(D_METHOD("get_chunk_lod_falloff"), &VoxelWorldDefault::get_chunk_lod_falloff);
	ClassDB::bind_method(D_METHOD("set_chunk_lod_falloff", "value"), &VoxelWorldDefault::set_chunk_lod_falloff);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_lod_falloff"), "set_chunk_lod_falloff", "get_chunk_lod_falloff");

	ClassDB::bind_method(D_METHOD("get_num_lods"), &VoxelWorldDefault::get_num_lods);
	ClassDB::bind_method(D_METHOD("set_num_lods", "value"), &VoxelWorldDefault::set_num_lods);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_lods"), "set_num_lods", "get_num_lods");

	BIND_VMETHOD(MethodInfo("_update_lods"));
	ClassDB::bind_method(D_METHOD("update_lods"), &VoxelWorldDefault::update_lods);
	ClassDB::bind_method(D_METHOD("_update_lods"), &VoxelWorldDefault::_update_lods);

	ClassDB::bind_method(D_METHOD("get_vertex_colors", "transform", "vertices", "base_light_value", "ao_strength"), &VoxelWorldDefault::get_vertex_colors, DEFVAL(0.45), DEFVAL(0.2));
}