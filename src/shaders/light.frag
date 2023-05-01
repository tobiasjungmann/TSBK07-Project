#version 150

out vec4 out_Color;

in vec3 posInViewCoordinates;
in vec3 normalInViewCoordinates;
// convert non directional light in their view coordinates, do not modify directional lights
vec3 lightInViewCoordinatesArr[4];

uniform mat4 w2vMatrix;
uniform mat4 m2wMatrix;

// TODO measure performance improvement by uploading all matrices at once.
// TODO compare performance between computing matrice product in shaders vs uploading an extra matrice for the result


// TODO compare result of matrix product in vertex shader vs in fragment shader, does the interpolation changes anything ?

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

struct Light {
	vec3 dirPos;
	vec3 color;
	bool directional;
	bool viewCoordinates;
};

// vector s in the formula, light pointing to contact point on surface, normalized
// FROM surface TO light for SPECULAR
// FOR DIFFUSE, consider taking the opposite, as we need from light to surface
vec3 getVectorS(int index, vec3 lightInViewCoordinatesArr[4], vec3 pos) {
	if (!isDirectional[index])
	 return normalize(pos - lightInViewCoordinatesArr[index]);
	return normalize(-lightInViewCoordinatesArr[index]);
}

vec3 computeSpecularLight(int index, vec3 camera, vec3 lightInViewCoordinatesArr[4], vec3 normal, vec3 pos) {
	float kspec = 1; // just assuming for now the material's coeff for specular lighting is 1.0
	vec3 sourceIntensity = lightSourcesColorArr[index]; // i_s variable in formula
	vec3 lightDir = getVectorS(index, lightInViewCoordinatesArr, pos);
	vec3 viewDir = normalize(camera - pos); // from surface contact point to camera, direction towards the viewer
	vec3 reflectDir = normalize(reflect(lightDir, normal)); // mirror S relatively to NORMAL to get R.
	float cos_phi = max(0, dot(reflectDir, viewDir)); // angle PHI between R and toViewer
	return kspec * sourceIntensity * pow(cos_phi, specularExponent); // we take max(0, cos) here cuz there is no such thing as negative light
}

vec3 computeDiffuseLight(int index, vec3 lightInViewCoordinatesArr[4], vec3 normal, vec3 pos) {
	vec3 sourceIntensity = lightSourcesColorArr[index];
	float kdiff = 1;
	return kdiff * sourceIntensity * max(0, dot(-getVectorS(index, lightInViewCoordinatesArr, pos), normal));
}

void main(void)
{
	vec3 lightInViewCoordinatesArr[4];
	for (int i =0; i < 4; i++) {
		if (!isDirectional[i])
			lightInViewCoordinatesArr[i] = vec3(w2vMatrix * vec4(lightSourcesDirPosArr[i], 1.0));
		else
			lightInViewCoordinatesArr[i] = vec3(w2vMatrix * vec4(lightSourcesDirPosArr[i], 0.0)); // no translation for directional
	}
	vec3 normalizedNormal = normalize(normalInViewCoordinates);
	vec3 camera = vec3(0.0, 0.0, 0.0);
	vec3 specularComponent = vec3(0,0,0);
	vec3 diffuseComponent = vec3(0,0,0);
	
	specularComponent += computeSpecularLight(0, camera, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	specularComponent += computeSpecularLight(1, camera, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	specularComponent += computeSpecularLight(2, camera, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	specularComponent += computeSpecularLight(3, camera, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	
	diffuseComponent += computeDiffuseLight(0, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	diffuseComponent += computeDiffuseLight(1, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	diffuseComponent += 0.5 * computeDiffuseLight(2, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);
	diffuseComponent += 0.5 * computeDiffuseLight(3, lightInViewCoordinatesArr, normalizedNormal, posInViewCoordinates);


	vec3 shade = 0.5*diffuseComponent + 0.8*specularComponent; // assuming k_d = 1
	out_Color = vec4(shade, 1.0);
}
