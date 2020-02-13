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

#include "voxel_mesher.h"

#include "../world/voxel_chunk.h"

Ref<VoxelmanLibrary> VoxelMesher::get_library() {
	return _library;
}
void VoxelMesher::set_library(Ref<VoxelmanLibrary> library) {
	_library = library;
}

Ref<Material> VoxelMesher::get_material() {
	return _material;
}
void VoxelMesher::set_material(const Ref<Material> &material) {
	_material = material;
}

float VoxelMesher::get_ao_strength() const {
	return _ao_strength;
}
void VoxelMesher::set_ao_strength(float value) {
	_ao_strength = value;
}

float VoxelMesher::get_base_light_value() const {
	return _base_light_value;
}
void VoxelMesher::set_base_light_value(float value) {
	_base_light_value = value;
}

float VoxelMesher::get_voxel_scale() const {
	return _voxel_scale;
}
void VoxelMesher::set_voxel_scale(const float voxel_scale) {
	_voxel_scale = voxel_scale;
}

int VoxelMesher::get_lod_size() const {
	return _lod_size;
}
void VoxelMesher::set_lod_size(const int lod_size) {
	_lod_size = lod_size;
}

Rect2 VoxelMesher::get_uv_margin() const {
	return _uv_margin;
}
void VoxelMesher::set_uv_margin(const Rect2 margin) {
	_uv_margin = margin;
}

void VoxelMesher::build_mesh(RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	VS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	_surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

	//if (_material.is_valid())
	//	_surface_tool->set_material(_material);

	if (_colors.size() != _vertices.size()) {
		print_error("Colors.size() != vertices.size() -> " + String::num(_colors.size()) + " " + String::num(_vertices.size()));

		_colors.resize(0);
	}

	int len = _vertices.size();

	for (int i = 0; i < len; ++i) {
		if (_normals.size() > 0) {
			_surface_tool->add_normal(_normals.get(i));
		}

		if (_colors.size() > 0) {
			_surface_tool->add_color(_colors.get(i));
		}

		if (_uvs.size() > 0) {
			_surface_tool->add_uv(_uvs.get(i));
		}

		if (_uv2s.size() > 0) {
			_surface_tool->add_uv2(_uv2s.get(i));
		}

		_surface_tool->add_vertex(_vertices.get(i));
	}

	for (int i = 0; i < _indices.size(); ++i) {
		_surface_tool->add_index(_indices.get(i));
	}

	if (_normals.size() == 0) {
		_surface_tool->generate_normals();
	}

	Array arr = _surface_tool->commit_to_arrays();

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh, VisualServer::PRIMITIVE_TRIANGLES, arr);

	if (_material.is_valid())
		VS::get_singleton()->mesh_surface_set_material(mesh, 0, _library->get_material()->get_rid());
}

void VoxelMesher::reset() {
	_vertices.resize(0);
	_normals.resize(0);
	_colors.resize(0);
	_uvs.resize(0);
	_uv2s.resize(0);
	_indices.resize(0);
	_bones.resize(0);

	_surface_tool->clear();
}

void VoxelMesher::add_chunk_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	add_chunk(vchunk);
}
void VoxelMesher::add_chunk(VoxelChunk *chunk) {
	ERR_FAIL_COND(!has_method("_add_chunk"));
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	call("_add_chunk", chunk);
}

void VoxelMesher::add_chunk_liquid_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	add_chunk_liquid(vchunk);
}
void VoxelMesher::add_chunk_liquid(VoxelChunk *chunk) {
	ERR_FAIL_COND(!has_method("_add_chunk_liquid"));
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	call("_add_chunk_liquid", chunk);
}

void VoxelMesher::add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position, const Vector3 rotation, const Vector3 scale, const Rect2 uv_rect) {
	Transform transform = Transform(Basis(rotation).scaled(scale), position);

	add_mesh_data_resource_transform(mesh, transform, uv_rect);
}

