#include "prop_tool.h"

PropTool::PropTool() {
    _snap_to_mesh = false;
    _snap_axis = Vector3(0, -1, 0);
}
PropTool::PropTool(EditorNode *p_editor) {
    _snap_to_mesh = false;
    _snap_axis = Vector3(0, -1, 0);
}

