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

#include "voxel_world.h"

#include "core/version.h"

#include "voxel_chunk.h"
#include "voxel_structure.h"

#include "../defines.h"

#if PROPS_PRESENT
#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_entry.h"
#include "../../props/props/prop_data_light.h"
#include "../../props/props/prop_data_prop.h"
#include "../../props/props/prop_data_scene.h"
#endif

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif

const String VoxelWorld::BINDING_STRING_CHANNEL_TYPE_INFO = "Type,Isolevel,Liquid,Liquid Level";

bool VoxelWorld::get_editable() const {
	return _editable;
}
void VoxelWorld::set_editable(const bool value) {
	_editable = value;
}

int VoxelWorld::get_chunk_size_x() const {
	return _chunk_size_x;
}
void VoxelWorld::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int VoxelWorld::get_chunk_size_y() const {
	return _chunk_size_y;
}
void VoxelWorld::set_chunk_size_y(const int value) {
	_chunk_size_y = value;
}

int VoxelWorld::get_chunk_size_z() const {
	return _chunk_size_z;
}
void VoxelWorld::set_chunk_size_z(const int value) {
	_chunk_size_z = value;
}

int VoxelWorld::get_data_margin_start() const {
	return _data_margin_start;
}
void VoxelWorld::set_data_margin_start(const int value) {
	_data_margin_start = value;
}

int VoxelWorld::get_data_margin_end() const {
	return _data_margin_end;
}
void VoxelWorld::set_data_margin_end(const int value) {
	_data_margin_end = value;
}

int VoxelWorld::get_current_seed() const {
	return _current_seed;
}
void VoxelWorld::set_current_seed(const int value) {
	_current_seed = value;
}

bool VoxelWorld::get_use_threads() const {
	return _use_threads;
}
void VoxelWorld::set_use_threads(const bool value) {
	_use_threads = OS::get_singleton()->can_use_threads() ? value : false;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<VoxelChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_is_build_threaded(_use_threads);
	}
}

int VoxelWorld::get_max_concurrent_generations() const {
	return _max_concurrent_generations;
}
void VoxelWorld::set_max_concurrent_generations(const int value) {
	_max_concurrent_generations = OS::get_singleton()->can_use_threads() ? value : 1;
}

int VoxelWorld::get_max_frame_chunk_build_steps() const {
	return _max_frame_chunk_build_steps;
}
void VoxelWorld::set_max_frame_chunk_build_steps(const int value) {
	_max_frame_chunk_build_steps = value;
}

Ref<VoxelLibrary> VoxelWorld::get_library() {
	return _library;
}
void VoxelWorld::set_library(const Ref<VoxelLibrary> &library) {
	_library = library;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<VoxelChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_library(_library);
	}
}

Ref<VoxelLevelGenerator> VoxelWorld::get_level_generator() const {
	return _level_generator;
}
void VoxelWorld::set_level_generator(const Ref<VoxelLevelGenerator> &level_generator) {
	_level_generator = level_generator;
}

float VoxelWorld::get_voxel_scale() const {
	return _voxel_scale;
}
void VoxelWorld::set_voxel_scale(const float value) {
	_voxel_scale = value;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<VoxelChunk> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_voxel_scale(_voxel_scale);
	}
}

int VoxelWorld::get_chunk_spawn_range() const {
	return _chunk_spawn_range;
}
void VoxelWorld::set_chunk_spawn_range(const int value) {
	_chunk_spawn_range = value;
}

NodePath VoxelWorld::get_player_path() const {
	return _player_path;
}

void VoxelWorld::set_player_path(const NodePath &player_path) {
	_player_path = player_path;
}

Spatial *VoxelWorld::get_player() const {
	return _player;
}
void VoxelWorld::set_player(Spatial *player) {
	_player = player;
}
void VoxelWorld::set_player_bind(Node *player) {
	set_player(Object::cast_to<Spatial>(player));
}

Ref<WorldArea> VoxelWorld::world_area_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _world_areas.size(), Ref<WorldArea>());

	return _world_areas.get(index);
}
void VoxelWorld::world_area_add(const Ref<WorldArea> &area) {
	_world_areas.push_back(area);
}
void VoxelWorld::world_area_remove(const int index) {
	ERR_FAIL_INDEX(index, _world_areas.size());

#if VERSION_MAJOR < 4
	_world_areas.remove(index);
#else
	_world_areas.remove_at(index);
#endif
}
void VoxelWorld::world_areas_clear() {
	_world_areas.clear();
}
int VoxelWorld::world_area_get_count() const {
	return _world_areas.size();
}

//Voxel Structures

