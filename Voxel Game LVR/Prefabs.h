#ifndef PREFABS_H_
#define PREFABS_H_

#include "Mesh.h"

Mesh*   CreateSolidBox_Nolight      (float width, float height, float depth);  // positions only
Mesh*   CreateSolidBox              (float width, float height, float depth);  // positions and normals
Mesh*   CreateWireframeBox          (float width, float height, float depth);  // positions only

Mesh*   CreateSolidCube_Nolight     (float width);  // positions only
Mesh*   CreateSolidCube             (float width);  // positions and normals
Mesh*   CreateWireframeCube         (float width);  // positions only

Mesh*   CreateChunkyCylinder        (float radius, float height, int numSegments);  // positions and per-face normals
Mesh*   CreateSmoothCylinder        (float radius, float height, int numSegments);  // positions and per-vertex normals

Mesh*   CreateChunkyCone            (float radius, float height, int numSegments);  // positions and per-face normals

//
// Texturable stuff
//

Mesh*   CreateTexturedCube                (float width);
Mesh*   CreateChunkyTexturedCylinder      (float radius, float height, int numSegments);
Mesh*   CreateSmoothTexturedCylinder      (float radius, float height, int numSegments);
Mesh*   CreateTexturedQuad                (float width, float height, float uTile, float vTile);

//
// Debugging stuff
//

Mesh*   CreateAxes                  (float scale);  // positions and colors only

#endif
