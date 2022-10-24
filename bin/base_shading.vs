#version 330

//
// Basic diffuse lighting.
//

in vec3 vertexPosition;

in vec2 vertexTexCoord;

in vec4 vertexColor;

in vec3 vertexNormal;


out vec2 fragTexCoord;

//out vec4 fragColor;

out vec3 fragNormal;

out vec3 fragPosition;

out vec3 fragLightPos;

out vec3 camLightDir;

// Input uniform values
uniform mat4 mvp; // camera direction

uniform mat4 matModel;

uniform mat4 matNormal;

uniform mat4 matView;

uniform vec3 lightPos;

/*
    NOTE: Normals seem to be broken for some reason. (either matNormal, or the .obj file)

*/

// dependent on the angle that the light hits the surface.
out float lightAmount;

void main()
{
    fragTexCoord = vertexTexCoord;

    // get normal matrix by inverting and transposing model matrix
    mat4 normalMatrix = transpose( inverse(matModel * matView) );
    
    vec3 tnorm = vec3( normalize(normalMatrix * vec4(vertexNormal, 0.0)) );
//    fragNormal = vec3( normalMatrix * vec4(vertexPosition, 0.0) );

    // Convert normal and position to eye coords
//    vec3 tnorm = vec3( normalize( matNormal * vec4(vertexNormal, 0.0) ) );

    // position of the vertex in camera/view space. (camera pos. is origin)
    vec4 eyeCoords = matModel * matView * vec4(vertexPosition, 1.0);

    // lightPos is already in world space.
    vec4 lightPosView = matView * vec4(lightPos, 0.0);

    vec3 s = vec3( normalize( lightPosView - eyeCoords ) );

    // The diffuse shading equation
//    lightAmount = /*Ld * Kd **/ max( dot( s, tnorm ), 0.0 );
//    lightAmount = /*Ld * Kd **/ max( dot( s, tnorm ), 0.0 );
    lightAmount = /*Ld * Kd **/ min( dot( s, tnorm ) );
//    fragPosition =  max( dot( s, tnorm ), 0.0 );

    // Convert position to clip coordinates and pass along 

    gl_Position = mvp * vec4(vertexPosition,1.0);

/*
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // get normal matrix by inverting and transposing model matrix
//    mat4 normalMatrix = transpose( inverse(matModel) );
    // altermative: use matNormal
    fragNormal = vec3( matNormal * vec4(vertexPosition, 0.0) );
//    fragNormal = vec3( normalMatrix * vec4(vertexPosition, 0.0) );
    // Calculate vertex position on screen
    gl_Position =  mvp*vec4(vertexPosition, 1.0);

//    fragPosition = vec3(gl_Position);
    fragPosition = vertexPosition;
*/
}