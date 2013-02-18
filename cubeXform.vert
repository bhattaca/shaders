attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute vec2 texCoord;
attribute vec4 tangentCoord;


varying vec2 FtexCoord;

uniform mat4 mvp;
uniform vec3 LightPosition_worldspace;
uniform mat4 model;
uniform mat4 view;
uniform int use_texture;
uniform mat4 bias;


varying vec4 pcolor;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec3 Position_worldspace;
varying vec4 vertexPosition_cameraspace2;
varying vec3 R;
varying vec4 pnormal;

uniform mat4 depthMVP;

varying vec4 ShadowCoord;

//// bump mapping 
varying vec3 lightVec;
varying vec3 eyeVec;
void main()
{
/*
      pcolor = color;

//    gl_Position = gl_ModelViewProjectionMatrix * position;
//
//    replace the defulat matrix by the matrix passed from our opengl program

    gl_Position = mvp * position;

    Position_worldspace = (model * position).xyz;

    vec3 vertexPosition_cameraspace = (view*model*position).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    vec3 LightPosition_cameraspace = ( view * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	
    //FtexCoord = position;
    FtexCoord = texCoord;
    // compute reflection vector
    Normal_cameraspace = normalize(gl_NormalMatrix * normal.xyz);
  vec3 LightColor = vec3(1,1,1);
        float LightPower = 100.0;
        vec3 MaterialDiffuseColor= pcolor.xyz;
        vec3 MaterialAmbiantColor = vec3(0.5,0.5,0.5) * MaterialDiffuseColor;
        vec3 MaterialSpecularColor = vec3(0.8,0.8,0.8);

        float distance = length( LightPosition_worldspace - Position_worldspace );

        vec3 n = normalize( Normal_cameraspace );
        vec3 l = normalize( LightDirection_cameraspace );

        float cosTheta = clamp( dot( n,l ), 0.0,1.0 );

        vec3 E = normalize(EyeDirection_cameraspace);
        vec3 R = reflect(-l,n);

        float cosAlpha = clamp( dot( E,R ), 0.0,1.0 );

     
    
    if ( use_texture == 1){
   		ShadowCoord.xyzw=bias*mvp*position;
		gl_FrontColor = gl_Color;
    }
*/
/*
	//// enviorment mapping and simple texture
	
 	pcolor = color;
    gl_Position = mvp * position;
    Position_worldspace = (model * position).xyz;
    vec3 vertexPosition_cameraspace = (view*model*position).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    vec3 LightPosition_cameraspace = ( view * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    //FtexCoord = position;
    FtexCoord = texCoord;
    // compute reflection vector
    Normal_cameraspace = normalize(gl_NormalMatrix * normal.xyz);
    R = reflect(EyeDirection_cameraspace.xyz, Normal_cameraspace);
    gl_TexCoord[0] = gl_TextureMatrix[0] * vec4 (R, 1.0);
	
	*/  
	  /*
	///// bump mapping 
	 gl_Position = mvp * position;
	Normal_cameraspace = normalize(gl_NormalMatrix * normal.xyz);
	vec3 vertexPosition_cameraspace = (view*model*position).xyz;
	vec3 n = normalize(gl_NormalMatrix * normal.xyz);
	vec3 t = normalize(model * tangentCoord).xyz;
	vec3 b = normalize(cross(n, t));
	vec3 LightPosition_cameraspace = ( view * vec4(LightPosition_worldspace,1)).xyz;
	vec3 tmpVec=LightPosition_cameraspace - vertexPosition_cameraspace;
	lightVec.x = dot(tmpVec, t);
	lightVec.y = dot(tmpVec, b);
	lightVec.z = dot(tmpVec, n);
	tmpVec = -vertexPosition_cameraspace;
	eyeVec.x = dot(tmpVec, t);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, n);
	FtexCoord = texCoord;
	*/
	

	/////////// geometry shader 
     	gl_Position = position;
     	pnormal = normal;
     	pcolor = vec4(1.0,1.0,0.0,1.0);
 		//FtexCoord = texCoord;
 		Normal_cameraspace = normalize(gl_NormalMatrix * normal.xyz);
		vec3 vertexPosition_cameraspace = (view*model*position).xyz;
 		vec3 n = normalize(gl_NormalMatrix * normal.xyz);
		vec3 t = normalize(model * tangentCoord).xyz;
		vec3 b = normalize(cross(n, t));
		vec3 LightPosition_cameraspace = ( view * vec4(LightPosition_worldspace,1)).xyz;
		vec3 tmpVec=LightPosition_cameraspace - vertexPosition_cameraspace;
		lightVec.x = dot(tmpVec, t);
		lightVec.y = dot(tmpVec, b);
		lightVec.z = dot(tmpVec, n);
		tmpVec = -vertexPosition_cameraspace;
		eyeVec.x = dot(tmpVec, t);
		eyeVec.y = dot(tmpVec, b);
		eyeVec.z = dot(tmpVec, n);
		FtexCoord = texCoord;
     	  
    if ( use_texture == 1){
		gl_FrontColor = gl_Color;
    }
    
}
