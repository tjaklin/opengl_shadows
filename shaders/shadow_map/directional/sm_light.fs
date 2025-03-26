#version 330 core

in vec3 eye_position;
in vec3 eye_normals;
in vec4 light_position;

uniform sampler2D shadowMap;
uniform bool isPCF;
uniform vec3 lightDir;

uniform mat4 view;
uniform mat4 projection;

vec3 CalcDirLight(vec3 normal)
{
	vec3 diffuseVal = vec3( 0.8 );
	vec3 ambientVal = vec3( 0.2 );

	vec3 eye_lightDir = ( view * vec4( lightDir, 0.0f ) ).xyz;

	vec3 l = normalize(eye_lightDir);

	float cosTheta = max( dot(normal, l), 0.0 );

	vec3 diffuse = diffuseVal * cosTheta;
	vec3 ambient = ambientVal;

return ambient + diffuse;
}

float calculateShadow( bool isPCF )
{
		float bias = 0.0005f;
		vec3 l_pos = light_position.xyz;
		float currentDepth = l_pos.z - bias;

	if( !isPCF ){
		float isShadowed = 1.0f;
		float closestDepth = texture(shadowMap, l_pos.xy).r;

		if( currentDepth > closestDepth )
			isShadowed = 0.4f;
	return isShadowed;
	}
	else{
	float isShadowed = 0.0f;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		for(int x = -1; x <= 1; ++x)
		    for(int y = -1; y <= 1; ++y)
		    {
			float pcfDepth = texture(shadowMap, l_pos.xy + vec2(x, y) * texelSize).r;
			isShadowed += currentDepth - bias > pcfDepth ? 0.0 : 1.0;
		    }
		isShadowed /= 9.0f;
	return isShadowed;	// "puta" 0.4f
	}
}

void main()
{
	vec3 n = normalize(eye_normals);
	vec3 light = CalcDirLight(n);

	float shadow = calculateShadow( isPCF );
	gl_FragColor = vec4( vec3(shadow), 1.0f ) * vec4(light, 1.0f);
}