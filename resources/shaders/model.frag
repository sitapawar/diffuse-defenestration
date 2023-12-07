#version 330 core
// Task 5: declare "in" variables for the world-space position and normal,
//         received post-interpolation from the vertex shader
in vec3 outWorldSpacePosition;
in vec3 outWorldSpaceNormal;
in vec2 outTexCoord;

// Task 10: declare an out vec4 for your output color
out vec4 fragColor;

uniform sampler2D textureSampler;

void main() {
    fragColor = vec4(1);
//    fragColor = texture(textureSampler, outTexCoord);
}
