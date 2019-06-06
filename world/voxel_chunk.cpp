#include "voxel_chunk.h"

NodePath VoxelChunk::get_library_path() {
	return _library_path;
}
void VoxelChunk::set_library_path(NodePath value) {
	_library_path = value;
}

NodePath VoxelChunk::get_mesh_instance_path() {
	return _mesh_instance_path;
}
void VoxelChunk::set_mesh_instance_path(NodePath value) {
	_mesh_instance_path = value;
}

Ref<VoxelmanLibrary> VoxelChunk::get_library() {
	return _library;
}
void VoxelChunk::set_library(Ref<VoxelmanLibrary> value) {
	_library = value;
}

float VoxelChunk::get_voxel_scale() {
	return _voxel_scale;
}
void VoxelChunk::set_voxel_scale(float value) {
	_voxel_scale = value;
}

Ref<VoxelMesher> VoxelChunk::get_mesher() const {
	return _mesher;
}
void VoxelChunk::set_mesher(Ref<VoxelMesher> mesher) {
	_mesher = mesher;
}

bool VoxelChunk::get_build_mesh() {
	return _build_mesh;
}
void VoxelChunk::set_build_mesh(bool value) {
	_build_mesh = value;
}

bool VoxelChunk::get_create_collider() {
	return _create_collider;
}
void VoxelChunk::set_create_collider(bool value) {
	_create_collider = value;
}

bool VoxelChunk::get_bake_lights() {
	return _bake_lights;
}
void VoxelChunk::set_bake_lights(bool value) {
	_bake_lights = value;
}

bool VoxelChunk::get_bake_ambient_occlusion() {
	return _bake_ambient_occlusion;
}
void VoxelChunk::set_bake_ambient_occlusion(bool value) {
	_bake_ambient_occlusion = value;
}

float VoxelChunk::get_ao_radius() {
	return _ao_radius;
}
void VoxelChunk::set_ao_radius(float value) {
	_ao_radius = value;
}

float VoxelChunk::get_ao_intensity() {
	return _ao_intensity;
}
void VoxelChunk::set_ao_intensity(float value) {
	_ao_intensity = value;
}

int VoxelChunk::get_ao_sample_count() {
	return _ao_sample_count;
}
void VoxelChunk::set_ao_sample_count(int value) {
	_ao_sample_count = value;
}

NodePath VoxelChunk::get_debug_drawer_path() {
	return _debug_drawer_path;
}
void VoxelChunk::set_debug_drawer_path(NodePath value) {
	_debug_drawer_path = value;
}

Ref<VoxelBuffer> VoxelChunk::get_buffer() const {
	return _buffer;
}

void VoxelChunk::clear() {
	_voxel_lights.clear();
}

void VoxelChunk::build() {
	ERR_FAIL_COND(!_library.is_valid());
	ERR_FAIL_COND(!_mesher.is_valid());

	_mesher->set_library(_library);

	if (_debug_drawer == NULL) {
		Node *n = get_node(_debug_drawer_path);

		if (n != NULL) {
			_debug_drawer = Object::cast_to<ImmediateGeometry>(n);
		}
	}

	_mesher->add_buffer(_buffer);

	if (get_build_mesh()) {
		ERR_FAIL_COND(!has_method("_build_mesh"));

		call("_build_mesh");

		finalize_mesh();
	}

	if (get_create_collider()) {
		update_collider();
	}
}

void VoxelChunk::finalize_mesh() {
	_mesher->set_library(_library);

	Node *node = get_node(_mesh_instance_path);

	if (node != NULL) {

		_mesh_instance = Object::cast_to<MeshInstance>(node);

		if (_mesh_instance != NULL) {
			//if (get_bake_ambient_occlusion()) {
			//	set_enabled(true);
			//} else {
				Ref<ArrayMesh> mesh = get_mesher()->build_mesh();

				_mesh_instance->set_mesh(mesh);
			//}
		}
	}
}

