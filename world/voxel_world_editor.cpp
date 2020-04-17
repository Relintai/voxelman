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

#include "../library/voxel_surface.h"
#include "../library/voxelman_library.h"

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
				return do_input_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true);
			} else {
				return false;
			}

			//return do_input_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true);
		}
	}

	return false;
}

bool VoxelWorldEditor::do_input_action(Camera *p_camera, const Point2 &p_point, bool p_click) {

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
		Vector3 pos;
		int selected_voxel = 0;
		int channel = 0;

		if (_current_tab == 0) {
			channel = _channel_type;
		} else {
			channel = _channel_liquid_type;
		}

		if (channel == -1)
			return false;

		if (_tool_mode == TOOL_MODE_ADD) {
			pos = (res.position + (Vector3(0.1, 0.1, 0.1) * res.normal));
			selected_voxel = _selected_type + 1;
		} else if (_tool_mode == TOOL_MODE_REMOVE) {
			pos = (res.position + (Vector3(0.1, 0.1, 0.1) * -res.normal));
			selected_voxel = 0;
		}

		_world->set_voxel_at_world_position(pos, selected_voxel, channel);

		return true;
	}

	return false;
}

void VoxelWorldEditor::edit(VoxelWorld *p_world) {
	_world = p_world;

	if (!_world)
		return;

	_channel_type = _world->get_channel_index_info(VoxelWorld::CHANNEL_TYPE_INFO_TYPE);
	_channel_liquid_type = _world->get_channel_index_info(VoxelWorld::CHANNEL_TYPE_INFO_LIQUID);

	spatial_editor = Object::cast_to<SpatialEditorPlugin>(_editor->get_editor_plugin_screen());

	for (int i = 0; i < _surfaces_vbox_container->get_child_count(); ++i) {
		Node *child = _surfaces_vbox_container->get_child(i);

		if (!child->is_queued_for_deletion()) {
			child->queue_delete();
		}
	}

	for (int i = 0; i < _liquid_surfaces_vbox_container->get_child_count(); ++i) {
		Node *child = _liquid_surfaces_vbox_container->get_child(i);

		if (!child->is_queued_for_deletion()) {
			child->queue_delete();
		}
	}

	Ref<VoxelmanLibrary> library = _world->get_library();

	if (!library.is_valid())
		return;

	bool f = false;
	for (int i = 0; i < library->get_num_surfaces(); ++i) {
		Ref<VoxelSurface> surface = library->get_voxel_surface(i);

		if (!surface.is_valid())
			continue;

		String text = String::num(i) + " - " + surface->get_name();

		Button *button = memnew(Button);
		button->set_text(text);
		button->set_text_align(Button::ALIGN_LEFT);
		button->set_meta("index", i);
		button->set_toggle_mode(true);
		button->set_button_group(_surfaces_button_group);
		button->set_h_size_flags(SIZE_EXPAND_FILL);
		button->connect("button_up", this, "_on_surface_button_pressed");
		_surfaces_vbox_container->add_child(button);

		if (!f) {
			button->set_pressed(true);
			f = true;
		}
	}

	f = false;
	for (int i = 0; i < library->get_num_liquid_surfaces(); ++i) {
		Ref<VoxelSurface> surface = library->get_liquid_surface(i);

		if (!surface.is_valid())
			continue;

		String text = String::num(i) + " - " + surface->get_name();

		Button *button = memnew(Button);
		button->set_h_size_flags(SIZE_EXPAND_FILL);
		button->set_text(text);
		button->set_text_align(Button::ALIGN_LEFT);
		button->set_meta("index", i);
		button->set_toggle_mode(true);
		button->set_button_group(_liquid_surfaces_button_group);
		button->connect("button_up", this, "_on_surface_button_pressed");
		_liquid_surfaces_vbox_container->add_child(button);

		if (!f) {
			button->set_pressed(true);
			f = true;
		}
	}
}

