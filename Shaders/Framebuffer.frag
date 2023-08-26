#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool HDR;

const float offset = 1.0 / 300.0;  



void main()
{


    float exposure=1.0f;
    const float gamma =2.2f;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
          0,   0,   0,
          0,   1,   0,
          0,   0,   0
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    vec3 hdrColor= texture(screenTexture, TexCoords).rgb;
    vec3 result = (vec3(1.0)-exp(-hdrColor*exposure));

    result = pow(result, vec3(1.0 / gamma));
    if(HDR){
    FragColor = vec4(result, 1.0);
    }
    else{
    FragColor = vec4(col, 1.0);
    }
}