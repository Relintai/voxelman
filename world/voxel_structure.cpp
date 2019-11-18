#include "voxel_structure.h"

int VoxelStructure::get_chunk_size_x() const {
	return _chunk_size_x;
}
void VoxelStructure::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int VoxelStructure::get_chunk_size_y() const {
	return _chunk_size_y;
}
void VoxelStructure::set_chunk_size_y(const int value) {
	_chunk_size_y = value;
}

int VoxelStructure::get_chunk_size_z() const {
	return _chunk_size_z;
}
void VoxelStructure::set_chunk_size_z(const int value) {
	_chunk_size_z = value;
}

int VoxelStructure::get_world_position_x() const {
	return _world_position_x;
}
void VoxelStructure::set_world_position_x(const int value) {
	_world_position_x = value;
}

int VoxelStructure::get_world_position_y() const {
	return _world_position_y;
}
void VoxelStructure::set_world_position_y(const int value) {
	_world_position_y = value;
}

int VoxelStructure::get_world_position_z() const {
	return _world_position_z;
}
void VoxelStructure::set_world_position_z(const int value) {
	_world_position_z = value;
}

VoxelChunk *VoxelStructure::get_chunk_voxel_pos(int x, int y, int z) {

	VoxelChunk *b = get_chunk(x / _chunk_size_x, y / _chunk_size_y, z / _chunk_size_z);
/*
	if (!b.is_valid()) {
		b.instance();

		add_chunk(b, x / _chunk_size_x, y / _chunk_size_y, z / _chunk_size_z);
	}
*/
	return b;
}

int VoxelStructure::get_voxel(int x, int y, int z, unsigned int channel_index) {
	VoxelChunk *b = get_chunk_voxel_pos(x, y, z);

	//ERR_FAIL_COND_V(!b.is_valid(), 0);

	return b->get_voxel(x, y, z, channel_index);
}
void VoxelStructure::set_voxel(int value, int x, int y, int z, unsigned int channel_index) {
	VoxelChunk *b = get_chunk_voxel_pos(x, y, z);

	//ERR_FAIL_COND(!b.is_valid());

	return b->set_voxel(value, x, y, z, channel_index);
}

void VoxelStructure::add_chunk(VoxelChunk *chunk, const int x, const int y, const int z) {
	//_chunks.set(Vector3i(x, y, z), chunk);

	//VoxelStructureChunk c;
	//c.chunk = chunk;
	//c.position = Vector3i(x, y, z);

	//_chunks_vector.push_back(c);
}
VoxelChunk *VoxelStructure::get_chunk(const int x, const int y, const int z) {
	//const VoxelChunk **chunk = _chunks.getptr(Vector3i(x, y, z));

	//return Ref<VoxelBuffer>(chunk);

	return NULL;
}
VoxelChunk *VoxelStructure::remove_chunk(const int x, const int y, const int z) {
	/*
	VoxelChunk **chunk = _chunks.getptr(Vector3i(x, y, z));

	VoxelChunk *c(chunk);

	if (c.is_valid()) {

		for (int i = 0; i < _chunks_vector.size(); ++i) {
			if (_chunks_vector.get(i).chunk == c) {
				_chunks_vector.remove(i);
				break;
			}
		}
	}

	return c;*/

	return NULL;
}

VoxelChunk *VoxelStructure::get_chunk_index(const int index) {
	//return _chunks_vector.get(index).chunk;
}
int VoxelStructure::get_chunk_count() const {
	return _chunks_vector.size();
}

void VoxelStructure::clear_chunks() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		//_chunks_vector.get(i).chunk.unref();
	}

	//_chunks_vector.clear();

	//_chunks.clear();
}

VoxelStructure::VoxelStructure() {
	//_chunk_size = Vector3i(16, 16, 16);
	//_world_position = Vector3i(0, 0, 0);
}

VoxelStructure::~VoxelStructure() {
	for (int i = 0; i < _chunks_vector.size(); ++i) {
		//_chunks_vector.get(i).chunk.unref();
	}

	_chunks.clear();
	_chunks_vector.clear();
}

void VoxelStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelStructure::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelStructure::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelStructure::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelStructure::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelStructure::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelStructure::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_world_position_x"), &VoxelStructure::get_world_position_x);
	ClassDB::bind_method(D_METHOD("set_world_position_x", "value"), &VoxelStructure::set_world_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_x"), "set_world_position_x", "get_world_position_x");

	ClassDB::bind_method(D_METHOD("get_world_position_y"), &VoxelStructure::get_world_position_y);
	ClassDB::bind_method(D_METHOD("set_world_position_y", "value"), &VoxelStructure::set_world_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_y"), "set_world_position_y", "get_world_position_y");

	ClassDB::bind_method(D_METHOD("get_world_position_z"), &VoxelStructure::get_world_position_z);
	ClassDB::bind_method(D_METHOD("set_world_position_z", "value"), &VoxelStructure::set_world_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_z"), "set_world_position_z", "get_world_position_z");

	ClassDB::bind_method(D_METHOD("get_chunk_voxel_pos", "x", "y", "z"), &VoxelStructure::get_chunk_voxel_pos);

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "z", "channel_index"), &VoxelStructure::get_voxel, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "y", "z", "channel_index"), &VoxelStructure::set_voxel, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_voxel_v", "value", "pos", "channel_index"), &VoxelStructure::set_voxel_v, DEFVAL(0));

	ClassDB::bind_method(D_METHOD("add_chunk", "chunk", "x", "y", "z"), &VoxelStructure::add_chunk);
	ClassDB::bind_method(D_METHOD("get_chunk", "x", "y", "z"), &VoxelStructure::get_chunk);
	ClassDB::bind_method(D_METHOD("remove_chunk", "x", "y", "z"), &VoxelStructure::remove_chunk);

	ClassDB::bind_method(D_METHOD("get_chunk_index", "index"), &VoxelStructure::get_chunk_index);
	ClassDB::bind_method(D_METHOD("get_chunk_count"), &VoxelStructure::get_chunk_count);

	ClassDB::bind_method(D_METHOD("clear_chunks"), &VoxelStructure::clear_chunks);
}
