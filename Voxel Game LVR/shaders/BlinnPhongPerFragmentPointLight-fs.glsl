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
uniform vec3 u_LightPos;
uniform float u_AttQuad;
uniform float u_AttLin;
uniform float u_AttConst;

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

	vec3 lightDir = u_LightPos - var_Pos;

	// can remove these normalizations if we're absolutely sure that normals and light directions are unit vectors
	vec3 N = normalize(var_Normal);	    // surface normal
	vec3 L = normalize(lightDir);       // direction to light

	// compute diffuse lighting intensity
	float NdotL = dot(N, L);

	if (NdotL > 0) {

		// distance to light
		float dist = length(u_LightPos - var_Pos);
		float attenuationFactor = attenuate(dist, u_AttQuad, u_AttLin, u_AttConst);

		accumColor += NdotL * attenuationFactor * u_LightColor * matColor.rgb;

		vec3 E = normalize(-var_Pos);    // direction to camera
		vec3 H = normalize(E + L);       // half vector for specular highlight

		float NdotH = dot(N, H);

		if (NdotH > 0) {
			float blinnTerm = pow(NdotH, u_MatShininess);
			accumColor += blinnTerm * attenuationFactor * u_LightColor * u_MatSpecularColor;
		}
	}

	//out_Color = vec4(1, 0, 1, 1);
	out_Color = vec4(accumColor, matColor.a);
}

/*
#version 330

// global light info
uniform vec3 uAmbientLightColor;

// directional light info
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform float uAttQuad;
uniform float uAttLin;
uniform float uAttConst;

// material properties
uniform vec3 uMatDiffuseColor;
uniform vec3 uMatSpecularColor;
uniform float uMatShininess;

// input from rasterizer
smooth in vec3 interpNormal;
smooth in vec3 interpPos;    // in eye space

// output to framebuffer
out vec4 fragColor;

float attenuate(float dist, float Q, float L, float C)
{
	return 1.0 / (Q * dist * dist + L * dist + C);
}

void main()
{
	vec3 accumColor = uAmbientLightColor * uMatDiffuseColor;

	// can remove these normalizations if we're absolutely sure that normals and light directions are unit vectors
	vec3 N = normalize(interpNormal);
	vec3 L = normalize(uLightPos - interpPos);		// direction to light

	// compute diffuse lighting intensity
	float NdotL = dot(N, L);

	if (NdotL > 0) {

		// distance to light
		float dist = length(uLightPos - interpPos);
		float attenuationFactor = attenuate(dist, uAttQuad, uAttLin, uAttConst);

		vec3 lightIntensity = attenuationFactor * uLightColor;

		accumColor += NdotL * lightIntensity * uMatDiffuseColor;

		vec3 toEye = normalize(-interpPos);
		vec3 H = normalize(toEye + L);
		float NdotH = dot(N, H);
		if (NdotH > 0) {
			float blinnTerm = pow(NdotH, uMatShininess);
			accumColor += blinnTerm * lightIntensity * uMatSpecularColor;
		}
	}

	fragColor = vec4(accumColor, 1.0);
}
*/
