#version 330

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 3) in vec2 in_TexCoord;

// transformations
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelviewMatrix;
uniform mat3 u_NormalMatrix;

// outputs to rasterizer
out vec2 var_TexCoord;
out vec3 var_Normal;
out vec3 var_Pos;    // vertex position in eye (camera) space

void main()
{
    gl_Position = u_ProjectionMatrix * u_ModelviewMatrix * in_Position;

	// pass texture coordinate to rasterizer
	var_TexCoord = in_TexCoord;

	// transform surface normal
	// (can remove this normalization if we're absolutely sure that normals are unit vectors)
	var_Normal = normalize(u_NormalMatrix * in_Normal);

	// transform position to eye (camera) space
	var_Pos = vec3(u_ModelviewMatrix * in_Position);
}
