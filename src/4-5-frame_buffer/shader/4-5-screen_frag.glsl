#version 450 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;  

void main()
{ 	
	// kernel
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );
	// sharpen kernel
	// float kernel[9] = float[](
	// 	-1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
	// );
	// blur kernel
	// float kernel[9] = float[](
    // 1.0 / 16, 2.0 / 16, 1.0 / 16,
    // 2.0 / 16, 4.0 / 16, 2.0 / 16,
    // 1.0 / 16, 2.0 / 16, 1.0 / 16  
	// );
	// edge-detection
	float kernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	);
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoord.st + offsets[i]));
    }
	vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
	
	vec3 texColor = texture(screenTexture, TexCoord).rgb;
	// 反相
	vec3 invColor = 1.0 - texColor;
	// 灰度
	float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;

	FragColor = vec4(col, 1.0f);
}
