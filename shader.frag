#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;



uniform sampler2D terrain1;
uniform sampler2D terrain2;
uniform sampler2D terrain3;
uniform sampler2D terrain4;


in vec2 FragTexCoords;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    //color = vec4(1.0f, 1.0f, 1.0f, sampleExtraOutput);
	color = texture(terrain1, FragTexCoords);
}
