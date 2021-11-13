#version 330

// vertex attributes
layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 3) in vec2 in_TexCoord;

// transformations
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelviewMatrix;
uniform mat3 u_NormalMatrix;

// directional light info
uniform vec3 u_LightColor;
uniform vec3 u_LightDir;    // direction to light

// outputs to rasterizer
out vec3 var_LightColor;
out vec2 var_TexCoord;

void main()
{
	// transform vertex position
    gl_Position = u_ProjectionMatrix * u_ModelviewMatrix * in_Position;

	// can remove these normalizations if we're absolutely sure that normals and light directions are unit vectors
	vec3 N = normalize(u_NormalMatrix * in_Normal);		// transform surface normal
	vec3 L = normalize(u_LightDir);						// direction to light

	// compute diffuse lighting intensity
	float NdotL = max(dot(N, L), 0);	// assumes N and L are unit vectors; clamps negative values to 0

	// pass light color to rasterizer
	var_LightColor = NdotL * u_LightColor;

	// pass texture coordinate to rasterizer
	var_TexCoord = in_TexCoord;

	// possible debugging stuff
	//var_LightColor = NdotL * vec3(1, 1, 1);
	//var_LightColor = normalize(in_Normal);
	//var_LightColor = normalize(in_Position.xyz);
	//var_LightColor = u_LightColor;
}
