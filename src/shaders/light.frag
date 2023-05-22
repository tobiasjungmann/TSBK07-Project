#version 150

struct Light {
    vec3 color;
    vec3 direction;
    vec3 position;
    bool directional;
    bool inView;
    
    float ambientK;
    float diffuseK;
    float specularK;
    float powerK;

    float attenuConst;
    float attenuLinear;
    float attenuSquare;
    bool attenuate;
    
    float cutOff;
    float outerEdgeCutOff;
    bool spotlight;
};

struct MaterialLight {
	float diffuseness;
	float ambientCoeff;
	float specularity;
	float specularExponent;
};


in vec3 posInViewCoordinates;
in vec3 normalInViewCoordinates;

uniform mat4 w2vMatrix;
uniform mat4 m2wMatrix;
uniform MaterialLight materialLight;
uniform Light lights[2];

uniform sampler2D texUnit;
uniform float texScalingF;

in vec2 texCoord;
out vec4 out_Color;

/************* GLOBALS **************/
vec3 lightsViewCoords[2];

vec3 positionInView(int index) {
    return lights[index].inView ? lights[index].position : vec3(w2vMatrix * vec4(lights[index].position, 1.0));
}

vec3 directionInView(int index) {
    // no translation for directional so zero out last component of vec4
    return vec3(w2vMatrix * vec4(lights[index].direction, 0.0));
}

float withinSpotlight(int index) {
    if (!lights[index].spotlight) return 1.0;
    // have both direction and position if spotlight
    // compute vectorS as in getVectorS
    vec3 fragToLight = normalize(posInViewCoordinates - positionInView(index));
    float spreading = dot(fragToLight, normalize(directionInView(index)));
    return clamp((spreading - lights[index].outerEdgeCutOff) / (lights[index].cutOff - lights[index].outerEdgeCutOff), 0.0, 1.0);
}

// vector s in the formula, light pointing to contact point on surface, normalized
// FROM surface TO light for SPECULAR
// FOR DIFFUSE, consider taking the opposite, as we need from light to surface
vec3 getVectorS(int index, vec3 pos) {
	if (!lights[index].directional)
	 return normalize(pos - lightsViewCoords[index]);
	return normalize(-lightsViewCoords[index]);
}

vec3 computeSpecularLight(int index, vec3 camera, vec3 normal, vec3 pos) {
	vec3 sourceIntensity = lights[index].specularK * lights[index].color; // i_s variable in formula
	vec3 lightDir = getVectorS(index, pos);
	vec3 viewDir = normalize(camera - pos); // from surface contact point to camera, direction towards the viewer
	vec3 reflectDir = normalize(reflect(lightDir, normal)); // mirror S relatively to NORMAL to get R.
	float cos_phi = max(0, dot(reflectDir, viewDir)); // angle PHI between R and toViewer
	return sourceIntensity * pow(cos_phi, materialLight.specularExponent); // we take max(0, cos) here cuz there is no such thing as negative light
}

vec3 computeDiffuseLight(int index, vec3 normal, vec3 pos) {
	vec3 sourceIntensity = lights[index].diffuseK * lights[index].color;
	return sourceIntensity * max(0, dot(-getVectorS(index, pos), normal));
}


float computeAttenuation(int index) {
    float attenuation = 1.0;
    Light light = lights[index];
    if (light.attenuate) {
        float dist = length(positionInView(index) - posInViewCoordinates);
        attenuation = 1.0 / (light.attenuConst + light.attenuLinear * dist + light.attenuSquare * dist * dist);
    }
    return attenuation;
}

void main(void)
{
	vec3 camera = vec3(0.0, 0.0, 0.0);

	for (int i =0; i < 2; i++) {
        lightsViewCoords[i] = lights[i].directional ? directionInView(i) : positionInView(i);
    }
	vec3 normalizedNormal = normalize(normalInViewCoordinates);

    vec3 ambientComponent = vec3(0);
	vec3 specularComponent = vec3(0,0,0);
	vec3 diffuseComponent = vec3(0,0,0);
	
	for (int i =0; i < 2; i++) {
        ambientComponent += lights[i].color * lights[i].ambientK;
        float spot = withinSpotlight(i);
        if (spot > 0) {
            float attenuation = spot * computeAttenuation(i);
	    	diffuseComponent += lights[i].powerK * attenuation * computeDiffuseLight(i, normalizedNormal, posInViewCoordinates);
    		specularComponent += lights[i].powerK * attenuation * computeSpecularLight(i, camera, normalizedNormal, posInViewCoordinates); 
        }
	}

    specularComponent *= materialLight.specularity;
    diffuseComponent *= materialLight.diffuseness;
    ambientComponent *= materialLight.ambientCoeff; 

	vec3 shade = ambientComponent + diffuseComponent + specularComponent;
    shade.r = clamp(shade.r, 0.0, 1.0);
    shade.g = clamp(shade.g, 0.0, 1.0);
    shade.b = clamp(shade.b, 0.0, 1.0);
	out_Color = vec4(shade, 1.0);
    out_Color *= texture(texUnit, texScalingF*texCoord);
}
