#version 430 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Normal;
layout (location = 3) in vec3 geo_Tangent;
layout (location = 4) in vec3 geo_Bitangent;

#include ":Shaders/Common/Buffers/EngineUBO.glsl"
#include ":Shaders/Common/IO/VertexOutput.glsl"

void main()
{
    vs_out.TBN = mat3
    (
        normalize(vec3(ubo_Model * vec4(geo_Tangent,   0.0))),
        normalize(vec3(ubo_Model * vec4(geo_Bitangent, 0.0))),
        normalize(vec3(ubo_Model * vec4(geo_Normal,    0.0)))
    );

    mat3 TBNi = transpose(vs_out.TBN);

    vs_out.FragPos          = vec3(ubo_Model * vec4(geo_Pos, 1.0));
    vs_out.Normal           = normalize(mat3(transpose(inverse(ubo_Model))) * geo_Normal);
    vs_out.TexCoords        = geo_TexCoords;
    vs_out.TangentViewPos   = TBNi * ubo_ViewPos;
    vs_out.TangentFragPos   = TBNi * vs_out.FragPos;

    gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
}
