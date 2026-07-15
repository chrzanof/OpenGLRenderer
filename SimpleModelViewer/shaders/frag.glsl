#version 330 core

in vec4 MVPosition;
in vec3 Normal;
in vec3 Tangent;
in vec3 LightPos;
in vec3 LightColor;
in vec2 TexCoord;
in vec4 LightViewPosition;

uniform sampler2D depthMap;
uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform sampler2D heightMap;
uniform bool useDiffuseTexture;
uniform bool useNormalMap;
uniform bool useHeightMap;

out vec4 FragColor;

float CalculatePCFShadow();
mat3 GetTBNMatrix();
vec2 CalcPOMTextCoords();
vec3 CalcBumpedNormal(vec2 texCoord);

void main()
{
	//Ambient
	float ambientStrength = 0.15;
	vec3 ambient = ambientStrength * LightColor;

	//diffuseTexture
	vec2 texCoord;
	if(textureSize(heightMap, 0) == vec2(1) || !useHeightMap) {
		texCoord = TexCoord;
	} else {
		texCoord = CalcPOMTextCoords();
		if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
			discard;
	}
	vec3 norm;
	if(textureSize(normalMap, 0) == vec2(1) || !useNormalMap) {
		norm = normalize(Normal);
	} else {
		norm = CalcBumpedNormal(texCoord);
	}
	
	vec3 lightDir = normalize(LightPos - vec3(MVPosition));
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * LightColor;

	//Specular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(-vec3(MVPosition));
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * LightColor;

	vec4 basicColor;
	
	if(textureSize(diffuseTexture, 0) == vec2(1) || !useDiffuseTexture)
	{
		basicColor = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		basicColor = texture(diffuseTexture, texCoord);
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

mat3 GetTBNMatrix()
{
	vec3 normal = normalize(Normal);
    vec3 tangent = normalize(Tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
	return mat3(tangent, bitangent, normal);
}

vec3 CalcBumpedNormal(vec2 texCoord)
{
	mat3 TBN = GetTBNMatrix();
    vec3 bumpMapNormal = texture(normalMap, texCoord).xyz;
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 newNormal;
    newNormal = TBN * bumpMapNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}

vec2 CalcPOMTextCoords()
{
	// TODO: go from simple parallax mapping to steep parallax mapping and then to parallax occlusion mapping
	float heightScale = 0.1;
	mat3 TBN = GetTBNMatrix();
	vec3 tangentViewDir = inverse(TBN) * normalize(-vec3(MVPosition));
	tangentViewDir = normalize(tangentViewDir);
	float height = texture(heightMap, TexCoord).r;
	vec2 p = (tangentViewDir * height * heightScale).xy;
	return TexCoord - p;
}
