#version 330

// input from rasterizer
in vec3 var_LightColor;		// interpolated per-vertex light color
in vec2 var_TexCoord;		// interpolated texture coordinate

// input from application
uniform sampler2D u_TexSampler;

// output to framebuffer
out vec4 out_Color;

void main()
{
    // texture lookup
    vec4 texColor = texture2D(u_TexSampler, var_TexCoord);

	// apply lighting
	out_Color.rgb = texColor.rgb * var_LightColor;
	out_Color.a = texColor.a;

	//out_Color.rgb = var_LightColor;  // for debugging
}