void VoxelChunk::update_collider() {
	//_mesh_instance->create_trimesh_collision();
	//StaticBody *static_body = Object::cast_to<StaticBody>(create_trimesh_collision_node());

	StaticBody *static_body = create_trimesh_collision_node();
	ERR_FAIL_COND(!static_body);
	static_body->set_name(String(get_name()) + "_col");

	add_child(static_body);
	if (get_owner()) {
		CollisionShape *cshape = Object::cast_to<CollisionShape>(static_body->get_child(0));
		static_body->set_owner(get_owner());
		cshape->set_owner(get_owner());
	}
}

StaticBody *VoxelChunk::create_trimesh_collision_node() {

	Ref<ConcavePolygonShape> shape = memnew(ConcavePolygonShape);

	_mesher->create_trimesh_shape(shape);

	StaticBody *static_body = memnew(StaticBody);
	CollisionShape *cshape = memnew(CollisionShape);
	cshape->set_shape(shape);
	static_body->add_child(cshape);
	return static_body;
}

void VoxelChunk::query_marching_cubes_data(Ref<MarchingCubesVoxelQuery> query) {
	ERR_FAIL_COND(!query.is_valid());

	Vector3i position = query->get_position();
	int size = query->get_size();
	/*
	query->set_entries(Ref<Voxel>(_VoxelChunk->getptr(position)), Ref<Voxel>(_VoxelChunk->getptr(Vector3i(size, 0, 0) + position)),
			Ref<Voxel>(_VoxelChunk->getptr(Vector3i(0, size, 0) + position)), Ref<Voxel>(_VoxelChunk->getptr(Vector3i(size, size, 0) + position)),
			Ref<Voxel>(_VoxelChunk->getptr(Vector3i(0, 0, size) + position)), Ref<Voxel>(_VoxelChunk->getptr(Vector3i(size, 0, size) + position)),
			Ref<Voxel>(_VoxelChunk->getptr(Vector3i(0, size, size) + position)), Ref<Voxel>(_VoxelChunk->getptr(Vector3i(size, size, size) + position)));*/
}

void VoxelChunk::create_mesh_for_marching_cubes_query(Ref<MarchingCubesVoxelQuery> query) {
	ERR_FAIL_COND(!query.is_valid());
	ERR_FAIL_COND(!_mesher.is_valid());

	_mesher->create_mesh_for_marching_cubes_query(query);
}

void VoxelChunk::set_enabled(bool p_enabled) {

	_enabled = p_enabled;

	if (is_inside_tree())
		set_physics_process_internal(p_enabled);
}

bool VoxelChunk::is_enabled() const {
	return _enabled;
}

void VoxelChunk::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_ENTER_TREE: {
			if (!_mesher.is_valid()) {
				if (has_method("_create_mesher")) {
					call("_create_mesher");

					if (!Engine::get_singleton()->is_editor_hint()) {
						ERR_FAIL_COND(!_mesher.is_valid());
					}
				} else {
					_mesher = Ref<VoxelMesher>(memnew(VoxelMesher()));
				}
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {

			//if (_mesher != NULL) {
			//	memdelete(_mesher);
			//}

		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {

			//if (!_enabled)
			//	break;

			//if (_mesh_instance != NULL) {
			//	if (get_bake_ambient_occlusion()) {
					//get_mesher()->calculate_vertex_ambient_occlusion(_mesh_instance, get_ao_radius(), get_ao_intensity(), get_ao_sample_count());
			//	}

				//Ref<ArrayMesh> mesh = get_mesher()->build_mesh();

				//_mesh_instance->set_mesh(mesh);
			//}

			//set_enabled(false);

		} break;
	}
}

void VoxelChunk::add_voxel_light_bind(Vector3 position, Color color, float strength) {
	add_voxel_light(position, color, strength);
}

Ref<VoxelLight> VoxelChunk::add_voxel_light(Vector3i position, Color color, float strength, Vector3 offset) {
	Vector3 pos(position.x, position.y, position.z);

	Ref<VoxelLight> light = Ref<VoxelLight>(memnew(VoxelLight(position, color, strength, to_global(pos + Vector3((float)0.5, (float)0.5, (float)0.5) * _voxel_scale), offset)));

	_voxel_lights.push_back(light);

	return light;
}

void VoxelChunk::draw_cross_voxels(Vector3 pos) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.2));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.2));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.2, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.2, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.2, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.2, 0, 0));
}

