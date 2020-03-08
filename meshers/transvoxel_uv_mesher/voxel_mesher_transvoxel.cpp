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

#include "voxel_mesher_transvoxel.h"

#include "../../world/voxel_chunk.h"
#include "core/array.h"
#include "core/dictionary.h"
#include "servers/visual_server.h"

int VoxelMesherTransvoxel::get_texture_scale() const {
	return _texture_scale;
}
void VoxelMesherTransvoxel::set_texture_scale(const int value) {
	_texture_scale = value;
}

int *VoxelMesherTransvoxel::get_voxel_type_array(VoxelChunk *chunk, const int x, const int y, const int z, const int size) {
	int *arr = memnew_arr(int, 8);

	arr[0] = chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[1] = chunk->get_voxel(x, y + size, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[2] = chunk->get_voxel(x, y, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[3] = chunk->get_voxel(x, y + size, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[4] = chunk->get_voxel(x + size, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[5] = chunk->get_voxel(x + size, y + size, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[6] = chunk->get_voxel(x + size, y, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);
	arr[7] = chunk->get_voxel(x + size, y + size, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	return arr;
}
int VoxelMesherTransvoxel::get_case_code_from_arr(const int *data) {
	int case_code = 0;

	if (data[0] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_000;

	if (data[1] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_010;

	if (data[2] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_001;

	if (data[3] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_011;

	if (data[4] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_100;

	if (data[5] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_110;

	if (data[6] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_101;

	if (data[7] != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_111;

	return case_code;
}
int VoxelMesherTransvoxel::get_case_code(VoxelChunk *chunk, const int x, const int y, const int z, const int size) {
	int case_code = 0;

	if (chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_000;

	if (chunk->get_voxel(x, y + size, z, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_010;

	if (chunk->get_voxel(x, y, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_001;

	if (chunk->get_voxel(x, y + size, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_011;

	if (chunk->get_voxel(x + size, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_100;

	if (chunk->get_voxel(x + size, y + size, z, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_110;

	if (chunk->get_voxel(x + size, y, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_101;

	if (chunk->get_voxel(x + size, y + size, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE) != 0)
		case_code = case_code | VOXEL_ENTRY_MASK_111;

	return case_code;
}

int VoxelMesherTransvoxel::get_voxel_type(VoxelChunk *chunk, const int x, const int y, const int z, const int size) {
	int type = 0;

	type = chunk->get_voxel(x, y + size, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x, y, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x, y + size, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x + size, y + size, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x + size, y + size, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x + size, y, z, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	if (type != 0)
		return type;

	type = chunk->get_voxel(x + size, y, z + size, VoxelChunk::DEFAULT_CHANNEL_TYPE);

	return type;
}

void VoxelMesherTransvoxel::_add_chunk(Node *p_chunk) {
	VoxelChunk *chunk = Object::cast_to<VoxelChunk>(p_chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int y_size = chunk->get_size_y();
	int z_size = chunk->get_size_z();

	int lod_size = get_lod_size();

	for (int y = 0; y < y_size; y += lod_size) {
		for (int z = 0; z < z_size; z += lod_size) {
			for (int x = 0; x < x_size; x += lod_size) {

				int *type_arr = get_voxel_type_array(chunk, x, y, z, lod_size);
				int case_code = get_case_code_from_arr(type_arr);

				if (case_code == 0 or case_code == 255) {
					memdelete_arr(type_arr);
					continue;
				}

				int regular_cell_class = get_regular_cell_class(case_code);

				Ref<TransvoxelCellData> cell_data = get_regular_cell_data(regular_cell_class);

				int index_count = cell_data->get_triangle_count() * 3;
				int vertex_count = cell_data->get_vertex_count();

				for (int i = 0; i < index_count; ++i) {
					int ind = get_vertex_count() + cell_data->get_vertex_index(i);
					add_indices(ind);
				}

				Vector<Vector3> temp_verts;

				Dictionary carr;

				for (int i = 0; i < 8; ++i) {
					int t = type_arr[i];

					if (carr.has(t))
						carr[t] = static_cast<int>(carr[t]) + 1;
					else
						carr[t] = 1;
				}

				int type_id1 = -1;
				int type_id1c = -1;
				int type_id2 = -1;
				int type_id2c = -1;

				const Variant *K = NULL;
				while ((K = carr.next(K))) {
					int k = *K;

					if (k == 0)
						continue;

					int c = carr[k];

					if (type_id1c == -1) {
						type_id1 = k;
						type_id1c = c;
						continue;
					}

					if (c > type_id1c) {
						type_id1 = k;
						type_id1c = c;
					}
				}

				K = NULL;
				while ((K = carr.next(K))) {
					int k = *K;

					if (k == 0)
						continue;

					int c = carr[k];

					if (type_id2c == -1) {
						type_id2 = k;
						type_id2c = c;
						continue;
					}

					if (c > type_id2c && k != type_id1) {
						type_id2 = k;
						type_id2c = c;
					}
				}

				float surface_ratio = 1.0;

				if (type_id1 != type_id2)
					surface_ratio = static_cast<float>(type_id1c) / static_cast<float>(type_id2c) / 8.0;

				Ref<VoxelSurface> surface1 = _library->get_voxel_surface(type_id1);
				Ref<VoxelSurface> surface2 = _library->get_voxel_surface(type_id2);

				for (int i = 0; i < vertex_count; ++i) {
					int fv = get_regular_vertex_data_first_vertex(case_code, i);
					int sv = get_regular_vertex_data_second_vertex(case_code, i);

					Vector3 offs0 = corner_id_to_vertex(fv) * lod_size;
					Vector3 offs1 = corner_id_to_vertex(sv) * lod_size;

					int type = chunk->get_voxel(int(x + offs0.x), int(y + offs0.y), int(z + offs0.z), VoxelChunk::DEFAULT_CHANNEL_TYPE);

					int fill = 0;

					Vector3 vert_pos;
					Vector3 vert_dir;

					if (type == 0) {
						fill = chunk->get_voxel(int(x + offs1.x), int(y + offs1.y), int(z + offs1.z), VoxelChunk::DEFAULT_CHANNEL_ISOLEVEL);

						vert_pos = get_regular_vertex_second_position(case_code, i);
						vert_dir = get_regular_vertex_first_position(case_code, i);
					} else {
						fill = chunk->get_voxel(int(x + offs0.x), int(y + offs0.y), int(z + offs0.z), VoxelChunk::DEFAULT_CHANNEL_ISOLEVEL);

						vert_pos = get_regular_vertex_first_position(case_code, i);
						vert_dir = get_regular_vertex_second_position(case_code, i);
					}

					vert_dir = vert_dir - vert_pos;

					vert_pos += vert_dir * (fill / 256.0);

					temp_verts.push_back(vert_pos);
				}

				PoolVector<Vector3> temp_normals;
				temp_normals.resize(temp_verts.size());

				for (int i = 0; i < index_count; i += 3) {
					int indices[] = {
						cell_data->get_vertex_index(i),
						cell_data->get_vertex_index(i + 1),
						cell_data->get_vertex_index(i + 2)
					};

					Vector3 vertices[] = {
						temp_verts[indices[0]],
						temp_verts[indices[1]],
						temp_verts[indices[2]],
					};

					Vector3 i0 = temp_normals[indices[0]];
					Vector3 i1 = temp_normals[indices[1]];
					Vector3 i2 = temp_normals[indices[2]];

					Vector3 v0 = vertices[0];
					Vector3 v1 = vertices[1];
					Vector3 v2 = vertices[2];

					temp_normals.set(indices[0], i0 + (v1 - v0).cross(v0 - v2));
					temp_normals.set(indices[1], i1 + (v2 - v1).cross(v1 - v0));
					temp_normals.set(indices[2], i2 + (v2 - v1).cross(v2 - v0));
				}
				for (int i = 0; i < temp_verts.size(); ++i)
					temp_normals.set(i, temp_normals[i].normalized());

				PoolVector<Vector2> temp_uvs;
				temp_uvs.resize(temp_verts.size());

				PoolVector<Vector2> temp_uv2s;
				temp_uv2s.resize(temp_verts.size());

				for (int cvi = 0; cvi < temp_verts.size(); ++cvi) {
					Vector3 vertex = temp_verts[cvi];
					Vector3 normal = temp_normals[cvi];

					Vector3 s;
					Vector3 t;
					t.x = vertex.z;
					t.y = vertex.z;
					t.z = vertex.y;

					s.x = vertex.y;
					s.y = vertex.x;
					s.z = vertex.x;

					real_t bx = ABS(normal.x);
					real_t by = ABS(normal.y);
					real_t bz = ABS(normal.z);

					if ((bx + 0.0001 > by) && (bx + 0.0001 > bz)) {
						Vector2 uv(s.x, t.x);
						Rect2 umargin = get_uv_margin();
						uv.x *= umargin.size.x;
						uv.y *= umargin.size.y;

						uv.x += umargin.position.x;
						uv.y += umargin.position.y;

						temp_uvs.set(cvi, surface1->transform_uv_scaled(VoxelSurface::VOXEL_SIDE_SIDE, uv, x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()));
						temp_uv2s.set(cvi, surface2->transform_uv_scaled(VoxelSurface::VOXEL_SIDE_SIDE, uv, x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()));
					} else if ((bz + 0.0001 > bx) && (bz + 0.0001 > by)) {
						Vector2 uv(s.z, t.z);
						Rect2 umargin = get_uv_margin();
						uv.x *= umargin.size.x;
						uv.y *= umargin.size.y;

						uv.x += umargin.position.x;
						uv.y += umargin.position.y;

						temp_uvs.set(cvi, surface1->transform_uv_scaled(VoxelSurface::VOXEL_SIDE_SIDE, uv, x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()));
						temp_uv2s.set(cvi, surface2->transform_uv_scaled(VoxelSurface::VOXEL_SIDE_SIDE, uv, x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()));
					} else {
						Vector2 uv(s.y, t.y);
						Rect2 umargin = get_uv_margin();
						uv.x *= umargin.size.x;
						uv.y *= umargin.size.y;

						uv.x += umargin.position.x;
						uv.y += umargin.position.y;

						temp_uvs.set(cvi, surface1->transform_uv_scaled(VoxelSurface::VOXEL_SIDE_TOP, uv, x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()));
						temp_uv2s.set(cvi, surface2->transform_uv_scaled(VoxelSurface::VOXEL_SIDE_TOP, uv, x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()));
					}
				}

				for (int i = 0; i < temp_verts.size(); ++i) {
					Vector3 vert_pos = temp_verts[i];

					vert_pos *= float(lod_size);
					vert_pos += Vector3(x, y, z);

					Vector3 normal = temp_normals[i];

					add_color(Color(1.0, 1.0, 1.0, surface_ratio));
					vert_pos *= get_voxel_scale();

					add_normal(normal);
					add_uv(temp_uvs[i]);
					add_uv2(temp_uv2s[i]);
					add_vertex(vert_pos);
				}

				memdelete_arr(type_arr);
			}
		}
	}
}

Vector3 VoxelMesherTransvoxel::corner_id_to_vertex(int corner_id) const {
	ERR_FAIL_COND_V(corner_id < 0 || corner_id > 8, Vector3());

	return transvoxel_vertices[corner_id];
}

int VoxelMesherTransvoxel::get_regular_cell_class(int index) const {
	return static_cast<int>(regularCellClass[index]);
}

Ref<TransvoxelCellData> VoxelMesherTransvoxel::get_regular_cell_data(int index) const {
	return _regular_cell_datas[index];
}

int VoxelMesherTransvoxel::get_regular_vertex_data(int index1, int index2) const {
	//return static_cast<int>(regularVertexData[index1][index2]);
	return regularVertexData[index1][index2];
}

//void VoxelMesherTransvoxel::set_regular_vertex_data(int index1, int index2, int value) {
//   ERR_FAIL_INDEX(index1, 256);
//    ERR_FAIL_INDEX(index2, 13);

//    regularVertexData[index1][index2] = value;
//}

int VoxelMesherTransvoxel::get_regular_vertex_data_first_vertex(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;

	return vert1;
}

int VoxelMesherTransvoxel::get_regular_vertex_data_second_vertex(int index1, int index2) const {
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return vert2;
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_first_position(int index1, int index2) const {
	int vert = regularVertexData[index1][index2] & 0x000F;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_second_position(int index1, int index2) const {
	int vert = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_regular_vertex_direction(int index1, int index2) const {
	int vert1 = regularVertexData[index1][index2] & 0x000F;
	int vert2 = (regularVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert2] - transvoxel_vertices[vert1];
}

int VoxelMesherTransvoxel::get_transition_cell_class(int index) const {
	return static_cast<int>(transitionCellClass[index]);
}

Ref<TransvoxelCellData> VoxelMesherTransvoxel::get_transition_cell_data(int index) const {
	return _transition_cell_data[index];
}

int VoxelMesherTransvoxel::get_transition_corner_data(int index) const {
	return static_cast<int>(transitionCornerData[index]);
}

int VoxelMesherTransvoxel::get_transition_vertex_data(int index1, int index2) const {
	return static_cast<int>(transitionVertexData[index1][index2]);
}

int VoxelMesherTransvoxel::get_transition_vertex_data_first_vertex(int index1, int index2) const {
	unsigned short val = transitionVertexData[index1][index2];

	unsigned short vert = val & 0x000F;

	return static_cast<int>(vert);
}

int VoxelMesherTransvoxel::get_transition_vertex_data_second_vertex(int index1, int index2) const {
	unsigned short val = transitionVertexData[index1][index2];

	unsigned short vert = (val & 0x00F0) >> 4;

	return static_cast<int>(vert);
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_first_position(int index1, int index2) const {
	int vert = transitionVertexData[index1][index2] & 0x000F;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_second_position(int index1, int index2) const {
	int vert = (transitionVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert];
}

Vector3 VoxelMesherTransvoxel::get_transition_vertex_direction(int index1, int index2) const {
	int vert1 = transitionVertexData[index1][index2] & 0x000F;
	int vert2 = (transitionVertexData[index1][index2] & 0x00F0) >> 4;

	return transvoxel_vertices[vert2] - transvoxel_vertices[vert1];
}

VoxelMesherTransvoxel::VoxelMesherTransvoxel() {
	_format = VisualServer::ARRAY_FORMAT_NORMAL | VisualServer::ARRAY_FORMAT_COLOR | VisualServer::ARRAY_FORMAT_TEX_UV | VisualServer::ARRAY_FORMAT_TEX_UV2;

	_texture_scale = 4;

	for (int i = 0; i < 16; ++i) {
		_regular_cell_datas[i] = Ref<TransvoxelCellData>(memnew(TransvoxelCellData(regularCellData[i])));
	}

	for (int i = 0; i < 56; ++i) {
		_transition_cell_data[i] = Ref<TransvoxelCellData>(memnew(TransvoxelCellData(transitionCellData[i])));
	}
}

VoxelMesherTransvoxel::~VoxelMesherTransvoxel() {
}

void VoxelMesherTransvoxel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_scale"), &VoxelMesherTransvoxel::get_texture_scale);
	ClassDB::bind_method(D_METHOD("set_texture_scale", "value"), &VoxelMesherTransvoxel::set_texture_scale);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_scale"), "set_texture_scale", "get_texture_scale");

	ClassDB::bind_method(D_METHOD("_add_chunk", "chunk"), &VoxelMesherTransvoxel::_add_chunk);

	ClassDB::bind_method(D_METHOD("corner_id_to_vertex", "index1"), &VoxelMesherTransvoxel::corner_id_to_vertex);

	ClassDB::bind_method(D_METHOD("get_regular_cell_class", "index"), &VoxelMesherTransvoxel::get_regular_cell_class);
	ClassDB::bind_method(D_METHOD("get_regular_cell_data", "index"), &VoxelMesherTransvoxel::get_regular_cell_data);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data);
	//  ClassDB::bind_method(D_METHOD("set_regular_vertex_data", "index1", "index2", "value"), &VoxelMesherTransvoxel::set_regular_vertex_data);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data_first_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data_first_vertex);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_data_second_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_data_second_vertex);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_first_position", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_first_position);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_second_position", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_second_position);
	ClassDB::bind_method(D_METHOD("get_regular_vertex_direction", "index1", "index2"), &VoxelMesherTransvoxel::get_regular_vertex_direction);

	ClassDB::bind_method(D_METHOD("get_transition_cell_class", "index"), &VoxelMesherTransvoxel::get_transition_cell_class);
	ClassDB::bind_method(D_METHOD("get_transition_cell_data", "index"), &VoxelMesherTransvoxel::get_transition_cell_data);
	ClassDB::bind_method(D_METHOD("get_transition_corner_data", "index"), &VoxelMesherTransvoxel::get_transition_corner_data);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data_first_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data_first_vertex);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_data_second_vertex", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_data_second_vertex);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_first_position", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_first_position);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_second_position", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_second_position);
	ClassDB::bind_method(D_METHOD("get_transition_vertex_direction", "index1", "index2"), &VoxelMesherTransvoxel::get_transition_vertex_direction);

	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_000);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_100);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_010);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_110);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_001);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_101);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_011);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_INDEX_111);

	BIND_ENUM_CONSTANT(VOXEL_ENTRIES_SIZE);

	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_000);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_100);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_010);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_110);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_001);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_101);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_011);
	BIND_ENUM_CONSTANT(VOXEL_ENTRY_MASK_111);
}
