#version 330

// Phong shading model. //

// Imports the fragColor from the Vertex Shader
in vec4 fragColor;
// Imports the texture coordinates from the Vertex Shader
in vec2 fragTexCoord;
// Imports the fragNormal from the Vertex Shader
in vec3 fragNormal;

// Imports the current position from the Vertex Shader
in vec3 fragPos;

//
//  TODO: customize shader processing for the uniforms below.
// --> everything else can be left as is in the game engine code.
//

/*
    fragment shader uniforms handled by Raylib
    ( need to have same name as designated shader uniform names )
*/

// Gets the Texture Unit from the main function
uniform sampler2D texture0;
// Gets the fragColor of the light from the main function
uniform vec4 colDiffuse;

/*
    fragment shader uniforms handled by our program.
    ( We can name them freely here)
*/

// Gets the position of the light from the main function
uniform vec3 lightPos;

uniform vec4 lightColor;

uniform vec4 ambient;
uniform vec3 viewPos;
// Gets the position of the camera from the main function

// Outputs fragColors in RGBA
out vec4 finalColor;


void main()
{
	// ambient lighting
//	float ambients = 0.3;

	// texture point
//	vec4 texelColor = texture(texture0, fragTexCoord);

    //Calculations on light position relative to the fragment's position
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize( lightPos - fragPos );

    // TODO: something wrong with diffuse < possibly something wrong with the normals.
//	float diffuse = max( dot(normal, lightDirection), 0.0 );

    // Calculating on the viewer's position to the fragment's position for specular lighting
    vec3 viewDirection = normalize( viewPos - fragPos );
    vec3 reflectionDirection = vec3(0.0);
    reflectionDirection = reflect(-lightDirection, normal); // calculate reflection dir for spection

    // spec lighting intensity
    float specIntensity = 0.5;
    float specFactor = pow( max(dot(viewDirection, reflectionDirection), 0.0), 16 );
    vec3 specularLight = specIntensity * specFactor * vec3(lightColor);

    // calculate intensity of diffuse lighting
    float diffuseFactor = max( dot(normal, lightDirection), 0.0 );
    vec3 diffuseLight = vec3(lightColor)*diffuseFactor;

    // ambient lighting
    float ambientIntensity = 0.3;
    vec3 ambientLight = ambientIntensity * vec3(lightColor);

//    if (diffuse > 0.0) specCo = pow( max(dot(viewDirection, reflectionDirection), 0.0), 1.0 ); // 16 refers to shine
//	specular += specCo;

//    finalColor = texelColor * ( colDiffuse + vec4(specular, 1.0) + vec4(lightDot, 1.0) );

    finalColor = vec4( (ambientLight + diffuseLight + specularLight) * vec3(texture(texture0, fragTexCoord)), 1.0);
//    finalColor += texelColor * (ambients/1.0) * fragColor;

    // Gamma correction
//    finalColor = pow(finalColor, vec4(1.0/2.2));

}