Ref<VoxelStructure> VoxelWorld::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<VoxelStructure>());

	return _voxel_structures.get(index);
}
void VoxelWorld::voxel_structure_add(const Ref<VoxelStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void VoxelWorld::voxel_structure_remove(const Ref<VoxelStructure> &structure) {
	if (!structure.is_valid())
		return;

	int index = _voxel_structures.find(structure);

	if (index != -1) {
#if VERSION_MAJOR < 4
		_voxel_structures.remove(index);
#else
		_voxel_structures.remove_at(index);
#endif
	}
}
void VoxelWorld::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

#if VERSION_MAJOR < 4
	_voxel_structures.remove(index);
#else
	_voxel_structures.remove_at(index);
#endif
}
void VoxelWorld::voxel_structures_clear() {
	_voxel_structures.clear();
}
int VoxelWorld::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void VoxelWorld::voxel_structure_add_at_position(Ref<VoxelStructure> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _voxel_scale));
	structure->set_position_y(static_cast<int>(world_position.y / _voxel_scale));
	structure->set_position_z(static_cast<int>(world_position.z / _voxel_scale));

	voxel_structure_add(structure);
}

Vector<Variant> VoxelWorld::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void VoxelWorld::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structures_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<VoxelLight> structure = Ref<VoxelLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

void VoxelWorld::chunk_add(Ref<VoxelChunk> chunk, const int x, const int y, const int z) {
	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND_MSG(chunk->get_voxel_world() != NULL && chunk->get_voxel_world() != this, "Chunk is already owned by an another world!");

	IntPos pos(x, y, z);

	//ERR_FAIL_COND(_chunks.has(pos));

	chunk->set_voxel_world(this);
	chunk->set_position(x, y, z);
	chunk->world_transform_changed();

	if (!_chunks.has(pos))
		_chunks.set(pos, chunk);

	if (_chunks_vector.find(chunk) == -1)
		_chunks_vector.push_back(chunk);

	if (is_inside_tree())
		chunk->enter_tree();

	if (has_method("_chunk_added")) {
#if VERSION_MAJOR < 4
		call("_chunk_added", chunk);
#else
		GDVIRTUAL_CALL(_chunk_added, chunk);
#endif
	}
}
bool VoxelWorld::chunk_has(const int x, const int y, const int z) const {
	return _chunks.has(IntPos(x, y, z));
}
Ref<VoxelChunk> VoxelWorld::chunk_get(const int x, const int y, const int z) {
	IntPos pos(x, y, z);

	if (_chunks.has(pos))
		return _chunks.get(pos);

	return Ref<VoxelChunk>();
}
Ref<VoxelChunk> VoxelWorld::chunk_remove(const int x, const int y, const int z) {
	IntPos pos(x, y, z);

	if (!_chunks.has(pos))
		return NULL;

	Ref<VoxelChunk> chunk = _chunks.get(pos);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		if (_chunks_vector.get(i) == chunk) {
#if VERSION_MAJOR < 4
			_chunks_vector.remove(i);
#else
			_chunks_vector.remove_at(i);
#endif
			break;
		}
	}

	chunk->exit_tree();

	ERR_FAIL_COND_V(!_chunks.erase(pos), NULL);

	//_chunks.erase(pos);

	return chunk;
}
Ref<VoxelChunk> VoxelWorld::chunk_remove_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	Ref<VoxelChunk> chunk = _chunks_vector.get(index);
#if VERSION_MAJOR < 4
	_chunks_vector.remove(index);
#else
	_chunks_vector.remove_at(index);
#endif
	_chunks.erase(IntPos(chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z()));
	chunk->exit_tree();

	return chunk;
}

Ref<VoxelChunk> VoxelWorld::chunk_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _chunks_vector.size(), NULL);

	return _chunks_vector.get(index);
}
int VoxelWorld::chunk_get_count() const {
	return _chunks_vector.size();
}

void VoxelWorld::chunks_clear() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		_chunks_vector.get(i)->exit_tree();
	}

	_chunks_vector.clear();

	_chunks.clear();

	_generation_queue.clear();
	_generating.clear();
}

Ref<VoxelChunk> VoxelWorld::chunk_get_or_create(int x, int y, int z) {
	Ref<VoxelChunk> chunk = chunk_get(x, y, z);

	if (!chunk.is_valid()) {
		chunk = chunk_create(x, y, z);
	}

	return chunk;
}

Ref<VoxelChunk> VoxelWorld::chunk_create(const int x, const int y, const int z) {
#if VERSION_MAJOR < 4
	Ref<VoxelChunk> c = call("_create_chunk", x, y, z, Ref<VoxelChunk>());
#else
	Ref<VoxelChunk> c;
	GDVIRTUAL_CALL(_create_chunk, x, y, z, Ref<VoxelChunk>(), c);
#endif

	generation_queue_add_to(c);

	return c;
}

void VoxelWorld::chunk_setup(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

#if VERSION_MAJOR < 4
	call("_create_chunk", chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z(), chunk);
#else
	Ref<VoxelChunk> c;
	GDVIRTUAL_CALL(_create_chunk, chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z(), chunk, c);
#endif
}