void VoxelChunk::draw_cross_voxels(Vector3 pos, float fill) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.5 * fill));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.5 * fill));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.5 * fill, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.5 * fill, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.5 * fill, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.5 * fill, 0, 0));
}

void VoxelChunk::draw_debug_voxels(int max, Color color) {
	/*
	if (_debug_drawer == NULL) {
		Node *n = get_node(_debug_drawer_path);

		if (n != NULL) {
			_debug_drawer = Object::cast_to<ImmediateGeometry>(n);
		}
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	//if (_debug_drawer->emt)

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PRIMITIVE_LINES);
	_debug_drawer->set_color(color);

	HashMap<int, Ref<Voxel> > *map = _voxels->get_map();

	ERR_FAIL_COND(map == NULL);

	const int *k = NULL;

	//Vector3 pos = get_transform().get_origin();

	int a = 0;
	while ((k = map->next(k))) {
		Ref<Voxel> v = map->get(*k);
		ERR_FAIL_COND(!v.is_valid());

		Vector3i lp = v->get_local_position();
		//print_error(Vector3(lp.x, lp.y, lp.z));
		draw_cross_voxels(Vector3(lp.x + 0.5, lp.y + 0.5, lp.z + 0.5), v->get_fill() / 255.0);

		a++;

		if (a > max) {
			break;
		}
	}

	//for (int x = 0; x < 30; x++) {
	//	for (int y = 0; y < 30; y++) {
	//		for (int z = 0; z < 30; z++) {
	//draw_cross_voxels(Vector3(x, y, z));
	//
	//		}
	//	}
	//}
	//_debug_drawer->add_sphere(4, 4, 0.5, true);

	_debug_drawer->end();*/
}

void VoxelChunk::draw_debug_voxel_lights(int max, bool localPosition) {
	/*
	if (_debug_drawer == NULL) {
		Node *n = get_node(_debug_drawer_path);

		if (n != NULL) {
			_debug_drawer = Object::cast_to<ImmediateGeometry>(n);
		}
	}

	if (_debug_drawer == NULL) {
		return;
	}

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);
	_debug_drawer->set_color(Color(1, 1, 1));

	const int *k = NULL;

	Vector3 pos = get_transform().get_origin();

	int a = 0;
	while ((k = _voxel_lights->next(k))) {
		Ref<VoxelLight> v = _voxel_lights->get(*k);

		if (localPosition) {
			Vector3i lp = v->get_local_position();
			draw_cross_voxels(pos + Vector3(lp.x, lp.y, lp.z), (float)v->get_strength() / (float)10);
		} else {
			Vector3i wp = v->get_world_position();
			draw_cross_voxels(pos + Vector3(wp.x, wp.y, wp.z), (float)v->get_strength() / (float)10);
		}
		++a;

		if (a > max) {
			break;
		}
	}

	_debug_drawer->end();*/
}

