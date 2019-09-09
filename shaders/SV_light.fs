#version 330 core

struct pointlight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D mPos;
uniform sampler2D mNor;

uniform pointlight pLight;

vec3 CalcPointLight( vec3 fNormal, vec3 fPosition)
{
    vec3 lightDir = normalize(pLight.position - fPosition);

    // Diffuse value
    float cosTheta = max( dot( fNormal, lightDir ), 0.0);

    // Combine results
    vec3 ambient = pLight.ambient;
    vec3 diffuse = pLight.diffuse * cosTheta;

return (ambient + diffuse);
}

void main()
{
	vec2 gScreenSize = vec2( 1024, 1024);
   	vec2 TexCoord = gl_FragCoord.xy / gScreenSize;

   	vec3 fPosition = texture(mPos, TexCoord).xyz;
   	vec3 fNormal = texture(mNor, TexCoord).xyz;
   	fNormal = normalize(fNormal);
	vec3 light = CalcPointLight( fNormal, fPosition);

   	gl_FragColor = vec4( light, 1.0);
}