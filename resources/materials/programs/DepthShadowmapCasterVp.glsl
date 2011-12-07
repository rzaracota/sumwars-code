uniform mat4 worldViewProj;
uniform vec4 texelOffsets;

#if LINEAR_RANGE
uniform vec4 depthRange;
#endif

varying vec2 depth;

void main()
{
	// Pass-through default components
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
#if ALPHA
	gl_TexCoord[0] = gl_MultiTexCoord0;
#endif

	// fix pixel / texel alignment
	gl_Position.xy += texelOffsets.zw * gl_Position.w;
	// linear depth storage
	// offset / scale range output
#if LINEAR_RANGE
	depth.x = (gl_Position.z - depthRange.x) * depthRange.w;
#else
	depth.x = gl_Position.z;
#endif
	depth.y = gl_Position.w;
}

