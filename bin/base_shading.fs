#version 330

//
// Basic diffuse lighting.
//

in vec2 fragTexCoord;

//in vec4 fragColor;

in vec3 fragNormal;

in vec3 fragPosition;

in vec3 fragLightPos;


uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Gets the light color from the main function
uniform vec4 lightColor;

uniform vec4 ambient;

//uniform vec3 viewPos;

uniform vec3 lightPos;

//uniform mat4 matModel;

//uniform mat4 matView;

//uniform mat4 matNormal;

//uniform mat4 mvp;

in float lightAmount;

out vec4 finalColor;

/*
    NOTE: Normals seem to be broken for some reason. (either matNormal, or the .obj file)

*/

void main()
{

//    finalColor = vec4(lightIntensity, 1.0);
    finalColor = /*Ld * Kd *  */ /*lightColor */ lightAmount * texture(texture0, fragTexCoord);
/*
    // position of light source
    vec4 lightPosition = vec4(lightPos, 0.0);
                                                // world position where light source is pointed at.
    vec3 lightDir = normalize( (lightPosition - vec4(0.0, 0.0, 0.0, 1.0)).xyz);

    vec3 fragNorm = normalize( vec3(fragNormal) );

    float NdotL = dot(fragNorm, -lightDir);

    vec4 diffuse_color = 2.5 * colDiffuse * NdotL;
    
//    finalColor = vec4(0.82, 0.82, 0.82, 1.0) + intensity * diffuse_color;
    finalColor = diffuse_color;
*/
}