Ref<VoxelChunk> VoxelWorld::_create_chunk(const int x, const int y, const int z, Ref<VoxelChunk> chunk) {
	if (!chunk.is_valid()) {
#if VERSION_MAJOR < 4
		chunk.instance();
#else
		chunk.instantiate();
#endif
	}

	//no meshers here

	ERR_FAIL_COND_V(!chunk.is_valid(), NULL);

	chunk->set_name("Chunk[" + String::num(x) + "," + String::num(y) + "," + String::num(z) + "]");

	chunk->set_voxel_world(this);

	//TODO this will need to be changed
	if (chunk->has_method("set_is_build_threaded"))
		chunk->call("set_is_build_threaded", _use_threads);

	chunk->set_position(x, y, z);
	chunk->set_library(_library);
	chunk->set_voxel_scale(_voxel_scale);
	chunk->set_size(_chunk_size_x, _chunk_size_y, _chunk_size_z, _data_margin_start, _data_margin_end);
	//chunk->set_translation(Vector3(x * _chunk_size_x * _voxel_scale, y * _chunk_size_y * _voxel_scale, z * _chunk_size_z * _voxel_scale));

	chunk_add(chunk, x, y, z);

	return chunk;
}

void VoxelWorld::chunk_generate(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_prepare_chunk_for_generation")) {
#if VERSION_MAJOR < 4
		call("_prepare_chunk_for_generation", chunk);
#else
		GDVIRTUAL_CALL(_prepare_chunk_for_generation, chunk);
#endif
	}

#if VERSION_MAJOR < 4
	call("_generate_chunk", chunk);
#else
	GDVIRTUAL_CALL(_generate_chunk, chunk);
#endif

	chunk->build();
}

Vector<Variant> VoxelWorld::chunks_get() {
	VARIANT_ARRAY_GET(_chunks_vector);
}

void VoxelWorld::chunks_set(const Vector<Variant> &chunks) {
	if (is_inside_tree()) {
		for (int i = 0; i < _chunks_vector.size(); ++i) {
			Ref<VoxelChunk> chunk = Ref<VoxelChunk>(_chunks_vector[i]);

			if (chunks.find(chunk) == -1) {
				chunk_remove_index(i);
				_generation_queue.erase(chunk);
				_generating.erase(chunk);
				--i;
			}
		}

		//add the difference
		for (int i = 0; i < chunks.size(); ++i) {
			Ref<VoxelChunk> chunk = Ref<VoxelChunk>(chunks[i]);

			if (!chunk.is_valid())
				continue;

			if (_chunks_vector.find(chunk) != -1)
				continue;

			chunk_add(chunk, chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z());
		}
	} else {
		_chunks_vector.clear();

		for (int i = 0; i < chunks.size(); ++i) {
			Ref<VoxelChunk> chunk = Ref<VoxelChunk>(chunks[i]);

			_chunks_vector.push_back(chunk);
		}
	}
}

int VoxelWorld::_get_channel_index_info(const VoxelWorld::ChannelTypeInfo channel_type) {
	return -1;
}

void VoxelWorld::_set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel) {
	Vector3 pos;

	if (mode_add) {
		pos = (hit_position + (Vector3(0.1, 0.1, 0.1) * hit_normal * get_voxel_scale()));
	} else {
		pos = (hit_position + (Vector3(0.1, 0.1, 0.1) * -hit_normal * get_voxel_scale()));
	}

	int channel_type = get_channel_index_info(VoxelWorld::CHANNEL_TYPE_INFO_TYPE);
	int channel_isolevel = get_channel_index_info(VoxelWorld::CHANNEL_TYPE_INFO_ISOLEVEL);

	if (channel_isolevel == -1) {
		set_voxel_at_world_position(pos, selected_voxel, channel_type);
	} else {
		set_voxel_at_world_position(pos, selected_voxel, channel_type, false);
		set_voxel_at_world_position(pos, isolevel, channel_isolevel);
	}
}

bool VoxelWorld::can_chunk_do_build_step() {
	if (_max_frame_chunk_build_steps == 0) {
		return true;
	}

	return _num_frame_chunk_build_steps++ < _max_frame_chunk_build_steps;
}

bool VoxelWorld::is_position_walkable(const Vector3 &p_pos) {
	int x = static_cast<int>(Math::floor(p_pos.x / (_chunk_size_x * _voxel_scale)));
	int y = static_cast<int>(Math::floor(p_pos.y / (_chunk_size_y * _voxel_scale)));
	int z = static_cast<int>(Math::floor(p_pos.z / (_chunk_size_z * _voxel_scale)));

	Ref<VoxelChunk> c = chunk_get(x, y, z);

	if (!c.is_valid())
		return false;

	return !c->get_is_generating();
}

