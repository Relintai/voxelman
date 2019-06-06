#include "voxel_mesher.h"

VoxelMesher::VoxelMesher(Ref<VoxelmanLibrary> library) {
	_library = library;

	_debug_voxel_face = false;
	size = (float)1;
	vertexOffset = Vector3((float)0.5, (float)0.5, (float)0.5);

	_surface_tool = memnew(SurfaceTool());
}

VoxelMesher::VoxelMesher() {
	_debug_voxel_face = false;
	size = (float)1;
	vertexOffset = Vector3((float)0.5, (float)0.5, (float)0.5);

	_surface_tool = memnew(SurfaceTool());
}

VoxelMesher::~VoxelMesher() {
	_vertices.clear();
	_normals.clear();
	_colors.clear();
	_uvs.clear();
	_indices.clear();
	_bones.clear();
	memdelete(_surface_tool);
}

Ref<ArrayMesh> VoxelMesher::build_mesh() {
	_surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	_surface_tool->set_material(_library->get_material());

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

		_surface_tool->add_vertex(_vertices.get(i));
	}

	for (int i = 0; i < _indices.size(); ++i) {
		_surface_tool->add_index(_indices.get(i));
	}

	_surface_tool->generate_normals();

	Ref<ArrayMesh> m = _surface_tool->commit();

	return m;
}

void VoxelMesher::reset() {
	_vertices.clear();
	_normals.clear();
	_colors.clear();
	_uvs.clear();
	_indices.clear();
	_bones.clear();
}

void VoxelMesher::create_mesh_for_marching_cubes_query(Ref<MarchingCubesVoxelQuery> query) {
	ERR_FAIL_COND(!query.is_valid());
	ERR_FAIL_COND(!has_method("_create_mesh_for_marching_cubes_query"));

	call("_create_mesh_for_marching_cubes_query", query);
}

void VoxelMesher::add_buffer(Ref<VoxelBuffer> voxels) {
	ERR_FAIL_COND(!has_method("_add_voxel"));
	
	call("_add_voxel", voxels);
}

void VoxelMesher::create_trimesh_shape(Ref<ConcavePolygonShape> shape) const {
	if (_vertices.size() == 0)
		return;

	PoolVector<Vector3> face_points;

	if (_indices.size() == 0) {

		//face_points.resize(_vertices.size());

		int len = (_vertices.size() / 4);

		for (int i = 0; i < len; ++i) {

			face_points.push_back(_vertices.get(i * 4));
			face_points.push_back(_vertices.get((i * 4) + 2));
			face_points.push_back(_vertices.get((i * 4) + 1));

			face_points.push_back(_vertices.get(i * 4));
			face_points.push_back(_vertices.get((i * 4) + 3));
			face_points.push_back(_vertices.get((i * 4) + 2));
		}

		shape->set_faces(face_points);

		return;
	}

	face_points.resize(_indices.size());
	for (int i = 0; i < face_points.size(); i++) {
		face_points.set(i, _vertices.get(_indices.get(i)));
	}

	shape->set_faces(face_points);
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

			value *= light->get_strength();
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

Vector<Vector3> *VoxelMesher::get_vertices() {
	return &_vertices;
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

Vector<Vector3> *VoxelMesher::get_normals() {
	return &_normals;
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

Vector<Color> *VoxelMesher::get_colors() {
	return &_colors;
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

Vector<Vector2> *VoxelMesher::get_uvs() {
	return &_uvs;
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


Vector<int> *VoxelMesher::get_indices() {
	return &_indices;
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


void VoxelMesher::_bind_methods() {
	//BIND_VMETHOD(MethodInfo("_build_mesh", PropertyInfo(Variant::BOOL, "recal", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_add_voxels", PropertyInfo(Variant::OBJECT, "buffer", PROPERTY_HINT_RESOURCE_TYPE, "VoxelBuffer")));
	BIND_VMETHOD(MethodInfo("_create_mesh_for_marching_cubes_query", PropertyInfo(Variant::OBJECT, "query", PROPERTY_HINT_RESOURCE_TYPE, "MarchingCubesVoxelQuery")));

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelMesher::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelMesher::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_vertex_count"), &VoxelMesher::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &VoxelMesher::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &VoxelMesher::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &VoxelMesher::add_vertex);

	ClassDB::bind_method(D_METHOD("get_normal_count"), &VoxelMesher::get_normal_count);
	ClassDB::bind_method(D_METHOD("get_normal", "idx"), &VoxelMesher::get_normal);
	ClassDB::bind_method(D_METHOD("remove_normal", "idx"), &VoxelMesher::remove_normal);
	ClassDB::bind_method(D_METHOD("add_normal", "normal"), &VoxelMesher::add_normal);

	ClassDB::bind_method(D_METHOD("get_color_count"), &VoxelMesher::get_color_count);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &VoxelMesher::get_color);
	ClassDB::bind_method(D_METHOD("remove_color", "idx"), &VoxelMesher::remove_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &VoxelMesher::add_color);

	ClassDB::bind_method(D_METHOD("get_uv_count"), &VoxelMesher::get_uv_count);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &VoxelMesher::get_uv);
	ClassDB::bind_method(D_METHOD("remove_uv", "idx"), &VoxelMesher::remove_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "vertex"), &VoxelMesher::add_uv);

	ClassDB::bind_method(D_METHOD("get_indices_count"), &VoxelMesher::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_indice", "idx"), &VoxelMesher::get_indice);
	ClassDB::bind_method(D_METHOD("remove_indices", "idx"), &VoxelMesher::remove_indices);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &VoxelMesher::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &VoxelMesher::reset);

	//ClassDB::bind_method(D_METHOD("calculate_vertex_ambient_occlusion", "meshinstance_path", "radius", "intensity", "sampleCount"), &VoxelMesher::calculate_vertex_ambient_occlusion_path);

	ClassDB::bind_method(D_METHOD("build_mesh"), &VoxelMesher::build_mesh);
}
