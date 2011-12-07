

// Diffuse texture
uniform sampler2D diffuseMap;


void main()
{
	// Exit if fully in fog
//	if (fog == 0.0)
//	{
//		gl_FragColor = gl_Fog.color;
//		return;
//	}

	vec4 diffuseColour = texture2D(diffuseMap, gl_TexCoord[0].st);


	float z = gl_FragCoord.z / gl_FragCoord.w;

#if FOG_LINEAR
	float fog = clamp((gl_Fog.end - z) * gl_Fog.scale, 0.0, 1.0);
#endif
#if FOG_EXP
	float fog = clamp(exp(- gl_Fog.density * .z), 0.0, 1.0);
#endif
#if FOG_EXP2
	float fog = clamp(exp(- gl_Fog.density * gl_Fog.density * z * z), 0.0, 1.0);
#endif

	vec3 colour = vec3(gl_LightModel.ambient * diffuseColour + diffuseColour);

	gl_FragColor.rgb = mix(gl_Fog.color.rgb, colour, fog);
	gl_FragColor.a = max(diffuseColour.a, 1.0 - fog);

}