void VoxelWorld::on_chunk_mesh_generation_finished(Ref<VoxelChunk> p_chunk) {
	call_deferred("emit_signal", "chunk_mesh_generation_finished", p_chunk);
}

void VoxelWorld::generation_queue_add_to(const Ref<VoxelChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	set_process_internal(true);

	_generation_queue.push_back(chunk);
}
Ref<VoxelChunk> VoxelWorld::generation_queue_get_index(int index) {
	ERR_FAIL_INDEX_V(index, _generation_queue.size(), NULL);

	return _generation_queue.get(index);
}
void VoxelWorld::generation_queue_remove_index(int index) {
	ERR_FAIL_INDEX(index, _generation_queue.size());

#if VERSION_MAJOR < 4
	_generation_queue.remove(index);
#else
	_generation_queue.remove_at(index);
#endif
}
int VoxelWorld::generation_queue_get_size() const {
	return _generation_queue.size();
}

void VoxelWorld::generation_add_to(const Ref<VoxelChunk> &chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	_generating.push_back(chunk);
}
Ref<VoxelChunk> VoxelWorld::generation_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _generating.size(), NULL);

	return _generating.get(index);
}
void VoxelWorld::generation_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _generating.size());

#if VERSION_MAJOR < 4
	_generating.remove(index);
#else
	_generating.remove_at(index);
#endif
}
int VoxelWorld::generation_get_size() const {
	return _generating.size();
}

#if PROPS_PRESENT
void VoxelWorld::prop_add(Transform tarnsform, const Ref<PropData> &prop, const bool apply_voxel_scael) {
	ERR_FAIL_COND(!prop.is_valid());

	if (apply_voxel_scael) {
		tarnsform = tarnsform.scaled(Vector3(get_voxel_scale(), get_voxel_scale(), get_voxel_scale()));
	}

	Vector3 wp;
	wp = tarnsform.xform(wp);
	Ref<VoxelChunk> chunk = get_or_create_chunk_at_world_position(wp);

	chunk->prop_add(tarnsform, prop);

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<PropDataEntry> entry = prop->get_prop(i);

		if (!entry.is_valid())
			continue;

		Transform t = tarnsform * entry->get_transform();

		wp = t.xform(Vector3());
		chunk = get_or_create_chunk_at_world_position(wp);

		Ref<PropDataProp> prop_entry_data = entry;

		if (prop_entry_data.is_valid()) {
			Ref<PropData> p = prop_entry_data->get_prop();

			if (!p.is_valid())
				continue;

			prop_add(t, p, false);

			continue;
		}

		Ref<PropDataScene> scene_data = entry;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid())
				continue;

#if VERSION_MAJOR < 4
			Node *n = sc->instance();
#else
			Node *n = sc->instantiate();
#endif
			add_child(n);
			n->set_owner(this);

			Spatial *sp = Object::cast_to<Spatial>(n);

			if (sp) {
				sp->set_transform(t);
			}

			continue;
		}

		Ref<PropDataLight> light_data = entry;

		if (light_data.is_valid()) {
			Ref<VoxelLight> light;
#if VERSION_MAJOR < 4
			light.instance();
#else
			light.instantiate();
#endif

			light->set_world_position(wp.x / get_voxel_scale(), wp.y / get_voxel_scale(), wp.z / get_voxel_scale());
			light->set_color(light_data->get_light_color());
			light->set_size(light_data->get_light_size());

			light_add(light);

			continue;
		}

#if MESH_DATA_RESOURCE_PRESENT
		Ref<PropDataMeshData> mesh_data = entry;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid()) {
				continue;
			}

			chunk->mesh_data_resource_add(t, mdr, mesh_data->get_texture());

			continue;
		}
#endif
	}
}
#endif

//Lights
void VoxelWorld::light_add(const Ref<VoxelLight> &light) {
	_lights.push_back(light);

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<VoxelChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_added(light);
		}
	}
}
Ref<VoxelLight> VoxelWorld::light_get(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<VoxelLight>());

	return _lights.get(index);
}
void VoxelWorld::light_remove(const int index) {
	ERR_FAIL_INDEX(index, _lights.size());

	Ref<VoxelLight> light = _lights[index];

	for (int i = 0; i < _chunks_vector.size(); ++i) {
		Ref<VoxelChunk> chunk = _chunks_vector[i];

		if (chunk.is_valid()) {
			chunk->world_light_removed(light);
		}
	}
}
int VoxelWorld::light_get_count() const {
	return _lights.size();
}
void VoxelWorld::lights_clear() {
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<VoxelLight> light = _lights[i];

		if (!light.is_valid())
			continue;

		for (int j = 0; j < _chunks_vector.size(); ++j) {
			Ref<VoxelChunk> chunk = _chunks_vector[j];

			if (chunk.is_valid()) {
				chunk->world_light_removed(light);
			}
		}
	}

	_lights.clear();
}

