/*
Copyright (c) 2020 Péter Magyar

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

#include "voxel_world_editor.h"

#include "core/version.h"

#include "editor/editor_scale.h"
#include "editor/editor_settings.h"

#include "voxel_world.h"

#include "core/math/geometry.h"
#include "core/os/keyboard.h"

#include "voxel_chunk.h"

#include "core/version.h"

#if VERSION_MAJOR < 4

#include "core/os/input.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"

#else

#include "core/input/input_event.h"
#include "editor/plugins/node_3d_editor_plugin.h"
#include "scene/3d/camera_3d.h"

#define PhysicsDirectSpaceState PhysicsDirectSpaceState3D
#define SpatialEditor Node3DEditor
#define SpatialEditorPlugin Node3DEditorPlugin

#endif

bool VoxelWorldEditor::forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event) {
	if (!_world || !_world->get_editable()) {
		return false;
	}

	Ref<InputEventMouseButton> mb = p_event;

	if (mb.is_valid()) {

		if (mb->is_pressed()) {
			Ref<VoxelmanLibrary> lib = _world->get_library();

			if (!lib.is_valid())
				return false;

			if (mb->get_button_index() == BUTTON_LEFT) {
				return do_input_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true, _seletced_type);
			} else {
				return false;
			}

			//return do_input_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true);
		}
	}

	return false;
}

bool VoxelWorldEditor::do_input_action(Camera *p_camera, const Point2 &p_point, bool p_click, int selected_voxel) {

	if (!spatial_editor || !_world)
		return false;

	Camera *camera = p_camera;
	Vector3 from = camera->project_ray_origin(p_point);
	Vector3 to = from + camera->project_ray_normal(p_point) * 10000;
	Transform local_xform = _world->get_global_transform().affine_inverse();

	from = local_xform.xform(from);
	to = local_xform.xform(to);

	PhysicsDirectSpaceState *ss = _world->get_world()->get_direct_space_state();
	PhysicsDirectSpaceState::RayResult res;

	if (ss->intersect_ray(from, to, res)) {
		Vector3 pos = (res.position + (Vector3(0.1, 0.1, 0.1) * res.normal)) / _world->get_voxel_scale();

		//todo add these?:
		//_world->set_voxel(pos, data[]);
		//Ref<VoxelChunk> chunk = _world->get_or_spawn_chunk_at_world_pos(pos);

		//Note: floor is needed to handle negative numbers proiberly
		int x = static_cast<int>(Math::floor(pos.x / _world->get_chunk_size_x() / _world->get_voxel_scale()));
		int y = static_cast<int>(Math::floor(pos.y / _world->get_chunk_size_y() / _world->get_voxel_scale()));
		int z = static_cast<int>(Math::floor(pos.z / _world->get_chunk_size_z() / _world->get_voxel_scale()));

		int bx = static_cast<int>(Math::floor(pos.x / _world->get_voxel_scale())) % _world->get_chunk_size_x();
		int by = static_cast<int>(Math::floor(pos.y / _world->get_voxel_scale())) % _world->get_chunk_size_y();
		int bz = static_cast<int>(Math::floor(pos.z / _world->get_voxel_scale())) % _world->get_chunk_size_z();

		if (bx < 0) {
			bx += _world->get_chunk_size_x();
		}

		if (by < 0) {
			by += _world->get_chunk_size_y();
		}

		if (bz < 0) {
			bz += _world->get_chunk_size_z();
		}

		if (_world->get_data_margin_end() > 0) {
			if (bx == 0) {
				Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x - 1, y, z);
				chunk->set_voxel(selected_voxel, _world->get_chunk_size_x(), by, bz, 0);
				chunk->build();
			}

			if (by == 0) {
				Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x, y - 1, z);
				chunk->set_voxel(selected_voxel, bx, _world->get_chunk_size_y(), bz, 0);
				chunk->build();
			}

			if (bz == 0) {
				Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x, y, z - 1);
				chunk->set_voxel(selected_voxel, bx, by, _world->get_chunk_size_z(), 0);
				chunk->build();
			}
		}

		if (_world->get_data_margin_start() > 0) {
			if (bx == _world->get_chunk_size_x() - 1) {
				Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x + 1, y, z);
				chunk->set_voxel(selected_voxel, -1, by, bz, 0);
				chunk->build();
			}

			if (by == _world->get_chunk_size_y() - 1) {
				Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x, y + 1, z);
				chunk->set_voxel(selected_voxel, bx, -1, bz, 0);
				chunk->build();
			}

			if (bz == _world->get_chunk_size_z() - 1) {
				Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x, y, z + 1);
				chunk->set_voxel(selected_voxel, bx, by, -1, 0);
				chunk->build();
			}
		}

		Ref<VoxelChunk> chunk = _world->get_or_create_chunk(x, y, z);
		chunk->set_voxel(selected_voxel, bx, by, bz, 0);
		chunk->build();

		return true;
	}

	return false;
}

void VoxelWorldEditor::edit(VoxelWorld *p_world) {
	_world = p_world;

	spatial_editor = Object::cast_to<SpatialEditorPlugin>(_editor->get_editor_plugin_screen());
}

VoxelWorldEditor::VoxelWorldEditor() {
	_world = NULL;
	_seletced_type = 1;
	_editor = NULL;
}
VoxelWorldEditor::VoxelWorldEditor(EditorNode *p_editor) {
	_world = NULL;
	_seletced_type = 1;
	_editor = p_editor;

	spatial_editor_hb = memnew(HBoxContainer);
	spatial_editor_hb->set_h_size_flags(SIZE_EXPAND_FILL);
	spatial_editor_hb->set_alignment(BoxContainer::ALIGN_END);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(spatial_editor_hb);
}
VoxelWorldEditor::~VoxelWorldEditor() {
	_world = NULL;
}

void VoxelWorldEditor::_node_removed(Node *p_node) {

	if (p_node == _world)
		_world = NULL;
}

void VoxelWorldEditor::_bind_methods() {
	ClassDB::bind_method("_node_removed", &VoxelWorldEditor::_node_removed);
}

void VoxelWorldEditorPlugin::_notification(int p_what) {

	if (p_what == EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED) {

		switch ((int)EditorSettings::get_singleton()->get("editors/voxelman/editor_side")) {
			case 0: { // Left.
				SpatialEditor::get_singleton()->get_palette_split()->move_child(voxel_world_editor, 0);
			} break;
			case 1: { // Right.
				SpatialEditor::get_singleton()->get_palette_split()->move_child(voxel_world_editor, 1);
			} break;
		}
	}
}

void VoxelWorldEditorPlugin::edit(Object *p_object) {

	voxel_world_editor->edit(Object::cast_to<VoxelWorld>(p_object));
}

bool VoxelWorldEditorPlugin::handles(Object *p_object) const {

	if (!p_object->is_class("VoxelWorld"))
		return false;

	VoxelWorld *w = Object::cast_to<VoxelWorld>(p_object);

	return w->get_editable();
}

void VoxelWorldEditorPlugin::make_visible(bool p_visible) {

	if (p_visible) {
		voxel_world_editor->show();
		voxel_world_editor->spatial_editor_hb->show();
		voxel_world_editor->set_process(true);
	} else {

		voxel_world_editor->spatial_editor_hb->hide();
		voxel_world_editor->hide();
		voxel_world_editor->edit(NULL);
		voxel_world_editor->set_process(false);
	}
}

VoxelWorldEditorPlugin::VoxelWorldEditorPlugin(EditorNode *p_node) {

	editor = p_node;

	EDITOR_DEF("editors/voxelman/editor_side", 1);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/voxelman/editor_side", PROPERTY_HINT_ENUM, "Left,Right"));

	voxel_world_editor = memnew(VoxelWorldEditor(editor));
	switch ((int)EditorSettings::get_singleton()->get("editors/voxelman/editor_side")) {
		case 0: { // Left.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_LEFT, voxel_world_editor);
		} break;
		case 1: { // Right.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT, voxel_world_editor);
		} break;
	}
	voxel_world_editor->hide();
}

VoxelWorldEditorPlugin::~VoxelWorldEditorPlugin() {
}
