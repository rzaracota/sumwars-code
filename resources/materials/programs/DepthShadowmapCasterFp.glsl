// Diffuse texture
uniform sampler2D diffuseMap;

varying vec2 depth;

void main()
{
#if ALPHA
	vec2 uv = gl_TexCoord[0].st;

	float alpha = texture2D(diffuseMap, uv).a;
	if (alpha < 0.5)
	{
		discard;
	}
#endif

#if LINEAR_RANGE
	float finalDepth = depth.x;
#else
	float finalDepth = depth.x / depth.y;
#endif
	// just smear across all components
	// therefore this one needs high individual channel precision
	gl_FragColor = vec4(finalDepth, finalDepth, finalDepth, 1.0);
}

