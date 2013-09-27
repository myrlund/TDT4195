#version 130

// Input vertex data, different for all executions of this shader.
in vec3 vertex;

uniform mat4 MVP;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertex, 1);

}

