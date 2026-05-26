#version 330 core

in vec4 MVPosition;
in vec3 Normal;
in vec3 LightPos;
in vec3 LightColor;
in vec2 TexCoord;
in vec4 LightViewPosition;

uniform sampler2D depthMap;
uniform sampler2D diffuseTexture;

out vec4 FragColor;

float CalculatePCFShadow();

void main()
{
	//Ambient
	float ambientStrength = 0.15;
	vec3 ambient = ambientStrength * LightColor;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - vec3(MVPosition));
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * LightColor;

	//Specular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(-vec3(MVPosition));
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * LightColor;

	vec4 basicColor;
	
	if(textureSize(diffuseTexture, 0) == vec2(1))
	{
		basicColor = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		basicColor = texture(diffuseTexture, TexCoord);
	}

	float shadow = CalculatePCFShadow();

	vec4 texColor = vec4(ambient + (1.0 - shadow) + diffuse + specular, 1.0f) * basicColor; 

	if(texColor.a < 0.1)
		discard;
	FragColor = texColor;
}

float CalculatePCFShadow()
{
	vec2 texelSize = vec2(1.0) / 1024.0;
	vec3 p = LightViewPosition.xyz / LightViewPosition.w * 0.5 + 0.5;
	float bias = 0.0005;
	float shadowSum = 0.0;
	
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			float shadow = texture(depthMap, p.xy + vec2(i, j) * texelSize).r < p.z - bias ? 1.0 : 0.0;
			if(p.z > 1.0) shadow = 0;
			shadowSum += shadow;
		}
	}
	return shadowSum / 9.0;
}