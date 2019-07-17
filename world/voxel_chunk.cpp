#include "voxel_chunk.h"

int VoxelChunk::get_chunk_position_x() {
	return _chunk_position.x;
}
void VoxelChunk::set_chunk_position_x(int value) {
	_chunk_position.x = value;
}

int VoxelChunk::get_chunk_position_y() {
	return _chunk_position.y;
}
void VoxelChunk::set_chunk_position_y(int value) {
	_chunk_position.y = value;
}

int VoxelChunk::get_chunk_position_z() {
	return _chunk_position.z;
}
void VoxelChunk::set_chunk_position_z(int value) {
	_chunk_position.z = value;
}

Vector3i VoxelChunk::get_chunk_position() const {
	return _chunk_position;
}
void VoxelChunk::set_chunk_position(int x, int y, int z) {
	_chunk_position.x = x;
	_chunk_position.y = y;
	_chunk_position.z = z;
}

int VoxelChunk::get_chunk_size_x() {
	return _chunk_size.x;
}
void VoxelChunk::set_chunk_size_x(int value) {
	_chunk_size.x = value;
}

int VoxelChunk::get_chunk_size_y() {
	return _chunk_size.y;
}
void VoxelChunk::set_chunk_size_y(int value) {
	_chunk_size.y = value;
}

int VoxelChunk::get_chunk_size_z() {
	return _chunk_size.z;
}
void VoxelChunk::set_chunk_size_z(int value) {
	_chunk_size.z = value;
}

Vector3i VoxelChunk::get_chunk_size() const {
	return _chunk_size;
}
void VoxelChunk::set_chunk_size(int x, int y, int z) {
	_chunk_size.x = x;
	_chunk_size.y = y;
	_chunk_size.z = z;
}

NodePath VoxelChunk::get_mesh_instance_path() const {
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

float VoxelChunk::get_voxel_scale() const {
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

bool VoxelChunk::get_build_mesh() const {
	return _build_mesh;
}
void VoxelChunk::set_build_mesh(bool value) {
	_build_mesh = value;
}

bool VoxelChunk::get_create_collider() const {
	return _create_collider;
}
void VoxelChunk::set_create_collider(bool value) {
	_create_collider = value;
}

bool VoxelChunk::get_bake_lights() const {
	return _bake_lights;
}
void VoxelChunk::set_bake_lights(bool value) {
	_bake_lights = value;
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

	if (!_mesher.is_valid()) {
		call("_create_mesher");

		ERR_FAIL_COND(!_mesher.is_valid());
	}

	_mesher->set_library(_library);

	if (_debug_drawer == NULL) {
		Node *n = get_node_or_null(_debug_drawer_path);

		if (n != NULL) {
			_debug_drawer = Object::cast_to<ImmediateGeometry>(n);
		}
	}

	if (get_build_mesh()) {
		if (has_method("_create_mesh")) {
			call("_create_mesh");
		} else {
			_mesher->add_buffer(_buffer);
		}

		finalize_mesh();
	}

	if (get_create_collider()) {
		update_collider();
	}
}

void VoxelChunk::_create_mesher() {
	_mesher = Ref<VoxelMesher>(memnew(VoxelMesher()));
}

void VoxelChunk::finalize_mesh() {
	_mesher->set_library(_library);

	Node *node = get_node(_mesh_instance_path);

	ERR_FAIL_COND(node == NULL);

	_mesh_instance = Object::cast_to<MeshInstance>(node);

	ERR_FAIL_COND(_mesh_instance == NULL);

	//if (get_bake_ambient_occlusion()) {
	//	set_enabled(true);
	//} else {
	Ref<ArrayMesh> mesh = get_mesher()->build_mesh();

	_mesh_instance->set_mesh(mesh);
	//}
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

void VoxelChunk::set_enabled(bool p_enabled) {

	_enabled = p_enabled;

	if (is_inside_tree())
		set_physics_process_internal(p_enabled);
}

bool VoxelChunk::is_enabled() const {
	return _enabled;
}

void VoxelChunk::add_lights(Array lights) {
	for (int i = 0; i < lights.size(); ++i) {
		Ref<VoxelLight> light = Ref<VoxelLight>(lights.get(i));

		if (light.is_valid()) {
			add_voxel_light(light);
		}
	}
}
void VoxelChunk::add_voxel_light(Ref<VoxelLight> light) {
	_voxel_lights.push_back(light);
}
void VoxelChunk::remove_voxel_light(Ref<VoxelLight> light) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		if (_voxel_lights[i] == light) {
			_voxel_lights.remove(i);
			return;
		}
	}
}
void VoxelChunk::clear_voxel_lights() {
	_voxel_lights.clear();
}

void VoxelChunk::get_lights(Array lights) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		lights.append(_voxel_lights[i]);
	}
}

void VoxelChunk::bake_lights() {
	clear_baked_lights();

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		bake_light(_voxel_lights[i]);
	}
}
void VoxelChunk::bake_light(Ref<VoxelLight> light) {
	ERR_FAIL_COND(!light.is_valid());

	Vector3i wp = light->get_world_position();

	int wpx = wp.x - (_chunk_position.x * _chunk_size.x);
	int wpy = wp.y - (_chunk_position.y * _chunk_size.y);
	int wpz = wp.z - (_chunk_position.z * _chunk_size.z);


	//int wpx = (int)(wp.x / _chunk_size.x) - _chunk_position.x;
	//int wpy = (int)(wp.y / _chunk_size.y) - _chunk_position.y;
	//int wpz = (int)(wp.z / _chunk_size.z) - _chunk_position.z;

	_buffer->add_light(wpx, wpy, wpz, light->get_size(), light->get_color());
}

