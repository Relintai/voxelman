#ifndef PROP_TOOL_EDITOR_PLUGIN_H
#define PROP_TOOL_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor/pane_drag.h"

#include "scene/gui/graph_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/main/viewport.h"

#include "../props/prop_data.h"

#include "scene/gui/viewport_container.h"

#include "editor/plugins/spatial_editor_plugin.h"

class PropDataEditor : public HSplitContainer {
	GDCLASS(PropDataEditor, HSplitContainer);

public:
	void edit(const Ref<PropData> &prop);

	PropDataEditor() {}
	PropDataEditor(EditorNode *p_editor);
	~PropDataEditor();

protected:
	static void _bind_methods();

private:
};

class PropDataEditorPlugin : public EditorPlugin {

	GDCLASS(PropDataEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "PropData"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	PropDataEditorPlugin(EditorNode *p_node);
	~PropDataEditorPlugin();

protected:
private:
	PropDataEditor *prop_editor;
	ToolButton *prop_editor_button;
	EditorNode *editor;
};

#endif
