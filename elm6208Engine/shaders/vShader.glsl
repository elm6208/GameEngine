#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;
layout (location = 3) uniform mat4 tran;
layout (location = 4) uniform mat4 cameraMat;

out vec2 fragUV;
out vec3 fragLoc;
out vec3 fragNormal;



void main()
{
	fragLoc = (tran * vec4(position, 1)).xyz;
	fragUV = uv;
	
	vec3 inv = normalize(transpose(inverse(tran)) * vec4(norm, 0)).xyz;
	//fragNormal = inv;
	fragNormal = norm;
	
	gl_Position = cameraMat * tran * vec4(position, 1);
	
	
}