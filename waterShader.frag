#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 FragNormal;
in vec3 FragPos;

uniform samplerCube skybox;
uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    vec3 I = normalize(FragPos - viewPos);
    
    vec3 reflection = reflect(I, normalize(FragNormal));
    vec3 refraction = refract(I, normalize(FragNormal), ratio);

    vec4 colorReflection = texture(skybox, reflection);
    vec4 colorRefraction = texture(skybox, refraction);

	vcolor = colorReflection * 0.5f + colorRefraction * 0.5f;
	
}
