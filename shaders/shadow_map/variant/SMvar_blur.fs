#version 330 core

uniform vec2 ScaleU;
uniform sampler2D shadowMap;

in vec2 texCo;

void main()
{
	vec4 color = vec4(0.0);

	color += texture2D( shadowMap, texCo + vec2( -3.0*ScaleU.x, -3.0*ScaleU.y ) )* 1 / 64;
	color += texture2D( shadowMap, texCo + vec2( -2.0*ScaleU.x, -2.0*ScaleU.y ) )* 6 / 64;
	color += texture2D( shadowMap, texCo + vec2( -1.0*ScaleU.x, -1.0*ScaleU.y ) )* 15/ 64;
	color += texture2D( shadowMap, texCo + vec2( 0.0 , 0.0) ) * 20 / 64;
	color += texture2D( shadowMap, texCo + vec2( 1.0*ScaleU.x,  1.0*ScaleU.y ) ) * 15/ 64;
	color += texture2D( shadowMap, texCo + vec2( 2.0*ScaleU.x,  2.0*ScaleU.y ) ) * 6 / 64;
	color += texture2D( shadowMap, texCo + vec2( 3.0*ScaleU.x, -3.0*ScaleU.y ) ) * 1 / 64;

	gl_FragColor = color;
}