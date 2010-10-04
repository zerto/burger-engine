uniform sampler2D diffuseMap; // regular texture: texture unit 0
uniform sampler2D specStrength; // regular texture: texture unit 1
uniform sampler2D lightBuffer; // light from GBuffer: texture unit 6

void main()
{
	vec2 vTexCoord = gl_TexCoord[0].xy;
	//we need screen space coordinates in order to get sample from the GBuffer
	vec2 vScreenTexCoord = vec2( gl_FragCoord.x / 1280.0, gl_FragCoord.y / 720.0 );

	vec4 vLighting = texture2D( lightBuffer, vScreenTexCoord );
	vec4 vSpecStrength = texture2D( specStrength, vTexCoord );
	//reconstructing specular
	vec3 vChromacity = clamp( vLighting.rgb / ( dot( vLighting.rgb, vec3(0.2126, 0.7152, 0.0722 ) ) + 0.00001f ), 0.0, 1.0 );

	gl_FragColor = vec4( (vLighting.rgb + vChromacity * vLighting.a * vSpecStrength ) * texture2D( diffuseMap, vTexCoord ),1.0);
}