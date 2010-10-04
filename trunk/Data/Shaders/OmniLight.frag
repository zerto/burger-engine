uniform float fWindowWidth;
uniform float fWindowHeight;

uniform sampler2D sNormalSampler;
uniform sampler2D sDepthSampler;

uniform mat4 mInvProj;
uniform int iDebug;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarInverseRadius;
varying float fVarMultiplier;

void main()
{
	vec4 finalColor;
	
	//we need the screen-space position of the fragment in order to fetch the GBuffer
	vec2 vTexCoord = vec2( gl_FragCoord.x / fWindowWidth, gl_FragCoord.y / fWindowHeight );

	//we need the view-space position of the vertex
	float fDepth = texture2D( sDepthSampler, vTexCoord ).r;
	
	// Construct screen-space position
	vec4 vClipPos = vec4( 2.0 * vTexCoord.x - 1.0, 2.0 * vTexCoord.y - 1.0, 2.0 * fDepth - 1.0, 1.0 );
	
	// Multiply by inverse projection matrix to get view-space position
	vec4 vViewSpaceVertex = mInvProj * vClipPos;
	vViewSpaceVertex = vViewSpaceVertex/vViewSpaceVertex.w;

	vec4 vNormalAndGloss = texture2D( sNormalSampler, vTexCoord );
	vNormalAndGloss.xyz = vNormalAndGloss.xyz * 2.0 - 1.0;	

	//Phong Lighting
	vec3 N = normalize( vNormalAndGloss.xyz );	
	vec3 E = normalize( -vViewSpaceVertex.xyz );


	vec3 vVertexToLight = vVarLightPos.xyz - vViewSpaceVertex.xyz;
						
	//attenuation
	//float fInverseRadius = fVarInverseRadius;				
	float fDistSqr = dot( vVertexToLight, vVertexToLight );
	float fAtt = clamp( 1.0 - fVarInverseRadius * sqrt(fDistSqr), 0.0, 1.0 );
	//fAtt *= fAtt;

	vec3 L = normalize( vVertexToLight );		
	float NDotLAtt = max( dot(N,L), 0.0 ) * fAtt;

	vec3 R = reflect(-L, N);

	vec3 diffuse = fVarMultiplier * NDotLAtt * vVarColor;

	float fSpecular = fVarMultiplier * pow( max( dot( R, E ), 0.0 ), vNormalAndGloss.a * 250.0 );
	float fSpecularLuminance = dot( fSpecular, vec3( 0.2126, 0.7152, 0.0722 ) );

	//storing diffuse and specular on different channels (rgb = diffuse, a = lum(spec) ) 
	finalColor += vec4(diffuse, fSpecularLuminance * NDotLAtt );

	gl_FragColor = finalColor;
}