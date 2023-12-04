#version 330 core

// Task 16: Create a UV coordinate in variable
in vec2 fragUV;

// Task 8: Add a sampler2D uniform
uniform sampler2D textureUniform1;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool invert;
uniform bool blur;
uniform bool grey;
uniform bool sobel;
uniform bool sharpen;


uniform int fbo_width;
uniform int fbo_height;



out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    // Task 17: Set fragColor using the sampler2D at the UV coordinate
    fragColor = texture(textureUniform1, fragUV);

    // blur
    if (blur) {
        float uOffset = 1.f / fbo_width;
        float vOffset = 1.f / fbo_height;
        vec4 pixelColor = vec4(0.f);

            // 5x5 kernel goes from -2 through 3 from the center pixel x and y coords
         for (int x = -2; x < 3; x++) {
                for (int y = -2; y < 3; y++) {
                    pixelColor += texture(textureUniform1, vec2(fragUV[0] + uOffset * x, fragUV[1] + vOffset*y));
                }
            }
            fragColor = (1.f/25.f) * pixelColor;
        }
    if(grey){
        float grey = 0.299f * fragColor.r + 0.587f * fragColor.g + 0.114f * fragColor.b;
        fragColor=vec4(grey);
    }
    if(invert){
        fragColor = 1-fragColor;
    }
    if(sobel){
        // Sobel edge detection filter
                float sobelX = 0.0;
                float sobelY = 0.0;
                float uOffset = 1.f / fbo_width;
                float vOffset = 1.f / fbo_height;

                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        vec4 neighborColor = texture(textureUniform1, fragUV + vec2(uOffset * float(x), vOffset * float(y)));
                        float grayNeighbor = 0.299 * neighborColor.r + 0.587 * neighborColor.g + 0.114 * neighborColor.b;

                        // Sobel operator
                        sobelX += float(x) * grayNeighbor;
                        sobelY += float(y) * grayNeighbor;
                    }
                }

                float sobelMagnitude = length(vec2(sobelX, sobelY));
                fragColor = vec4(sobelMagnitude, sobelMagnitude, sobelMagnitude, 1.0);
            }
    if (sharpen) {
        float uOffset = 1.0 / fbo_width;
        float vOffset = 1.0 / fbo_height;
        vec4 originalColor = texture(textureUniform1, fragUV);

        // Apply the sharpening filter by subtracting a blurred version from the original
        vec4 blurredColor = vec4(0.0);

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                vec4 neighborColor = texture(textureUniform1, fragUV + vec2(uOffset * float(x), vOffset * float(y)));
                blurredColor += neighborColor;
            }
        }
        blurredColor /= 9.0;
        float strength = 1.5;

        fragColor = originalColor + strength * (originalColor - blurredColor);
    }


}