Vector<Variant> VoxelWorld::lights_get() {
	VARIANT_ARRAY_GET(_lights);
}
void VoxelWorld::lights_set(const Vector<Variant> &chunks) {
	lights_clear();

	for (int i = 0; i < chunks.size(); ++i) {
		Ref<VoxelLight> light = Ref<VoxelLight>(chunks[i]);

		light_add(light);
	}
}

uint8_t VoxelWorld::get_voxel_at_world_position(const Vector3 &world_position, const int channel_index) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
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

	if (chunk.is_valid())
		return chunk->get_voxel(bx, by, bz, channel_index);

	return 0;
}

void VoxelWorld::set_voxel_at_world_position(const Vector3 &world_position, const uint8_t data, const int channel_index, const bool rebuild) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers properly
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

	if (get_data_margin_end() > 0) {
		if (bx == 0) {
			Ref<VoxelChunk> chunk = chunk_get_or_create(x - 1, y, z);
			chunk->set_voxel(data, get_chunk_size_x(), by, bz, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (by == 0) {
			Ref<VoxelChunk> chunk = chunk_get_or_create(x, y - 1, z);
			chunk->set_voxel(data, bx, get_chunk_size_y(), bz, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (bz == 0) {
			Ref<VoxelChunk> chunk = chunk_get_or_create(x, y, z - 1);
			chunk->set_voxel(data, bx, by, get_chunk_size_z(), channel_index);

			if (rebuild)
				chunk->build();
		}
	}

	if (get_data_margin_start() > 0) {
		if (bx == get_chunk_size_x() - 1) {
			Ref<VoxelChunk> chunk = chunk_get_or_create(x + 1, y, z);
			chunk->set_voxel(data, -1, by, bz, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (by == get_chunk_size_y() - 1) {
			Ref<VoxelChunk> chunk = chunk_get_or_create(x, y + 1, z);
			chunk->set_voxel(data, bx, -1, bz, channel_index);

			if (rebuild)
				chunk->build();
		}

		if (bz == get_chunk_size_z() - 1) {
			Ref<VoxelChunk> chunk = chunk_get_or_create(x, y, z + 1);
			chunk->set_voxel(data, bx, by, -1, channel_index);

			if (rebuild)
				chunk->build();
		}
	}

	Ref<VoxelChunk> chunk = chunk_get_or_create(x, y, z);
	chunk->set_voxel(data, bx, by, bz, channel_index);

	if (rebuild)
		chunk->build();
}

Ref<VoxelChunk> VoxelWorld::get_chunk_at_world_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return chunk_get(x, y, z);
}
Ref<VoxelChunk> VoxelWorld::get_or_create_chunk_at_world_position(const Vector3 &world_position) {
	Vector3 pos = world_position / get_voxel_scale();

	//Note: floor is needed to handle negative numbers proiberly
	int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
	int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));
	int z = static_cast<int>(Math::floor(pos.z / get_chunk_size_z()));

	return chunk_get_or_create(x, y, z);
}

void VoxelWorld::set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel) {
#if VERSION_MAJOR < 4
	call("_set_voxel_with_tool", mode_add, hit_position, hit_normal, selected_voxel, isolevel);
#else
	GDVIRTUAL_CALL(_set_voxel_with_tool, mode_add, hit_position, hit_normal, selected_voxel, isolevel);
#endif
}

int VoxelWorld::get_channel_index_info(const VoxelWorld::ChannelTypeInfo channel_type) {
#if VERSION_MAJOR < 4
	return call("_get_channel_index_info", channel_type);
#else
	int ret;
	if (GDVIRTUAL_CALL(_get_channel_index_info, channel_type, ret)) {
		return ret;
	}

	return 0;
#endif
}

VoxelWorld::VoxelWorld() {
	_editable = false;

	_is_priority_generation = true;

	_chunk_size_x = 16;
	_chunk_size_y = 16;
	_chunk_size_z = 16;
	_current_seed = 0;
	_data_margin_start = 0;
	_data_margin_end = 0;

	set_use_threads(true);
	set_max_concurrent_generations(3);

	_voxel_scale = 1;
	_chunk_spawn_range = 4;

	_player = NULL;
	_max_frame_chunk_build_steps = 0;
	_num_frame_chunk_build_steps = 0;
}

VoxelWorld ::~VoxelWorld() {
	_chunks.clear();
	_chunks_vector.clear();
	_world_areas.clear();
	_voxel_structures.clear();

	_library.unref();
	_level_generator.unref();

	_player = NULL;

	_generation_queue.clear();
	_generating.clear();

	_lights.clear();
}

void VoxelWorld::_generate_chunk(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (_level_generator.is_valid())
		_level_generator->generate_chunk(chunk);

	for (int i = 0; i < _voxel_structures.size(); ++i) {
		Ref<VoxelStructure> structure = _voxel_structures.get(i);

		if (!structure.is_valid())
			continue;

		if (structure->get_use_aabb()) {
			if (structure->get_chunk_aabb().has_point(Vector3(chunk->get_position_x(), chunk->get_position_y(), chunk->get_position_z())))
				structure->write_to_chunk(chunk);
		} else {
			structure->write_to_chunk(chunk);
		}
	}
}

void VoxelWorld::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_player_bind(get_node_or_null(get_player_path()));

			set_process_internal(true);
			set_physics_process_internal(true);
			set_notify_transform(true);

			if (_library.is_valid())
				_library->refresh_rects();

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk_setup(chunk);

					chunk->build();
				}
			}
		} break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			_num_frame_chunk_build_steps = 0;

			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->process(get_process_delta_time());
				}

				if (chunk->get_is_generating()) {
					chunk->generation_process(get_process_delta_time());
				}
			}

