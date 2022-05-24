#version 330

// Phong shading matModel. //

// Input vertex attributes
in vec3 vertexPosition;

in vec2 vertexTexCoord;

in vec3 vertexNormal;

in vec4 vertexColor;


// Output vertex attributes (to fragment shader)
out vec3 fragPos;
// Outputs the texture coordinates to the Fragment Shader
out vec2 fragTexCoord;
// Outputs the fragColor for the Fragment Shader
out vec4 fragColor;
// Outputs the fragNormal for the Fragment Shader
out vec3 fragNormal;

/*
// Imports the camera matrix from the main function (matModel View Projection)
uniform mat4 mvp;
// Imports the matModel matrix from the main function
uniform mat4 matModel;
*/

// Input uniform values
uniform mat4 mvp; // camera direction

uniform mat4 matModel;

uniform mat4 matNormal;


void main()
{
	// calculates current position
//	fragPos = vec3(matModel * vec4(vertexPosition, 1.0));
	fragPos = vertexPosition;

	// Assigns the fragColors from the Vertex Data to "fragColor"
	fragColor = vertexColor;
	// Assigns the texture coordinates from the Vertex Data to "fragTexCoord"
	fragTexCoord = vertexTexCoord;

	// Assigns the fragNormal from the Vertex Data to "fragNormal"
//	fragNormal = normalize( vec3(matNormal*vec4(vertexNormal, 1.0)) ); // NOTE: check if matNormal valid
//	fragNormal = vec3(matNormal*vec4(vertexNormal, 1.0)); // NOTE: check if matNormal valid
	fragNormal = vertexNormal;

	// Outputs the positions/coordinates of all vertices
	gl_Position = mvp*vec4(vertexPosition, 1.0);

}