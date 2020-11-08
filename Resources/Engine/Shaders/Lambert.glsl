#shader vertex
#version 430 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Normal;

layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
    float   ubo_Time;
};

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    vs_out.FragPos      = vec3(ubo_Model * vec4(geo_Pos, 1.0));
    vs_out.Normal       = normalize(mat3(transpose(inverse(ubo_Model))) * geo_Normal);
    vs_out.TexCoords    = geo_TexCoords;

    gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
#version 430 core

out vec4 FRAGMENT_COLOR;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec4        u_Diffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D   u_DiffuseMap;
uniform vec2        u_TextureTiling = vec2(1.0, 1.0);
uniform vec2        u_TextureOffset = vec2(0.0, 0.0);

const vec3 c_lightPosition    = vec3(-9000.0, 10000.0, 11000.0);
const vec3 c_lightDiffuse     = vec3(1.0, 1.0, 1.0);
const vec3 c_lightAmbient     = vec3(0.3, 0.3, 0.3);

vec3 Lambert(vec3 p_fragPos, vec3 p_normal)
{
    const float diffuse = max(dot(p_normal, normalize(c_lightPosition - p_fragPos)), 0.0);
    return clamp(c_lightDiffuse * diffuse + c_lightAmbient, 0.0, 1.0);
}

void main()
{
    const vec4 diffuse = texture(u_DiffuseMap, u_TextureOffset + vec2(mod(fs_in.TexCoords.x * u_TextureTiling.x, 1), mod(fs_in.TexCoords.y * u_TextureTiling.y, 1))) * u_Diffuse;
    FRAGMENT_COLOR = vec4(Lambert(fs_in.FragPos, fs_in.Normal) * diffuse.rgb, diffuse.a);
}