void VoxelMesher::add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform transform, const Rect2 uv_rect) {
	ERR_FAIL_COND(mesh->get_array().size() == 0);

	Array verts = mesh->get_array().get(Mesh::ARRAY_VERTEX);

	for (int i = 0; i < verts.size(); ++i) {
		Vector3 vert = verts[i];

		vert = transform.xform(vert);

		add_vertex(vert);
	}

	if (mesh->get_array().size() <= Mesh::ARRAY_NORMAL)
		return;

	Array normals = mesh->get_array().get(Mesh::ARRAY_NORMAL);

	for (int i = 0; i < normals.size(); ++i) {
		Vector3 normal = normals[i];

		normal = transform.basis.xform(normal);

		add_normal(normal);
	}

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_TANGENT)
		return;

	Array tangents = mesh->get_array().get(Mesh::ARRAY_TANGENT);

	for (int i = 0; i < verts.size(); ++i) {

		Plane p(tangents[i * 4 + 0], tangents[i * 4 + 1], tangents[i * 4 + 2], tangents[i * 4 + 3]);

		Vector3 tangent = p.normal;
		Vector3 binormal = p.normal.cross(tangent).normalized() * p.d;

		tangent = local_transform.basis.xform(tangent);
		binormal = local_transform.basis.xform(binormal);

		add_t(normal);
		add_binorm
	}*/

	if (mesh->get_array().size() <= Mesh::ARRAY_COLOR)
		return;

	Array colors = mesh->get_array().get(Mesh::ARRAY_COLOR);

	for (int i = 0; i < colors.size(); ++i) {
		Color color = colors[i];

		add_color(color);
	}

	if (mesh->get_array().size() <= Mesh::ARRAY_TEX_UV)
		return;

	Array tex_uv = mesh->get_array().get(Mesh::ARRAY_TEX_UV);

	for (int i = 0; i < tex_uv.size(); ++i) {
		Vector2 uv = tex_uv[i];

		uv.x *= uv_rect.size.x;
		uv.y *= uv_rect.size.y;

		uv.x += uv_rect.position.x;
		uv.y += uv_rect.position.y;

		add_uv(uv);
	}

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_TEX_UV2)
		return;

	Array tex_uv2 = mesh->get_array().get(Mesh::ARRAY_TEX_UV2);

	for (int i = 0; i < tex_uv.size(); ++i) {
		Vector2 uv = tex_uv2[i];

		add_uv2(uv);
	}*/

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_BONES)
		return;

	Array bones = mesh->get_array().get(Mesh::ARRAY_BONES);

	for (int i = 0; i < bones.size(); ++i) {
		int bone = bones[i];

		add_bone(bone);
	}*/

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_WEIGHTS)
		return;

	Array weights = mesh->get_array().get(Mesh::ARRAY_WEIGHTS);

	for (int i = 0; i < weights.size(); ++i) {
		float weight = weights[i];

		add_weight(weight);
	}*/

	if (mesh->get_array().size() <= Mesh::ARRAY_INDEX)
		return;

	Array indices = mesh->get_array().get(Mesh::ARRAY_INDEX);
	int ic = get_vertex_count() - verts.size();

	for (int i = 0; i < indices.size(); ++i) {
		int index = indices[i];

		add_indices(ic + index);
	}
}

void VoxelMesher::bake_colors_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	bake_colors(vchunk);
}
void VoxelMesher::bake_colors(VoxelChunk *chunk) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_bake_colors"))
		call("_bake_colors", chunk);
}
void VoxelMesher::_bake_colors(Node *p_chunk) {
	VoxelChunk *chunk = Object::cast_to<VoxelChunk>(p_chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	ERR_FAIL_COND(_vertices.size() != _normals.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vector3 vert = _vertices[i];

		if (vert.x < 0 || vert.y < 0 || vert.z < 0) {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}

			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _voxel_scale);
		unsigned int y = (unsigned int)(vert.y / _voxel_scale);
		unsigned int z = (unsigned int)(vert.z / _voxel_scale);

		if (chunk->validate_channel_data_position(x, y, z)) {
			Color light = Color(
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_AO) / 255.0) * _ao_strength;
			float rao = chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;

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

			if (_colors.size() < _vertices.size()) {
				_colors.push_back(light);
			} else {
				Color c = _colors[i];

				light.a = c.a;
				_colors.set(i, light);
			}
		} else {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}
		}
	}
}

void VoxelMesher::bake_liquid_colors_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	bake_liquid_colors(vchunk);
}
void VoxelMesher::bake_liquid_colors(VoxelChunk *chunk) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_bake_liquid_colors"))
		call("_bake_liquid_colors", chunk);
}
void VoxelMesher::_bake_liquid_colors(Node *p_chunk) {
	VoxelChunk *chunk = Object::cast_to<VoxelChunk>(p_chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	ERR_FAIL_COND(_vertices.size() != _normals.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vector3 vert = _vertices[i];

		if (vert.x < 0 || vert.y < 0 || vert.z < 0) {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}

			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _voxel_scale);
		unsigned int y = (unsigned int)(vert.y / _voxel_scale);
		unsigned int z = (unsigned int)(vert.z / _voxel_scale);

		if (chunk->validate_channel_data_position(x, y, z)) {
			Color light = Color(
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_AO) / 255.0) * _ao_strength;
			float rao = chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
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

			if (_colors.size() < _vertices.size()) {
				_colors.push_back(light);
			} else {
				_colors.set(i, light);
			}
		} else {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}
		}
	}
}