#if VERSION_MAJOR > 3
			if (_is_priority_generation && _generation_queue.is_empty() && _generating.is_empty()) {
#else
			if (_is_priority_generation && _generation_queue.empty() && _generating.empty()) {
#endif
				_is_priority_generation = false;

#if VERSION_MAJOR < 4
				call("_generation_finished");
#else
				GDVIRTUAL_CALL(_generation_finished);
#endif

				emit_signal("generation_finished");

				return;
			}

			for (int i = 0; i < _generating.size(); ++i) {
				Ref<VoxelChunk> chunk = _generating.get(i);

				if (!chunk.is_valid() || !chunk->get_is_generating()) {
#if VERSION_MAJOR < 4
					_generating.remove(i);
#else
					_generating.remove_at(i);
#endif
					--i;
					continue;
				}
			}

			if (_generating.size() >= _max_concurrent_generations)
				return;

			if (_generation_queue.size() == 0)
				return;

			while (_generating.size() < _max_concurrent_generations && _generation_queue.size() != 0) {
				Ref<VoxelChunk> chunk = _generation_queue.get(0);

#if VERSION_MAJOR < 4
				_generation_queue.remove(0);
#else
				_generation_queue.remove_at(0);
#endif

				ERR_FAIL_COND(!chunk.is_valid());

				_generating.push_back(chunk);

				chunk_generate(chunk);
			}
		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				ERR_CONTINUE(!chunk.is_valid());

				if (chunk->get_process()) {
					chunk->physics_process(get_physics_process_delta_time());
				}

				if (chunk->get_is_generating()) {
					chunk->generation_physics_process(get_physics_process_delta_time());
				}
			}

		} break;
		case NOTIFICATION_EXIT_TREE: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					if (chunk->get_voxel_world() == this) {
						chunk->exit_tree();
						chunk->set_voxel_world(NULL);
					}
				}
			}

		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			for (int i = 0; i < _chunks_vector.size(); ++i) {
				Ref<VoxelChunk> chunk = _chunks_vector[i];

				if (chunk.is_valid()) {
					chunk->world_transform_changed();
				}
			}

		} break;
	}
}

