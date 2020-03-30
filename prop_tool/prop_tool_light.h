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

#ifndef PROP_TOOL_LIGHT_H
#define PROP_TOOL_LIGHT_H

class EditorNode;
class EditorPlugin;
class PropData;

#include "../props/prop_data_light.h"
#include "scene/3d/light.h"

class propData;

class PropToolLight : public OmniLight {
	GDCLASS(PropToolLight, OmniLight);

#ifdef TOOLS_ENABLED
public:
	Ref<PropDataLight> get_data();
	void set_data(const Ref<PropDataLight> &data);

	bool get_snap_to_mesh() const;
	void set_snap_to_mesh(const bool value);

	PropToolLight();
	~PropToolLight();

protected:
	static void _bind_methods();

private:
	Ref<PropDataLight> _prop_light;
	bool _snap_to_mesh;
#endif
};

#endif
