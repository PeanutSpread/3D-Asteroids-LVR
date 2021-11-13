#version 330

// directional light info
struct DirLight {
	vec3 color;
	vec3 dir;
};

// point light info
struct PointLight {
	vec3 color;
	vec3 pos;
	float attQuad;
	float attLin;
	float attConst;
};


// inputs from rasterizer
in vec2 var_TexCoord;		// interpolated texture coordinate
in vec3 var_Normal;
in vec3 var_Pos;    // vertex position in eye (camera) space

uniform sampler2D u_TexSampler;

// global light info
uniform vec3 u_AmbientLightColor;

const int MAX_DIR_LIGHTS = 4;
const int MAX_POINT_LIGHTS = 8;

// arrays big enough to store the max number of lights
uniform DirLight u_DirLights[MAX_DIR_LIGHTS];
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

uniform int u_NumDirLights;
uniform int u_NumPointLights;

// material properties
uniform vec3 u_MatEmissiveColor;
uniform vec3 u_MatSpecularColor;
uniform float u_MatShininess;

// output to framebuffer
out vec4 out_Color;


float attenuate(float dist, float Q, float L, float C)
{
	return 1.0 / (Q * dist * dist + L * dist + C);
}


void main()
{
	// texture lookup
    vec4 matColor = texture2D(u_TexSampler, var_TexCoord);

	vec3 accumColor = u_MatEmissiveColor;

	accumColor += u_AmbientLightColor * matColor.rgb;

	// can remove these normalizations if we're absolutely sure that normals and light directions are unit vectors
	vec3 N = normalize(var_Normal);	    // surface normal

	vec3 E = normalize(-var_Pos);    // direction to camera

	for (int i = 0; i < u_NumDirLights; i++) {

		// can remove this normalization if we're absolutely sure that light directions are unit vectors
		vec3 L = normalize(u_DirLights[i].dir);		// compute direction to light

		// compute diffuse lighting intensity
		float NdotL = dot(N, L);

		if (NdotL > 0) {

			accumColor += NdotL * u_DirLights[i].color * matColor.rgb;

			vec3 H = normalize(E + L);

			float NdotH = dot(N, H);

			if (NdotH > 0) {
				float blinnTerm = pow(NdotH, u_MatShininess);
				accumColor += blinnTerm * u_DirLights[i].color * u_MatSpecularColor;
			}
		}
	}

	for (int i = 0;  i < u_NumPointLights; i++) {

		vec3 L = normalize(u_PointLights[i].pos - var_Pos);		// direction to light

		// compute diffuse lighting intensity
		float NdotL = dot(N, L);

		if (NdotL > 0) {

			// distance to light
			float dist = length(u_PointLights[i].pos - var_Pos);
			float attenuationFactor = attenuate(dist, u_PointLights[i].attQuad, u_PointLights[i].attLin, u_PointLights[i].attConst);

			vec3 lightIntensity = attenuationFactor * u_PointLights[i].color;

			accumColor += NdotL * lightIntensity * matColor.rgb;

			vec3 H = normalize(E + L);
			float NdotH = dot(N, H);
			if (NdotH > 0) {
				float blinnTerm = pow(NdotH, u_MatShininess);
				accumColor += blinnTerm * lightIntensity * u_MatSpecularColor;
			}
		}
	}

	out_Color = vec4(accumColor, 1.0);
}
