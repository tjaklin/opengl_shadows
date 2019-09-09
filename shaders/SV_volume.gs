#version 330

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 18) out;

in vec3 world_position[];

uniform vec3 gLightPos;
uniform mat4 view;
uniform mat4 projection;

float faktor = 0.001;

void ExtrudeVol(vec3 StartVertex, vec3 EndVertex)
{    // izvuci pravokutnik iz dviju primljenih vektora pozicija
     // prema tim vektorima "produzenim" u beskonacnost
    vec3 LightDir = normalize(StartVertex - gLightPos); 
    gl_Position = projection * view * vec4((StartVertex + LightDir * faktor), 1.0);
    EmitVertex();
 
    gl_Position = projection * view * vec4(LightDir, 0.0);
    EmitVertex();
    
    LightDir = normalize(EndVertex - gLightPos);
    gl_Position = projection * view * vec4((EndVertex + LightDir * faktor), 1.0);
    EmitVertex();
    
    gl_Position = projection * view * vec4(LightDir , 0.0);
    EmitVertex();

    EndPrimitive();            
}

void main()
{	//ex = rubovi
    vec3 e1 = world_position[2] - world_position[0];
    vec3 e2 = world_position[4] - world_position[0];
    vec3 e3 = world_position[1] - world_position[0];
    vec3 e4 = world_position[3] - world_position[2];
    vec3 e5 = world_position[4] - world_position[2];
    vec3 e6 = world_position[5] - world_position[0];

    vec3 Normal = normalize(cross(e1,e2));
    vec3 LightDir = normalize(gLightPos - world_position[0]);

	// pronadi obris kocke koji se vidi iz pozicije svijetla
    if (dot(Normal, LightDir) > 0) {

        Normal = cross(e3,e1);

        if (dot(Normal, LightDir) <= 0) {
            vec3 StartVertex = world_position[0];
            vec3 EndVertex = world_position[2];
            ExtrudeVol(StartVertex, EndVertex);
        }

        Normal = cross(e4,e5);
        LightDir = gLightPos - world_position[2];

        if (dot(Normal, LightDir) <= 0) {
            vec3 StartVertex = world_position[2];
            vec3 EndVertex = world_position[4];
            ExtrudeVol(StartVertex, EndVertex);
        }

        Normal = cross(e2,e6);
        LightDir = gLightPos - world_position[4];

        if (dot(Normal, LightDir) <= 0) {
            vec3 StartVertex = world_position[4];
            vec3 EndVertex = world_position[0];
            ExtrudeVol(StartVertex, EndVertex);
        }

	// zatvori prednju stranu volumena
        LightDir = (normalize(world_position[0] - gLightPos));
        gl_Position = projection * view * vec4((world_position[0] + LightDir * faktor), 1.0);
        EmitVertex();

        LightDir = (normalize(world_position[2] - gLightPos));
        gl_Position = projection * view * vec4((world_position[2] + LightDir * faktor), 1.0);
        EmitVertex();

        LightDir = (normalize(world_position[4] - gLightPos));
        gl_Position = projection * view * vec4((world_position[4] + LightDir * faktor), 1.0);
        EmitVertex();
        EndPrimitive();
 
        // zatvori straznju stranu volumena
        LightDir = world_position[0] - gLightPos;
        gl_Position = projection * view * vec4(LightDir, 0.0);
        EmitVertex();

        LightDir = world_position[4] - gLightPos;
        gl_Position = projection * view * vec4(LightDir, 0.0);
        EmitVertex();

        LightDir = world_position[2] - gLightPos;
        gl_Position = projection * view * vec4(LightDir, 0.0);
        EmitVertex();

        EndPrimitive();
    }
}