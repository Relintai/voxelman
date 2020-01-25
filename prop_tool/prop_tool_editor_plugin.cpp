#include "prop_tool_editor_plugin.h"

#include "editor/editor_scale.h"

void PropDataEditor::edit(const Ref<PropData> &prop) {
}


void PropDataEditor::_bind_methods() {
}

PropDataEditor::PropDataEditor(EditorNode *p_editor) {
	set_h_size_flags(SIZE_EXPAND_FILL);
    set_custom_minimum_size(Size2(0, 200) * EDSCALE);

    Tree *tree = memnew(Tree);
    tree->set_custom_minimum_size(Size2(100, 0) * EDSCALE);
	add_child(tree);

	ViewportContainer *container = memnew(ViewportContainer);
    container->set_h_size_flags(SIZE_EXPAND_FILL);
    container->set_h_size_flags(SIZE_EXPAND_FILL);
    container->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(container);

    Viewport *viewport = memnew(Viewport);
    viewport->set_size(Size2(400, 400));
    container->add_child(viewport);

    set_split_offset(200 * EDSCALE);
}
PropDataEditor::~PropDataEditor() {
}

void PropDataEditorPlugin::edit(Object *p_object) {
}

bool PropDataEditorPlugin::handles(Object *p_object) const {

	bool handles = p_object->is_class("PropData");

	if (handles) {
		prop_editor_button->show();
	} else {
		prop_editor_button->hide();
	}

	return handles;
}

void PropDataEditorPlugin::make_visible(bool p_visible) {
}

PropDataEditorPlugin::PropDataEditorPlugin(EditorNode *p_node) {

	editor = p_node;

	prop_editor = memnew(PropDataEditor(editor));

	prop_editor_button = add_control_to_bottom_panel(prop_editor, "Prop");

	prop_editor->hide();
}

PropDataEditorPlugin::~PropDataEditorPlugin() {
}