void VoxelMesher::build_collider(RID shape) const {
	ERR_FAIL_COND(shape == RID());

	if (_vertices.size() == 0)
		return;

	PoolVector<Vector3> face_points;

	if (_indices.size() == 0) {

		int len = (_vertices.size() / 4);

		for (int i = 0; i < len; ++i) {

			face_points.push_back(_vertices.get(i * 4));
			face_points.push_back(_vertices.get((i * 4) + 2));
			face_points.push_back(_vertices.get((i * 4) + 1));

			face_points.push_back(_vertices.get(i * 4));
			face_points.push_back(_vertices.get((i * 4) + 3));
			face_points.push_back(_vertices.get((i * 4) + 2));
		}

		PhysicsServer::get_singleton()->shape_set_data(shape, face_points);

		return;
	}

	face_points.resize(_indices.size());
	for (int i = 0; i < face_points.size(); i++) {
		face_points.set(i, _vertices.get(_indices.get(i)));
	}

	PhysicsServer::get_singleton()->shape_set_data(shape, face_points);
}

void VoxelMesher::bake_lights(MeshInstance *node, Vector<Ref<VoxelLight> > &lights) {
	ERR_FAIL_COND(node == NULL);

	Color darkColor(0, 0, 0, 1);

	for (int v = 0; v < _vertices.size(); ++v) {

		Vector3 vet = _vertices.get(v);
		Vector3 vertex = node->to_global(vet);

		//grab normal
		Vector3 normal = _normals.get(v);

		Vector3 v_lightDiffuse;

		//calculate the lights value
		for (int i = 0; i < lights.size(); ++i) {
			Ref<VoxelLight> light = lights.get(i);

			Vector3 lightDir = light->get_world_position() - vertex;

			float dist2 = lightDir.dot(lightDir);
			//inverse sqrt
			lightDir *= (1.0 / sqrt(dist2));

			float NdotL = normal.dot(lightDir);

			if (NdotL > 1.0) {
				NdotL = 1.0;
			} else if (NdotL < 0.0) {
				NdotL = 0.0;
			}

			Color cc = light->get_color();
			Vector3 cv(cc.r, cc.g, cc.b);

			Vector3 value = cv * (NdotL / (1.0 + dist2));

			value *= light->get_size();
			v_lightDiffuse += value;

			/*
                    float dist2 = Mathf.Clamp(Vector3.Distance(transformedLights[i], vertices), 0f, 15f);
                    dist2 /= 35f;

                    Vector3 value = Vector3.one;
                    value *= ((float) lights[i].Strength) / 255f;
                    value *= (1 - dist2);
                    v_lightDiffuse += value;*/
		}

		Color f = _colors.get(v);
		//Color f = darkColor;

		Vector3 cv2(f.r, f.g, f.b);
		cv2 += v_lightDiffuse;

		if (cv2.x > 1)
			cv2.x = 1;

		if (cv2.y > 1)
			cv2.y = 1;

		if (cv2.y > 1)
			cv2.y = 1;

		// cv2.x = Mathf.Clamp(cv2.x, 0f, 1f);
		//cv2.y = Mathf.Clamp(cv2.y, 0f, 1f);
		// cv2.z = Mathf.Clamp(cv2.z, 0f, 1f);

		f.r = cv2.x;
		f.g = cv2.y;
		f.b = cv2.z;

		//f.r = v_lightDiffuse.x;
		//f.g = v_lightDiffuse.y;
		//f.b = v_lightDiffuse.z;

		_colors.set(v, f);
	}

	//	for (int i = 0; i < _colors->size(); ++i) {
	//		print_error(_colors->get(i));
	//	}
}

PoolVector<Vector3> VoxelMesher::get_vertices() {
	return _vertices;
}

void VoxelMesher::set_vertices(const PoolVector<Vector3> &values) {
	_vertices = values;
}

int VoxelMesher::get_vertex_count() {
	return _vertices.size();
}

void VoxelMesher::add_vertex(Vector3 vertex) {
	_vertices.push_back(vertex);
}

Vector3 VoxelMesher::get_vertex(int idx) {
	return _vertices.get(idx);
}

