#version 330

// interpolated input from rasterizer
in vec4 var_Color;

// output to framebuffer
out vec4 out_Color;

void main()
{
    out_Color = var_Color;
}
