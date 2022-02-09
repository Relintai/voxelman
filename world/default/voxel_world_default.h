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

#ifndef VOXEL_WORLD_DEFAULT_H
#define VOXEL_WORLD_DEFAULT_H

#include "../voxel_world.h"

class VoxelWorldDefault : public VoxelWorld {
	GDCLASS(VoxelWorldDefault, VoxelWorld);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	bool get_use_default_lod_update() const;
	void set_use_default_lod_update(const bool value);

	float get_lod_update_interval() const;
	void set_lod_update_interval(const float value);

	int get_chunk_lod_falloff() const;
	void set_chunk_lod_falloff(const int value);

	int get_num_lods() const;
	void set_num_lods(const int value);

	void update_lods();

	PoolColorArray get_vertex_colors(const Transform &transform, const PoolVector3Array &vertices, const float base_light_value = 0.45, const float ao_strength = 0.2);

#if VERSION_MAJOR >= 4
	GDVIRTUAL0(_update_lods);
#endif

	VoxelWorldDefault();
	~VoxelWorldDefault();

protected:
	void _update_lods();
	Ref<VoxelChunk> _create_chunk(int x, int y, int z, Ref<VoxelChunk> p_chunk);
	virtual void _chunk_added(Ref<VoxelChunk> chunk);
	int _get_channel_index_info(const ChannelTypeInfo channel_type);

	virtual void _notification(int p_what);

	static void _bind_methods();

private:
	int _build_flags;
	bool _use_default_lod_update;
	float _lod_update_timer;
	float _lod_update_interval;
	int _chunk_lod_falloff;
	int _num_lods;
};

#endif