VoxelWorldEditor::VoxelWorldEditor() {
	_world = NULL;
	_selected_type = 0;
	_selected_liquid_type = 0;
	_channel_type = -1;
	_channel_liquid_type = -1;
	_editor = NULL;
	_tool_mode = TOOL_MODE_ADD;
	_current_tab = 0;
}
VoxelWorldEditor::VoxelWorldEditor(EditorNode *p_editor) {
	_world = NULL;
	_selected_type = 0;
	_selected_liquid_type = 0;
	_channel_type = -1;
	_channel_liquid_type = -1;
	_editor = p_editor;
	_tool_mode = TOOL_MODE_ADD;
	_current_tab = 0;

	spatial_editor_hb = memnew(HBoxContainer);
	spatial_editor_hb->set_h_size_flags(SIZE_EXPAND_FILL);
	spatial_editor_hb->set_alignment(BoxContainer::ALIGN_BEGIN);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(spatial_editor_hb);

	_tool_button_group.instance();

	ToolButton *add_button = memnew(ToolButton);
	add_button->set_text("Add");
	add_button->set_toggle_mode(true);
	add_button->set_pressed(true);
	add_button->set_button_group(_tool_button_group);
	add_button->set_meta("tool_mode", TOOL_MODE_ADD);
	add_button->connect("button_up", this, "_on_tool_button_pressed");
	add_button->set_shortcut(ED_SHORTCUT("voxelman_world_editor/add_mode", "Add Mode", KEY_A));
	spatial_editor_hb->add_child(add_button);

	ToolButton *remove_button = memnew(ToolButton);
	remove_button->set_text("Remove");
	remove_button->set_toggle_mode(true);
	remove_button->set_button_group(_tool_button_group);
	remove_button->set_meta("tool_mode", TOOL_MODE_REMOVE);
	remove_button->connect("button_up", this, "_on_tool_button_pressed");
	remove_button->set_shortcut(ED_SHORTCUT("voxelman_world_editor/remove_mode", "Remove Mode", KEY_S));
	spatial_editor_hb->add_child(remove_button);

	set_custom_minimum_size(Size2(200 * EDSCALE, 0));

	TabContainer *tab_container = memnew(TabContainer);
	tab_container->set_h_size_flags(SIZE_EXPAND_FILL);
	tab_container->set_v_size_flags(SIZE_EXPAND_FILL);
	tab_container->connect("tab_selected", this, "_tab_selected");
	add_child(tab_container);

	ScrollContainer *scs = memnew(ScrollContainer);
	scs->set_h_size_flags(SIZE_EXPAND_FILL);
	scs->set_v_size_flags(SIZE_EXPAND_FILL);
	scs->set_name("Surfaces");
	tab_container->add_child(scs);

	_surfaces_vbox_container = memnew(VBoxContainer);
	scs->add_child(_surfaces_vbox_container);
	_surfaces_vbox_container->set_h_size_flags(SIZE_EXPAND_FILL);

	ScrollContainer *scsl = memnew(ScrollContainer);
	scsl->set_h_size_flags(SIZE_EXPAND_FILL);
	scsl->set_v_size_flags(SIZE_EXPAND_FILL);
	scsl->set_name("Liquids");
	tab_container->add_child(scsl);

	_liquid_surfaces_vbox_container = memnew(VBoxContainer);
	scsl->add_child(_liquid_surfaces_vbox_container);
	_liquid_surfaces_vbox_container->set_h_size_flags(SIZE_EXPAND_FILL);

	_surfaces_button_group.instance();
	_liquid_surfaces_button_group.instance();
}
VoxelWorldEditor::~VoxelWorldEditor() {
	_world = NULL;

	_surfaces_button_group.unref();
	_liquid_surfaces_button_group.unref();
}

void VoxelWorldEditor::_node_removed(Node *p_node) {

	if (p_node == _world)
		_world = NULL;
}

void VoxelWorldEditor::_on_surface_button_pressed() {

	BaseButton *button = _surfaces_button_group->get_pressed_button();

	if (button) {
		_selected_type = button->get_meta("index");
	}

	button = _liquid_surfaces_button_group->get_pressed_button();

	if (button) {
		_selected_liquid_type = button->get_meta("index");
	}
}

void VoxelWorldEditor::_on_tool_button_pressed() {
	BaseButton *button = _tool_button_group->get_pressed_button();

	if (button) {
		_tool_mode = static_cast<VoxelWorldEditorToolMode>(static_cast<int>(button->get_meta("tool_mode")));
	}
}

void VoxelWorldEditor::_tab_selected(int tab) {
	_current_tab = tab;
}

void VoxelWorldEditor::_bind_methods() {
	ClassDB::bind_method("_node_removed", &VoxelWorldEditor::_node_removed);
	ClassDB::bind_method("_on_surface_button_pressed", &VoxelWorldEditor::_on_surface_button_pressed);
	ClassDB::bind_method("_on_tool_button_pressed", &VoxelWorldEditor::_on_tool_button_pressed);
	ClassDB::bind_method("_tab_selected", &VoxelWorldEditor::_tab_selected);
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