void VoxelChunk::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_create_mesher"));
	BIND_VMETHOD(MethodInfo("_build_mesh"));

	ClassDB::bind_method(D_METHOD("get_library_path"), &VoxelChunk::get_library_path);
	ClassDB::bind_method(D_METHOD("set_library_path", "value"), &VoxelChunk::set_library_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "library_path"), "set_library_path", "get_library_path");

	ClassDB::bind_method(D_METHOD("get_mesh_instance_path"), &VoxelChunk::get_mesh_instance_path);
	ClassDB::bind_method(D_METHOD("set_mesh_instance_path", "value"), &VoxelChunk::set_mesh_instance_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "mesh_instance_path"), "set_mesh_instance_path", "get_mesh_instance_path");

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelChunk::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelChunk::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelChunk::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelChunk::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_buffer"), &VoxelChunk::get_buffer);

	ADD_GROUP("Meshing", "meshing");

	ClassDB::bind_method(D_METHOD("meshing_get_build_mesh"), &VoxelChunk::get_build_mesh);
	ClassDB::bind_method(D_METHOD("meshing_set_build_mesh", "value"), &VoxelChunk::set_build_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_build_mesh"), "meshing_set_build_mesh", "meshing_get_build_mesh");

	ClassDB::bind_method(D_METHOD("meshing_get_create_collider"), &VoxelChunk::get_create_collider);
	ClassDB::bind_method(D_METHOD("meshing_set_create_collider", "value"), &VoxelChunk::set_create_collider);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_create_collider"), "meshing_set_create_collider", "meshing_get_create_collider");

	ClassDB::bind_method(D_METHOD("meshing_get_bake_lights"), &VoxelChunk::get_bake_lights);
	ClassDB::bind_method(D_METHOD("meshing_set_bake_lights", "value"), &VoxelChunk::set_bake_lights);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_bake_lights"), "meshing_set_bake_lights", "meshing_get_bake_lights");

	ClassDB::bind_method(D_METHOD("meshing_get_bake_ambient_occlusion"), &VoxelChunk::get_bake_ambient_occlusion);
	ClassDB::bind_method(D_METHOD("meshing_set_bake_ambient_occlusion", "value"), &VoxelChunk::set_bake_ambient_occlusion);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_bake_ambient_occlusion"), "meshing_set_bake_ambient_occlusion", "meshing_get_bake_ambient_occlusion");

	ADD_GROUP("Settings", "setting");

	ClassDB::bind_method(D_METHOD("get_debug_drawer_path"), &VoxelChunk::get_debug_drawer_path);
	ClassDB::bind_method(D_METHOD("set_debug_drawer_path", "value"), &VoxelChunk::set_debug_drawer_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "debug_drawer_path"), "set_debug_drawer_path", "get_debug_drawer_path");

	ADD_GROUP("Ambient Occlusion", "ao");
	ClassDB::bind_method(D_METHOD("get_ao_radius"), &VoxelChunk::get_ao_radius);
	ClassDB::bind_method(D_METHOD("set_ao_radius", "value"), &VoxelChunk::set_ao_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_radius"), "set_ao_radius", "get_ao_radius");

	ClassDB::bind_method(D_METHOD("get_ao_intensity"), &VoxelChunk::get_ao_intensity);
	ClassDB::bind_method(D_METHOD("set_ao_intensity", "value"), &VoxelChunk::set_ao_intensity);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_intensity"), "set_ao_intensity", "get_ao_intensity");

	ClassDB::bind_method(D_METHOD("get_ao_sample_count"), &VoxelChunk::get_ao_sample_count);
	ClassDB::bind_method(D_METHOD("set_ao_sample_count", "value"), &VoxelChunk::set_ao_sample_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ao_sample_count"), "set_ao_sample_count", "get_ao_sample_count");

	ClassDB::bind_method(D_METHOD("get_mesher"), &VoxelChunk::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "Mesher"), &VoxelChunk::set_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "VoxelMesher"), "set_mesher", "get_mesher");

	ClassDB::bind_method(D_METHOD("build"), &VoxelChunk::build);
	ClassDB::bind_method(D_METHOD("finalize_mesh"), &VoxelChunk::finalize_mesh);

	ClassDB::bind_method(D_METHOD("clear"), &VoxelChunk::clear);

	ClassDB::bind_method(D_METHOD("query_marching_cubes_data", "query"), &VoxelChunk::query_marching_cubes_data);
	ClassDB::bind_method(D_METHOD("create_mesh_for_marching_cubes_query", "query"), &VoxelChunk::create_mesh_for_marching_cubes_query);

	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max"), &VoxelChunk::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));
	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights", "max", "localPosition"), &VoxelChunk::draw_debug_voxel_lights);
}

VoxelChunk::VoxelChunk() {
	_build_mesh = true;
	_create_collider = true;
	_bake_lights = true;
	_bake_ambient_occlusion = true;

	_ao_radius = 6;
	_ao_intensity = 1;
	_ao_sample_count = 5;

	_voxel_scale = 1;

	_buffer.instance();

	_debug_drawer = NULL;
}

VoxelChunk::~VoxelChunk() {
	_voxel_lights.clear();

	if (_mesher.is_valid()) {
		_mesher.unref();
	}

	_buffer.unref();

	_debug_drawer = NULL;
}
