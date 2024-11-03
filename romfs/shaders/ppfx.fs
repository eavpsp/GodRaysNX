#version 430
#extension GL_NV_gpu_shader5 : enable
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float blurFX;
uniform float bloomFX;
out vec4 finalColor;
// NOTE: Add here your custom variables
//BLOOM
const vec2 size = vec2(1280, 720);   // render size
const float samples = 0.9;          // pixels per axis; higher = bigger glow, worse performance
const float quality = 0.4;     

//BLUR
vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
vec3 weight = vec3(0.3333333333, 0.3962162162, 0.1702702703);


vec4 blur(float val)
{
  // Texel color fetching from texture sampler
    vec3 texelColor = texture(texture0, fragTexCoord).rgb*weight[0];

    for (int i = 1; i < 3; i++)
    {
        texelColor += texture(texture0, fragTexCoord + vec2(offset[i])/size.x, 0.0).rgb*weight[i];
        texelColor += texture(texture0, fragTexCoord - vec2(offset[i])/size.x, 0.0).rgb*weight[i];
    }

    return vec4(texelColor, val);
}


vec4 bloom(float val)
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/size*quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);

    const int range = 2;            // should be = (samples - 1)/2;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(texture0, fragTexCoord + vec2(x, y)*sizeFactor);
        }
    }

    vec4 col = ((sum/(samples*samples)) + source)*colDiffuse;
    col.a = val;
    return col;
}

void main()
{
   finalColor = blur(blurFX) + blur(bloomFX);
}
// NOTE: Add here your custom variables

// NOTE: Render size values must be passed from code
//const float renderWidth = 1280.0;SIZE
//const float renderHeight = 720.0;