void VoxelMesher::remove_vertex(int idx) {
	_vertices.remove(idx);
}

PoolVector<Vector3> VoxelMesher::get_normals() {
	return _normals;
}

void VoxelMesher::set_normals(const PoolVector<Vector3> &values) {
	_normals = values;
}

int VoxelMesher::get_normal_count() {
	return _normals.size();
}

void VoxelMesher::add_normal(Vector3 normal) {
	_normals.push_back(normal);
}

Vector3 VoxelMesher::get_normal(int idx) {
	return _normals.get(idx);
}

void VoxelMesher::remove_normal(int idx) {
	_normals.remove(idx);
}

PoolVector<Color> VoxelMesher::get_colors() {
	return _colors;
}

void VoxelMesher::set_colors(const PoolVector<Color> &values) {
	_colors = values;
}

int VoxelMesher::get_color_count() {
	return _colors.size();
}

void VoxelMesher::add_color(Color color) {
	_colors.push_back(color);
}

Color VoxelMesher::get_color(int idx) {
	return _colors.get(idx);
}

void VoxelMesher::remove_color(int idx) {
	_colors.remove(idx);
}

PoolVector<Vector2> VoxelMesher::get_uvs() {
	return _uvs;
}

void VoxelMesher::set_uvs(const PoolVector<Vector2> &values) {
	_uvs = values;
}

int VoxelMesher::get_uv_count() {
	return _uvs.size();
}

void VoxelMesher::add_uv(Vector2 uv) {
	_uvs.push_back(uv);
}

Vector2 VoxelMesher::get_uv(int idx) {
	return _uvs.get(idx);
}

void VoxelMesher::remove_uv(int idx) {
	_uvs.remove(idx);
}

PoolVector<Vector2> VoxelMesher::get_uv2s() {
	return _uv2s;
}

void VoxelMesher::set_uv2s(const PoolVector<Vector2> &values) {
	_uv2s = values;
}

int VoxelMesher::get_uv2_count() {
	return _uv2s.size();
}

void VoxelMesher::add_uv2(Vector2 uv) {
	_uv2s.push_back(uv);
}

Vector2 VoxelMesher::get_uv2(int idx) {
	return _uv2s.get(idx);
}

void VoxelMesher::remove_uv2(int idx) {
	_uv2s.remove(idx);
}

PoolVector<int> VoxelMesher::get_indices() {
	return _indices;
}

void VoxelMesher::set_indices(const PoolVector<int> values) {
	_indices = values;
}

int VoxelMesher::get_indices_count() {
	return _indices.size();
}

void VoxelMesher::add_indices(int index) {
	_indices.push_back(index);
}

int VoxelMesher::get_indice(int idx) {
	return _indices.get(idx);
}

void VoxelMesher::remove_indices(int idx) {
	_indices.remove(idx);
}

VoxelMesher::VoxelMesher(Ref<VoxelmanLibrary> library) {
	_library = library;

	_voxel_scale = 1;
	_lod_size = 1;

	_surface_tool.instance();
}

VoxelMesher::VoxelMesher() {

	_voxel_scale = 1;
	_lod_size = 1;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);

	_surface_tool.instance();
}

VoxelMesher::~VoxelMesher() {
	_surface_tool.unref();

	if (_library.is_valid()) {
		_library.unref();
	}
}

