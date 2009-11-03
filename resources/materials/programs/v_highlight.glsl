uniform float size_value;
uniform float time;
varying vec3 normal;

void main(void)
{
	normal = gl_NormalMatrix * gl_Normal;
   vec3 Pos = gl_Vertex.xyz +  (1.1*size_value * normal );

   gl_Position = gl_ModelViewProjectionMatrix * vec4(Pos,1.0)   ;

  
}
