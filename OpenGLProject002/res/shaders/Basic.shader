#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

out vec2 v_texCoord;
out vec3 v_normals;
out vec3 v_fragPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
	gl_Position = u_proj * u_view * u_model * position;
	v_fragPos = vec3(u_model * position);
	v_texCoord = texCoord;
	v_normals = mat3(transpose(inverse(u_model))) * normals;
};

#shader fragment
#version 330 core

float ambientStrength = 0.2;

out vec4 FragColor;
uniform sampler2D u_texture;
uniform vec3 u_lightPos;

in vec2 v_texCoord;
in vec3 v_normals;
in vec3 v_fragPos;

void main() {
	vec3 norm = normalize(v_normals);
	vec3 lightDir = normalize(u_lightPos - v_fragPos);

	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diff * vec3(1.0f);

	vec4 texColor = texture(u_texture, v_texCoord);
	FragColor = texColor * vec4(vec3(ambientStrength) + diffuse, 1.0f);
};