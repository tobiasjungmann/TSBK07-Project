#version 150

struct Light {
	vec3 dirPos;
	vec3 color;
	bool directional;
};

struct MaterialLight {
	float diffuseness;
	float ambiantCoeff;
	float specularity;
	float specularExponent;
};


in vec3 posInViewCoordinates;
in vec3 normalInViewCoordinates;

uniform mat4 w2vMatrix;
uniform mat4 m2wMatrix;
uniform MaterialLight materialLight;
uniform Light lights[4];

out vec4 out_Color;

/************* GLOBALS **************/
vec3 lightsViewCoords[4];



// vector s in the formula, light pointing to contact point on surface, normalized
// FROM surface TO light for SPECULAR
// FOR DIFFUSE, consider taking the opposite, as we need from light to surface
vec3 getVectorS(int index, vec3 pos) {
	if (!lights[index].directional)
	 return normalize(pos - lightsViewCoords[index]);
	return normalize(-lightsViewCoords[index]);
}

vec3 computeSpecularLight(int index, vec3 camera, vec3 normal, vec3 pos) {
	vec3 sourceIntensity = lights[index].color; // i_s variable in formula
	vec3 lightDir = getVectorS(index, pos);
	vec3 viewDir = normalize(camera - pos); // from surface contact point to camera, direction towards the viewer
	vec3 reflectDir = normalize(reflect(lightDir, normal)); // mirror S relatively to NORMAL to get R.
	float cos_phi = max(0, dot(reflectDir, viewDir)); // angle PHI between R and toViewer
	return sourceIntensity * pow(cos_phi, materialLight.specularExponent); // we take max(0, cos) here cuz there is no such thing as negative light
}

vec3 computeDiffuseLight(int index, vec3 normal, vec3 pos) {
	vec3 sourceIntensity = lights[index].color;
	return sourceIntensity * max(0, dot(-getVectorS(index, pos), normal));
}

void main(void)
{
	for (int i =0; i < 4; i++) {
		if (!lights[i].directional)
			lightsViewCoords[i] = vec3(w2vMatrix * vec4(lights[i].dirPos, 1.0));
		else
			lightsViewCoords[i] = vec3(w2vMatrix * vec4(lights[i].dirPos, 0.0)); // no translation for directional
	}
	vec3 normalizedNormal = normalize(normalInViewCoordinates);
	vec3 camera = vec3(0.0, 0.0, 0.0);
	vec3 specularComponent = vec3(0,0,0);
	vec3 diffuseComponent = vec3(0,0,0);
	
	for (int i =0; i < 4; i++) {
		specularComponent += computeSpecularLight(i, camera, normalizedNormal, posInViewCoordinates);
		diffuseComponent += computeDiffuseLight(i, normalizedNormal, posInViewCoordinates);
	}


	vec3 shade = materialLight.diffuseness*diffuseComponent + materialLight.specularity*specularComponent; // assuming k_d = 1
	out_Color = vec4(shade, 1.0);
}
