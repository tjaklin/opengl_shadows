#version 330 core

struct pointlight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
};

in vec3 world_position;
in vec3 eye_position;
in vec3 eye_normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float far_plane;

uniform samplerCube shadowMap;
uniform bool isPCF;
uniform pointlight pLight;

vec3 CalcPointLight(vec3 normal)
{
    vec3 eye_lightPos = (view * vec4(pLight.position, 1.0f)).xyz;

    vec3 lightDir = normalize(eye_lightPos - eye_position);

    float cosTheta = max(dot(normal, lightDir), 0.0);

    vec3 ambient = pLight.ambient;
    vec3 diffuse = pLight.diffuse * cosTheta;

return ambient + diffuse;
}


// SHADOW MAPS
float calculateShadow( bool isPCF ){
	float bias = 0.05f;

	if( isPCF ){
		float isShadowed = 1.0f;
	vec3 fragToLight = world_position - pLight.position;
	float currentDepth = length(fragToLight);

	float closestDepth = texture(shadowMap, fragToLight).r;
	closestDepth *= far_plane;
	isShadowed = currentDepth -  bias > closestDepth ? 0.4 : 1.0;

	return isShadowed;
	}
	else{
		float isShadowed = 0.0;
	float biasFIX = 0.5f;
	float samples = 4.0;
	float offset = 0.1;
	vec3 fragToLight = world_position - pLight.position;
	float currentDepth = length(fragToLight);

	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	    for(float y = -offset; y < offset; y += offset / (samples * 0.5))
		for(float z = -offset; z < offset; z += offset / (samples * 0.5))
		{
		    float closestDepth = texture(shadowMap, fragToLight + vec3(x, y, z)).r;
		    closestDepth *= far_plane;   // Undo mapping [0;1]
		    if(currentDepth - bias > closestDepth)
		        isShadowed += 0.4;
		}

	isShadowed /= (samples * samples * samples);


	return 1-isShadowed;
	}
}

void main()
{
	vec3 result = vec3(0.0f);
	float shadow = 1.0f;

	vec3 n = normalize( eye_normals );

	result = CalcPointLight(n);

	shadow = calculateShadow( isPCF );

	result *= shadow;
	gl_FragColor = vec4(result, 1.0);
}