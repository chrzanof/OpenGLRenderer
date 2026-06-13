#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

  
uniform mat4 projection;

const float bias = 0.0001;

void main()
{
    gl_Position = projection * (gl_in[0].gl_Position + vec4(gs_in[0].normal, 0.0) * bias);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position + vec4(gs_in[1].normal, 0.0) * bias);
    EmitVertex();
    gl_Position = projection * (gl_in[2].gl_Position + vec4(gs_in[2].normal, 0.0) * bias);
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(gs_in[0].normal, 0.0) * bias);
    EmitVertex();
    EndPrimitive();
}  