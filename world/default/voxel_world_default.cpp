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

	for (int i = 0; i < get_chunk_count(); ++i) {
		Ref<VoxelChunkDefault> c = get_chunk_index(i);

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

void VoxelWorldDefault::update_lods() {
	call("_update_lods");
}

void VoxelWorldDefault::_update_lods() {
	if (!get_player() || !ObjectDB::instance_validate(get_player())) {
		return;
	}

	Vector3 ppos = get_player()->get_transform().origin;

	int ppx = int(ppos.x / (get_chunk_size_x() * get_voxel_scale()));
	int ppy = int(ppos.y / (get_chunk_size_y() * get_voxel_scale()));
	int ppz = int(ppos.z / (get_chunk_size_z() * get_voxel_scale()));

	for (int i = 0; i < get_chunk_count(); ++i) {
		Ref<VoxelChunkDefault> c = get_chunk_index(i);

		if (!c.is_valid())
			continue;

		int dx = abs(ppx - c->get_position_x());
		int dy = abs(ppy - c->get_position_y());
		int dz = abs(ppz - c->get_position_z());

		int mr = MAX(MAX(dx, dy), dz);

		if (mr <= 1)
			c->set_current_lod_level(0);
		else if (mr == 2)
			c->set_current_lod_level(1);
		else if (mr == 3) // || mr == 4)
			c->set_current_lod_level(2);
		else
			c->set_current_lod_level(3);
	}
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
		case CHANNEL_TYPE_INFO_LIQUID:
			return VoxelChunkDefault::DEFAULT_CHANNEL_LIQUID_TYPES;
		case CHANNEL_TYPE_INFO_LIQUID_LEVEL:
			return VoxelChunkDefault::DEFAULT_CHANNEL_LIQUID_FILL;
		default:
			return -1;
	}
}

VoxelWorldDefault::VoxelWorldDefault() {
	_lod_update_timer = 0;
	_lod_update_interval = 0.5;
	_build_flags = VoxelChunkDefault::BUILD_FLAG_CREATE_COLLIDER | VoxelChunkDefault::BUILD_FLAG_CREATE_LODS;

	set_data_margin_start(1);
	set_data_margin_end(1);
}

VoxelWorldDefault ::~VoxelWorldDefault() {
}

void VoxelWorldDefault::_notification(int p_what) {
	VoxelWorld::_notification(p_what);

	switch (p_what) {
		case NOTIFICATION_INTERNAL_PROCESS: {
			if ((get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_LODS) == 0)
				return;

			if (!get_player()) {
				return;
			}

			if (!ObjectDB::instance_validate(get_player())) {
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
}

void VoxelWorldDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_chunk_added", "chunk"), &VoxelWorldDefault::_chunk_added);

	ClassDB::bind_method(D_METHOD("get_build_flags"), &VoxelWorldDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &VoxelWorldDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, VoxelChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_lod_update_interval"), &VoxelWorldDefault::get_lod_update_interval);
	ClassDB::bind_method(D_METHOD("set_lod_update_interval", "value"), &VoxelWorldDefault::set_lod_update_interval);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "lod_update_interval"), "set_lod_update_interval", "get_lod_update_interval");

	BIND_VMETHOD(MethodInfo("_update_lods"));
	ClassDB::bind_method(D_METHOD("update_lods"), &VoxelWorldDefault::update_lods);
	ClassDB::bind_method(D_METHOD("_update_lods"), &VoxelWorldDefault::_update_lods);
}