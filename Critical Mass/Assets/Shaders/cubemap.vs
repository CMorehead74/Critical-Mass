#version 330

in vec4 vVertex;
in vec3 vNormal;

uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform mat4 mInverseCamera;

smooth out vec3 vVaryingTexCoord;

void main( void )
{
	vec3 vEyeNormal = normalMatrix * vNormal;
	
	vec4 vVert4 = mvMatrix * vVertex;
	vec3 vEyeVertex = normalize( vVert4.xyz / vVert4.w );
	
	vec4 vCoords = vec4( reflect( vEyeVertex, vEyeNormal ), 1.0 );
	
	vCoords = mInverseCamera * vCoords;
	vVaryingTexCoord.xyz = normalize( vCoords.xyz );
	
	gl_Position = mvpMatrix * vVertex;
}