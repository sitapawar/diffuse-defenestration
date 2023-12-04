#version 330 core
layout(location = 0) in vec3 ObjectSpacePosition;
layout(location = 1) in vec3 ObjectSpaceNormal;  // Assuming you have a normal attribute

// Task 5: declare `out` variables for the world-space position and normal,
//         to be passed to the fragment shader
out vec3 outWorldSpacePosition;
out vec3 outWorldSpaceNormal;

// Task 6: declare a uniform mat4 to store model matrix
uniform mat4 modelMatrix;

// Task 7: declare uniform mat4's for the view and projection matrix
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
    // Task 8: compute the world-space position and normal, then pass them to
    //         the fragment shader using the variables created in task 5

    // Transform object-space position to world-space position using the model matrix
    vec4 worldSpacePosition = modelMatrix * vec4(ObjectSpacePosition, 1.0);

    // Transform object-space normal to world-space normal using the normal matrix
//    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 worldSpaceNormal = normalize(mat3(modelMatrix)* ObjectSpaceNormal);

    // Assign values to the out variables
    outWorldSpacePosition = worldSpacePosition.xyz;
    outWorldSpaceNormal = worldSpaceNormal;

    // Task 9: set gl_Position to the object space position transformed to clip space
    gl_Position = projMatrix * viewMatrix * worldSpacePosition;
}