void VoxelWorld::_bind_methods() {
	ADD_SIGNAL(MethodInfo("chunk_mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_editable"), &VoxelWorld::get_editable);
	ClassDB::bind_method(D_METHOD("set_editable", "value"), &VoxelWorld::set_editable);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "editable"), "set_editable", "get_editable");

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelWorld::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelWorld::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelWorld::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelWorld::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelWorld::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelWorld::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_data_margin_start"), &VoxelWorld::get_data_margin_start);
	ClassDB::bind_method(D_METHOD("set_data_margin_start", "value"), &VoxelWorld::set_data_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_start"), "set_data_margin_start", "get_data_margin_start");

	ClassDB::bind_method(D_METHOD("get_data_margin_end"), &VoxelWorld::get_data_margin_end);
	ClassDB::bind_method(D_METHOD("set_data_margin_end", "value"), &VoxelWorld::set_data_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_margin_end"), "set_data_margin_end", "get_data_margin_end");

	ClassDB::bind_method(D_METHOD("get_current_seed"), &VoxelWorld::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &VoxelWorld::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

	ClassDB::bind_method(D_METHOD("get_use_threads"), &VoxelWorld::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "value"), &VoxelWorld::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("get_max_concurrent_generations"), &VoxelWorld::get_max_concurrent_generations);
	ClassDB::bind_method(D_METHOD("set_max_concurrent_generations", "value"), &VoxelWorld::set_max_concurrent_generations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_concurrent_generations"), "set_max_concurrent_generations", "get_max_concurrent_generations");

	ClassDB::bind_method(D_METHOD("get_max_frame_chunk_build_steps"), &VoxelWorld::get_max_frame_chunk_build_steps);
	ClassDB::bind_method(D_METHOD("set_max_frame_chunk_build_steps", "value"), &VoxelWorld::set_max_frame_chunk_build_steps);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_frame_chunk_build_steps"), "set_max_frame_chunk_build_steps", "get_max_frame_chunk_build_steps");

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelWorld::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "library"), &VoxelWorld::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_level_generator"), &VoxelWorld::get_level_generator);
	ClassDB::bind_method(D_METHOD("set_level_generator", "level_generator"), &VoxelWorld::set_level_generator);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "level_generator", PROPERTY_HINT_RESOURCE_TYPE, "VoxelLevelGenerator"), "set_level_generator", "get_level_generator");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelWorld::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelWorld::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_chunk_spawn_range"), &VoxelWorld::get_chunk_spawn_range);
	ClassDB::bind_method(D_METHOD("set_chunk_spawn_range", "value"), &VoxelWorld::set_chunk_spawn_range);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_spawn_range"), "set_chunk_spawn_range", "get_chunk_spawn_range");

	ClassDB::bind_method(D_METHOD("get_player_path"), &VoxelWorld::get_player_path);
	ClassDB::bind_method(D_METHOD("set_player_path", "value"), &VoxelWorld::set_player_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path"), "set_player_path", "get_player_path");

	ClassDB::bind_method(D_METHOD("get_player"), &VoxelWorld::get_player);
	ClassDB::bind_method(D_METHOD("set_player", "player"), &VoxelWorld::set_player_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_RESOURCE_TYPE, "Spatial", 0), "set_player", "get_player");

	ClassDB::bind_method(D_METHOD("world_area_get", "index"), &VoxelWorld::world_area_get);
	ClassDB::bind_method(D_METHOD("world_area_add", "area"), &VoxelWorld::world_area_add);
	ClassDB::bind_method(D_METHOD("world_area_remove", "index"), &VoxelWorld::world_area_remove);
	ClassDB::bind_method(D_METHOD("world_areas_clear"), &VoxelWorld::world_areas_clear);
	ClassDB::bind_method(D_METHOD("world_area_get_count"), &VoxelWorld::world_area_get_count);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &VoxelWorld::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &VoxelWorld::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &VoxelWorld::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &VoxelWorld::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structures_clear"), &VoxelWorld::voxel_structures_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &VoxelWorld::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &VoxelWorld::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &VoxelWorld::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &VoxelWorld::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "17/17:VoxelStructure", PROPERTY_USAGE_DEFAULT, "VoxelStructure"), "voxel_structures_set", "voxel_structures_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
#else
	GDVIRTUAL_BIND(_chunk_added, "chunk");
#endif

	ClassDB::bind_method(D_METHOD("chunk_add", "chunk", "x", "y", "z"), &VoxelWorld::chunk_add);
	ClassDB::bind_method(D_METHOD("chunk_has", "x", "y", "z"), &VoxelWorld::chunk_has);
	ClassDB::bind_method(D_METHOD("chunk_get", "x", "y", "z"), &VoxelWorld::chunk_get);
	ClassDB::bind_method(D_METHOD("chunk_remove", "x", "y", "z"), &VoxelWorld::chunk_remove);
	ClassDB::bind_method(D_METHOD("chunk_remove_index", "index"), &VoxelWorld::chunk_remove_index);

	ClassDB::bind_method(D_METHOD("chunk_get_index", "index"), &VoxelWorld::chunk_get_index);
	ClassDB::bind_method(D_METHOD("chunk_get_count"), &VoxelWorld::chunk_get_count);

	ClassDB::bind_method(D_METHOD("chunks_clear"), &VoxelWorld::chunks_clear);

	ClassDB::bind_method(D_METHOD("chunks_get"), &VoxelWorld::chunks_get);
	ClassDB::bind_method(D_METHOD("chunks_set"), &VoxelWorld::chunks_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "chunks", PROPERTY_HINT_NONE, "17/17:VoxelChunk", PROPERTY_USAGE_DEFAULT, "VoxelChunk"), "chunks_set", "chunks_get");

	ClassDB::bind_method(D_METHOD("generation_queue_add_to", "chunk"), &VoxelWorld::generation_queue_add_to);
	ClassDB::bind_method(D_METHOD("generation_queue_get_index", "index"), &VoxelWorld::generation_queue_get_index);
	ClassDB::bind_method(D_METHOD("generation_queue_remove_index", "index"), &VoxelWorld::generation_queue_remove_index);
	ClassDB::bind_method(D_METHOD("generation_queue_get_size"), &VoxelWorld::generation_queue_get_size);

	ClassDB::bind_method(D_METHOD("generation_add_to", "chunk"), &VoxelWorld::generation_add_to);
	ClassDB::bind_method(D_METHOD("generation_get_index", "index"), &VoxelWorld::generation_get_index);
	ClassDB::bind_method(D_METHOD("generation_remove_index", "index"), &VoxelWorld::generation_remove_index);
	ClassDB::bind_method(D_METHOD("generation_get_size"), &VoxelWorld::generation_get_size);

	ADD_SIGNAL(MethodInfo("generation_finished"));

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_generation_finished"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), "_create_chunk", PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "y"), PropertyInfo(Variant::INT, "z"), PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_prepare_chunk_for_generation", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
#else
	GDVIRTUAL_BIND(_generation_finished);
	GDVIRTUAL_BIND(_create_chunk, "chunk", "x", "y", "z", "chunk", "ret");
	GDVIRTUAL_BIND(_prepare_chunk_for_generation, "chunk");
	GDVIRTUAL_BIND(_generate_chunk, "chunk");
