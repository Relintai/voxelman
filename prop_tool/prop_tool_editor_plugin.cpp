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