void VoxelMesher::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_add_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_add_chunk_liquid", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_bake_colors", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_bake_liquid_colors", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelMesher::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelMesher::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_material"), &VoxelMesher::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &VoxelMesher::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelMesher::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelMesher::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_lod_size"), &VoxelMesher::get_lod_size);
	ClassDB::bind_method(D_METHOD("set_lod_size", "value"), &VoxelMesher::set_lod_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_size"), "set_lod_size", "get_lod_size");

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &VoxelMesher::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &VoxelMesher::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &VoxelMesher::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &VoxelMesher::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");

	ClassDB::bind_method(D_METHOD("get_uv_margin"), &VoxelMesher::get_uv_margin);
	ClassDB::bind_method(D_METHOD("set_uv_margin", "value"), &VoxelMesher::set_uv_margin);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "uv_margin"), "set_uv_margin", "get_uv_margin");

	ClassDB::bind_method(D_METHOD("add_chunk", "chunk"), &VoxelMesher::add_chunk_bind);
	ClassDB::bind_method(D_METHOD("add_chunk_liquid", "chunk"), &VoxelMesher::add_chunk_liquid_bind);

	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "mesh", "position", "rotation", "scale", "uv_rect"), &VoxelMesher::add_mesh_data_resource, DEFVAL(Rect2(0, 0, 1, 1)), DEFVAL(Vector3(1.0, 1.0, 1.0)), DEFVAL(Vector3()), DEFVAL(Vector3()));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform", "mesh", "transform", "uv_rect"), &VoxelMesher::add_mesh_data_resource_transform, DEFVAL(Rect2(0, 0, 1, 1)));

	ClassDB::bind_method(D_METHOD("bake_colors", "chunk"), &VoxelMesher::bake_colors_bind);
	ClassDB::bind_method(D_METHOD("_bake_colors", "chunk"), &VoxelMesher::_bake_colors);

	ClassDB::bind_method(D_METHOD("bake_liquid_colors", "chunk"), &VoxelMesher::bake_liquid_colors_bind);
	ClassDB::bind_method(D_METHOD("_bake_liquid_colors", "chunk"), &VoxelMesher::_bake_liquid_colors);

	ClassDB::bind_method(D_METHOD("get_vertices"), &VoxelMesher::get_vertices);
	ClassDB::bind_method(D_METHOD("set_vertices", "values"), &VoxelMesher::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &VoxelMesher::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &VoxelMesher::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &VoxelMesher::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &VoxelMesher::add_vertex);

	ClassDB::bind_method(D_METHOD("get_normals"), &VoxelMesher::get_normals);
	ClassDB::bind_method(D_METHOD("set_normals", "values"), &VoxelMesher::set_normals);
	ClassDB::bind_method(D_METHOD("get_normal_count"), &VoxelMesher::get_normal_count);
	ClassDB::bind_method(D_METHOD("get_normal", "idx"), &VoxelMesher::get_normal);
	ClassDB::bind_method(D_METHOD("remove_normal", "idx"), &VoxelMesher::remove_normal);
	ClassDB::bind_method(D_METHOD("add_normal", "normal"), &VoxelMesher::add_normal);

	ClassDB::bind_method(D_METHOD("get_colors"), &VoxelMesher::get_colors);
	ClassDB::bind_method(D_METHOD("set_colors", "values"), &VoxelMesher::set_colors);
	ClassDB::bind_method(D_METHOD("get_color_count"), &VoxelMesher::get_color_count);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &VoxelMesher::get_color);
	ClassDB::bind_method(D_METHOD("remove_color", "idx"), &VoxelMesher::remove_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &VoxelMesher::add_color);

	ClassDB::bind_method(D_METHOD("get_uvs"), &VoxelMesher::get_uvs);
	ClassDB::bind_method(D_METHOD("set_uvs", "values"), &VoxelMesher::set_uvs);
	ClassDB::bind_method(D_METHOD("get_uv_count"), &VoxelMesher::get_uv_count);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &VoxelMesher::get_uv);
	ClassDB::bind_method(D_METHOD("remove_uv", "idx"), &VoxelMesher::remove_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "uv"), &VoxelMesher::add_uv);

	ClassDB::bind_method(D_METHOD("get_uv2s"), &VoxelMesher::get_uv2s);
	ClassDB::bind_method(D_METHOD("set_uv2s", "values"), &VoxelMesher::set_uv2s);
	ClassDB::bind_method(D_METHOD("get_uv2_count"), &VoxelMesher::get_uv2_count);
	ClassDB::bind_method(D_METHOD("get_uv2", "idx"), &VoxelMesher::get_uv2);
	ClassDB::bind_method(D_METHOD("remove_uv2", "idx"), &VoxelMesher::remove_uv2);
	ClassDB::bind_method(D_METHOD("add_uv2", "uv"), &VoxelMesher::add_uv2);

	ClassDB::bind_method(D_METHOD("get_indices"), &VoxelMesher::get_indices);
	ClassDB::bind_method(D_METHOD("set_indices", "values"), &VoxelMesher::set_indices);
	ClassDB::bind_method(D_METHOD("get_indices_count"), &VoxelMesher::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_indice", "idx"), &VoxelMesher::get_indice);
	ClassDB::bind_method(D_METHOD("remove_indices", "idx"), &VoxelMesher::remove_indices);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &VoxelMesher::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &VoxelMesher::reset);

	//ClassDB::bind_method(D_METHOD("calculate_vertex_ambient_occlusion", "meshinstance_path", "radius", "intensity", "sampleCount"), &VoxelMesher::calculate_vertex_ambient_occlusion_path);

	ClassDB::bind_method(D_METHOD("build_mesh", "mesh_rid"), &VoxelMesher::build_mesh);
}
