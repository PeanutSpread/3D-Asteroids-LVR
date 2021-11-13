#version 330

// vertex attributes
layout(location=0) in vec4 in_Position;
layout(location=2) in vec4 in_Color;

// transformations
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelviewMatrix;

// output for rasterizer
out vec4 var_Color;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ModelviewMatrix * in_Position;
    var_Color = in_Color;
}
