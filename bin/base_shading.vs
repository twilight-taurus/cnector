#version 330

in vec3 vertexPosition;

in vec2 vertexTexCoord;

in vec4 vertexColor;

in vec3 vertexNormal;


out vec2 fragTexCoord;

out vec4 fragColor;

out vec3 fragNormal;

// Input uniform values
uniform mat4 mvp; // camera direction

uniform mat4 matModel;

uniform mat4 matNormal;



void main()
{

    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;

    fragColor = vertexColor;

    fragNormal = normalize( vec3(matNormal*vec4(vertexNormal, 1.0)) );

    // Calculate final vertex position
    gl_Position =  mvp*vec4(vertexPosition, 1.0);
}