#version 330 core

in vec3 eye_position;
in vec3 eye_normals;
in vec4 light_position;

uniform sampler2D shadowMap;
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

float chebyshevUpperBound( float distance, vec2 myPos)
{
	vec2 moments = texture(shadowMap, myPos).rg;
	
	if (distance <= moments.x)
		return 1.0f;
//	if (distance > moments.x)
//		return 0.2f;
	float variance = moments.y - ( moments.x*moments.x );
	variance = min( max( variance, 0.00002 ), 1.0f);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);
	p_max = clamp(p_max, 0, 1);

return p_max;
}

void main()
{
	vec4 ls_Pos = light_position / light_position.w;

	vec3 n = normalize(eye_normals);
	vec3 light = CalcDirLight(n);
	float shadow = chebyshevUpperBound(ls_Pos.z, ls_Pos.xy);

	gl_FragColor = vec4( vec3(shadow), 1.0f ) * vec4(light, 1.0f);
//	gl_FragColor = vec4( vec3(light), 1.0f );
}