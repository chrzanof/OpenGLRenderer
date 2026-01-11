#version 330 core

in vec4 MVPosition;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	//Ambient
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 1.0f;
	vec3 ambient = ambientStrength * lightColor;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - vec3(MVPosition));
	vec3 diffuse = max(dot(norm, lightDir), 0.0f) * lightColor;

	//Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(-vec3(MVPosition));
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 
	vec4 texColor = vec4(ambient + diffuse + specular, 1.0f) * texture(ourTexture, TexCoord);

	if(texColor.a < 0.1)
		discard;
	FragColor = texColor;
}