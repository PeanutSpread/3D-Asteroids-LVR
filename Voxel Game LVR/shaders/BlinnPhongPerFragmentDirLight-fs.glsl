#version 330

// inputs from rasterizer
in vec2 var_TexCoord;		// interpolated texture coordinate
in vec3 var_Normal;
in vec3 var_Pos;    // vertex position in eye (camera) space

uniform sampler2D u_TexSampler;

// global light info
uniform vec3 u_AmbientLightColor;

// directional light info
uniform vec3 u_LightColor;
uniform vec3 u_LightDir;

// material properties
uniform vec3 u_MatEmissiveColor;
uniform vec3 u_MatSpecularColor;
uniform float u_MatShininess;

// output to framebuffer
out vec4 out_Color;

void main()
{
	// texture lookup
    vec4 matColor = texture2D(u_TexSampler, var_TexCoord);

	vec3 accumColor = u_MatEmissiveColor;

	accumColor += u_AmbientLightColor * matColor.rgb;

	// can remove these normalizations if we're absolutely sure that normals and light directions are unit vectors
	vec3 N = normalize(var_Normal);	    // surface normal
	vec3 L = normalize(u_LightDir);     // direction to light

	// compute diffuse lighting intensity
	float NdotL = dot(N, L);

	if (NdotL > 0) {

		accumColor += NdotL * u_LightColor * matColor.rgb;

		vec3 E = normalize(-var_Pos);    // direction to camera
		vec3 H = normalize(E + L);       // half vector for specular highlight

		float NdotH = dot(N, H);

		if (NdotH > 0) {
			float blinnTerm = pow(NdotH, u_MatShininess);
			accumColor += blinnTerm * u_LightColor * u_MatSpecularColor;
		}
	}

	out_Color = vec4(accumColor, matColor.a);
}
