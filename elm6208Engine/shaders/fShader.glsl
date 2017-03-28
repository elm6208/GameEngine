#version 430

//precision mediump float;
uniform sampler2D myTexture;

layout (location = 5) uniform vec3 lightLoc;
layout (location = 6) uniform vec3 cameraLoc;

in vec2 fragUV;
in vec3 fragLoc;
in vec3 fragNormal;

void main()
{
	vec3 fragN = normalize(fragNormal);
	vec3 L = normalize(lightLoc - fragLoc);
	vec3 E = normalize(cameraLoc - fragLoc);
	vec3 H = normalize(L + E);
	vec3 N = normalize(fragN);
	
	float ambient = 0.2;
	float diffuse = .8 * max(dot(L, N), 0);
	float specular =  .8 * pow(max(dot(H, N),0), 16);
	float brightness = ambient + diffuse + specular;
	
	
	vec4 col = texture(myTexture, fragUV);
	gl_FragColor = vec4(brightness * col.rgb, col.a);
	//gl_FragColor = vec4(fragN, 1);
	//gl_FragColor = vec4(lightLoc, 1);
}