void VoxelChunk::clear_baked_lights() {
	_buffer->clear_lights();
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
	if (_debug_drawer == NULL) {
		Node *n = get_node(_debug_drawer_path);

		if (n != NULL) {
			_debug_drawer = Object::cast_to<ImmediateGeometry>(n);
		}
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PRIMITIVE_LINES);
	_debug_drawer->set_color(color);

	int a = 0;
	Vector3i size = _buffer->get_size();

	for (int y = 0; y < size.y; ++y) {
		for (int z = 0; z < size.z; ++z) {
			for (int x = 0; x < size.x; ++x) {

				int type = _buffer->get_voxel(x, y, z, VoxelBuffer::CHANNEL_TYPE);

				if (type == 0) {
					continue;
				}

				draw_cross_voxels(Vector3(x, y, z), _buffer->get_voxel(x, y, z, VoxelBuffer::CHANNEL_ISOLEVEL) / 255.0);

				++a;

				if (a > max) {
					break;
				}
			}
		}
	}

	_debug_drawer->end();
}

void VoxelChunk::draw_debug_voxel_lights() {
	if (_debug_drawer == NULL) {
		Node *n = get_node(_debug_drawer_path);

		if (n != NULL) {
			_debug_drawer = Object::cast_to<ImmediateGeometry>(n);
		}
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);
	_debug_drawer->set_color(Color(1, 1, 1));

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		Ref<VoxelLight> v = _voxel_lights[i];

		Vector3i pos = v->get_world_position();

		int pos_x = pos.x - (_chunk_size.x * _chunk_position.x);
		int pos_y = pos.y - (_chunk_size.y * _chunk_position.y);
		int pos_z = pos.z - (_chunk_size.z * _chunk_position.z);

		draw_cross_voxels(Vector3(pos_x, pos_y, pos_z), 1.0);
	}

	_debug_drawer->end();
}

void VoxelChunk::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_create_mesh"));
	BIND_VMETHOD(MethodInfo("_create_mesher"));

	ClassDB::bind_method(D_METHOD("_create_mesher"), &VoxelChunk::_create_mesher);

	ClassDB::bind_method(D_METHOD("get_chunk_position_x"), &VoxelChunk::get_chunk_position_x);
	ClassDB::bind_method(D_METHOD("set_chunk_position_x", "value"), &VoxelChunk::set_chunk_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_position_x"), "set_chunk_position_x", "get_chunk_position_x");

	ClassDB::bind_method(D_METHOD("get_chunk_position_y"), &VoxelChunk::get_chunk_position_y);
	ClassDB::bind_method(D_METHOD("set_chunk_position_y", "value"), &VoxelChunk::set_chunk_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_position_y"), "set_chunk_position_y", "get_chunk_position_y");

	ClassDB::bind_method(D_METHOD("get_chunk_position_z"), &VoxelChunk::get_chunk_position_z);
	ClassDB::bind_method(D_METHOD("set_chunk_position_z", "value"), &VoxelChunk::set_chunk_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_position_z"), "set_chunk_position_x", "get_chunk_position_z");

	ClassDB::bind_method(D_METHOD("set_chunk_position", "x", "y", "z"), &VoxelChunk::set_chunk_position);

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelChunk::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelChunk::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelChunk::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelChunk::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelChunk::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelChunk::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_x", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("set_chunk_size", "x", "y", "z"), &VoxelChunk::set_chunk_size);

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

	ADD_GROUP("Settings", "setting");

	ClassDB::bind_method(D_METHOD("get_debug_drawer_path"), &VoxelChunk::get_debug_drawer_path);
	ClassDB::bind_method(D_METHOD("set_debug_drawer_path", "value"), &VoxelChunk::set_debug_drawer_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "debug_drawer_path"), "set_debug_drawer_path", "get_debug_drawer_path");

	ClassDB::bind_method(D_METHOD("get_mesher"), &VoxelChunk::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "Mesher"), &VoxelChunk::set_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "VoxelMesher"), "set_mesher", "get_mesher");

	ClassDB::bind_method(D_METHOD("add_lights", "lights"), &VoxelChunk::add_lights);
	ClassDB::bind_method(D_METHOD("add_voxel_light", "light"), &VoxelChunk::add_voxel_light);
	ClassDB::bind_method(D_METHOD("remove_voxel_light", "light"), &VoxelChunk::remove_voxel_light);
	ClassDB::bind_method(D_METHOD("clear_voxel_lights"), &VoxelChunk::clear_voxel_lights);
	ClassDB::bind_method(D_METHOD("get_lights", "lights"), &VoxelChunk::get_lights);
	ClassDB::bind_method(D_METHOD("bake_lights"), &VoxelChunk::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &VoxelChunk::bake_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &VoxelChunk::clear_baked_lights);

	ClassDB::bind_method(D_METHOD("build"), &VoxelChunk::build);
	ClassDB::bind_method(D_METHOD("finalize_mesh"), &VoxelChunk::finalize_mesh);

	ClassDB::bind_method(D_METHOD("clear"), &VoxelChunk::clear);

	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max"), &VoxelChunk::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));
	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &VoxelChunk::draw_debug_voxel_lights);
}

VoxelChunk::VoxelChunk() {
	_build_mesh = true;
	_create_collider = true;
	_bake_lights = true;

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

	if (_library.is_valid()) {
		_library.unref();
	}

	if (_mesh.is_valid()) {
		_mesh.unref();
	}
}
