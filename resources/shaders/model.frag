#version 330 core
// Task 5: declare "in" variables for the world-space position and normal,
//         received post-interpolation from the vertex shader
in vec3 outWorldSpacePosition;
in vec3 outWorldSpaceNormal;
in vec2 outTexCoord;

// Task 10: declare an out vec4 for your output color
out vec4 fragColor;

uniform sampler2D textureSampler;

// phong uniforms
uniform float k_a;
uniform float k_d;
uniform float k_s;
uniform float shininess;
uniform vec4 worldCamPos;
uniform vec4 c_a;
uniform vec4 c_d;
uniform vec4 c_s;
// Lights
uniform vec4 lightIntensities[8];
uniform vec3 lightDirs[8]; // TO lights FROM point
uniform vec3 lightPositions[8];
uniform float lightPenumbras[8];
uniform int lightTypes[8];
uniform vec3 lightFunctions[8];
uniform float lightAngles[8];
uniform int lightCount;

float computeAttenuation(float dist, vec3 func) {
    return min(1.0, 1.0 / (func[0] + func[1]*dist + func[2]*dist*dist));
}

void main() {
    fragColor = vec4(1);
    vec4 texColor = texture(textureSampler, outTexCoord);
    vec4 c_a = texColor;
    vec4 c_d = texColor;
    vec4 c_s = texColor;

    //ambient term
    fragColor = k_a * c_a;

    vec3 dirToCam = worldCamPos.xyz - outWorldSpacePosition;
    // loop over lights
    for (int i = 0; i < lightCount; i++) {
        vec3 L; //normalized direction from the intersection point to light i
        float fatt = 1.f; //attenuation factor
        float distance;
        float falloff = 1.f; //set to one for no fall off (full light)

        if (lightTypes[i] == 1) { //directional light
            L = -lightDirs[i];
        }else if (lightTypes[i] == 2){ //point light
            L = normalize(lightPositions[i] - outWorldSpacePosition);
            // Calculate attenuation factor
            distance = length(lightPositions[i] - outWorldSpacePosition);
            fatt = min(1.f, 1.f/(lightFunctions[i][0] + distance*lightFunctions[i][1] + distance*distance*lightFunctions[i][2]));
        }
        else if (lightTypes[i] == 3){ //spot light
            L = normalize(vec3(lightPositions[i]) - outWorldSpacePosition);

            //calculaye attenuation factor
            distance = length(lightPositions[i] - outWorldSpacePosition);
            fatt = min(1.f, 1.f/(lightFunctions[i][0] + distance*lightFunctions[i][1] + distance*distance*lightFunctions[i][2]));

            //calculate fall off
            //x = angle between current direction and spotlight direction
            float x = acos(dot(lightDirs[i], -L) / (length(lightDirs[i]) * length(L)));
            float outer = lightAngles[i];
            float inner = outer - lightPenumbras[i];
            if (x <= inner) {
                falloff = 1.f; // full intensity of the light
            } else if (x <= outer) {
                float a = (x - inner) / (outer - inner);
                float fallFunc = -2.f*a*a*a + 3.f*a*a;
                falloff = 1.f - fallFunc;
            } else {
                falloff = 0;
            }
        } else{
            continue;
        }

        //diffuse
        vec3 lightDirection =normalize(L); //surface to light direction
        vec3 normalizedNormal = normalize(outWorldSpaceNormal);
        float diffuseIntensity = max(0, dot(normalize(outWorldSpaceNormal), lightDirection));
        vec3 diffuseColor = k_d * diffuseIntensity * vec3(c_d)*vec3(lightIntensities[i]);
        fragColor.rgb += diffuseColor*fatt*falloff;

        // specular
        if(shininess >0){
        vec3 viewDirection = normalize(worldCamPos.xyz - outWorldSpacePosition);
        vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);
        float specularIntensity = pow(max(0.0, dot(viewDirection, reflectDirection)), shininess);
        vec3 specularColor = k_s * specularIntensity * vec3(1,1,1)*vec3(lightIntensities[i]);
        fragColor.rgb += specularColor*fatt*falloff;
        }
    }
}
