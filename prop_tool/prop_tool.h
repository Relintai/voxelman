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

#ifndef PROP_TOOL_H
#define PROP_TOOL_H

class EditorNode;
class EditorPlugin;
class PropData;

#include "core/math/vector3.h"
#include "core/reference.h"
#include "scene/3d/spatial.h"

class propData;
class PropToolEditorPlugin;

class PropTool : public Spatial {
	GDCLASS(PropTool, Spatial);

#ifdef TOOLS_ENABLED
public:
	void edit(const Ref<PropData> &prop);

	void save();
	void save_node(Node *node, Transform parent_transform);
	void rebuild_hierarchy();
	void refresh_set(bool value);
	void set_target_prop(const Ref<PropData> &prop);
	void target_prop_set(const Ref<PropData> &prop);
	void load_scene_for(PropTool *t, const Ref<PropData> &prop);

	PropToolEditorPlugin *get_plugin();
	void set_plugin(PropToolEditorPlugin *plugin);

	PropTool();
	PropTool(EditorNode *p_editor);
	~PropTool();

protected:
	static void _bind_methods();

private:
	bool _refresh;
	Ref<PropData> _target_prop;
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	PropToolEditorPlugin *_plugin;
#endif
};

#endif