#endif

	ClassDB::bind_method(D_METHOD("chunk_get_or_create", "x", "y", "z"), &VoxelWorld::chunk_get_or_create);
	ClassDB::bind_method(D_METHOD("chunk_create", "x", "y", "z"), &VoxelWorld::chunk_create);
	ClassDB::bind_method(D_METHOD("chunk_setup", "chunk"), &VoxelWorld::chunk_setup);

	ClassDB::bind_method(D_METHOD("_create_chunk", "x", "y", "z", "chunk"), &VoxelWorld::_create_chunk);
	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &VoxelWorld::_generate_chunk);

	ClassDB::bind_method(D_METHOD("can_chunk_do_build_step"), &VoxelWorld::can_chunk_do_build_step);
	ClassDB::bind_method(D_METHOD("is_position_walkable", "position"), &VoxelWorld::is_position_walkable);
	ClassDB::bind_method(D_METHOD("on_chunk_mesh_generation_finished", "chunk"), &VoxelWorld::on_chunk_mesh_generation_finished);

#if PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_add", "transform", "prop", "apply_voxel_scael"), &VoxelWorld::prop_add, DEFVAL(true));
#endif

	//Lights
	ClassDB::bind_method(D_METHOD("light_add", "light"), &VoxelWorld::light_add);
	ClassDB::bind_method(D_METHOD("light_get", "index"), &VoxelWorld::light_get);
	ClassDB::bind_method(D_METHOD("light_remove", "index"), &VoxelWorld::light_remove);
	ClassDB::bind_method(D_METHOD("light_get_count"), &VoxelWorld::light_get_count);
	ClassDB::bind_method(D_METHOD("lights_clear"), &VoxelWorld::lights_clear);

	ClassDB::bind_method(D_METHOD("lights_get"), &VoxelWorld::lights_get);
	ClassDB::bind_method(D_METHOD("lights_set", "chunks"), &VoxelWorld::lights_set);

	ClassDB::bind_method(D_METHOD("get_voxel_at_world_position", "world_position", "channel_index"), &VoxelWorld::get_voxel_at_world_position);
	ClassDB::bind_method(D_METHOD("set_voxel_at_world_position", "world_position", "data", "channel_index", "rebuild"), &VoxelWorld::set_voxel_at_world_position, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_chunk_at_world_position", "world_position"), &VoxelWorld::get_chunk_at_world_position);
	ClassDB::bind_method(D_METHOD("get_or_create_chunk_at_world_position", "world_position"), &VoxelWorld::get_or_create_chunk_at_world_position);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::INT, "ret"), "_get_channel_index_info", PropertyInfo(Variant::INT, "channel_type", PROPERTY_HINT_ENUM, BINDING_STRING_CHANNEL_TYPE_INFO)));
#else
	GDVIRTUAL_BIND(_get_channel_index_info, "channel_type", "ret");
#endif

	ClassDB::bind_method(D_METHOD("get_channel_index_info", "channel_type"), &VoxelWorld::get_channel_index_info);
	ClassDB::bind_method(D_METHOD("_get_channel_index_info", "channel_type"), &VoxelWorld::_get_channel_index_info);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_set_voxel_with_tool",
			PropertyInfo(Variant::BOOL, "mode_add"),
			PropertyInfo(Variant::VECTOR3, "hit_position"),
			PropertyInfo(Variant::VECTOR3, "hit_normal"),
			PropertyInfo(Variant::INT, "selected_voxel"),
			PropertyInfo(Variant::INT, "isolevel")));
#else
	GDVIRTUAL_BIND(_set_voxel_with_tool, "mode_add", "hit_position", "hit_normal", "selected_voxel", "isolevel");
#endif

	ClassDB::bind_method(D_METHOD("set_voxel_with_tool", "mode_add", "hit_position", "hit_normal", "selected_voxel", "isolevel"), &VoxelWorld::set_voxel_with_tool);
	ClassDB::bind_method(D_METHOD("_set_voxel_with_tool", "mode_add", "hit_position", "hit_normal", "selected_voxel", "isolevel"), &VoxelWorld::_set_voxel_with_tool);

	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_TYPE);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_ISOLEVEL);
	BIND_ENUM_CONSTANT(CHANNEL_TYPE_INFO_LIQUID_FLOW);
}
