//-------------------------
// - Sparky Game Engine -
// Renderer2D Line Shader
//-------------------------

#type vertex
#version 460 core

layout (location = 0) in vec3  a_Position; // Vertex position
layout (location = 1) in vec4  a_Color; // Vertex color
layout (location = 2) in int   a_EntityID; // Vertex Entity ID

out vec4     f_Color;
out flat int f_EntityID;

uniform mat4 u_ViewProjection;

void main()
{
	f_Color = a_Color;

	f_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 460 core

layout (location = 0) out vec4 o_Color;
layout (location = 1) out int o_EntityID;

in vec4     f_Color;
in flat int f_EntityID;

void main()
{
	// Apply Gamma correction
	float gamma = 2.2;
	vec4 finalColor = vec4(pow(f_Color.rgb, vec3(1.0 / gamma)), f_Color.a);

    // Set output color
    o_Color = finalColor;
	o_EntityID = f_EntityID;
}