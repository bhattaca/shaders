varying vec4 pcolor;
varying vec3 Position_worldspace;
varying vec4 vertexPosition_cameraspace2;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec4 pnormal;

uniform samplerCube env;
uniform int use_texture;
uniform sampler2D tex;
uniform sampler2D grass;
uniform sampler2D normal_tex;
uniform sampler2D normal_tex2;
attribute vec2 texCoord;


uniform mat4 depthMVP;
varying vec2 FtexCoord;
varying vec3 R;
uniform vec3 LightPosition_worldspace;


uniform sampler2D ShadowMap;

varying vec4 ShadowCoord;
//// bump mapping 
varying vec3 lightVec;
varying vec3 eyeVec;

/////////////////
varying vec4 _pcolor;
varying vec3 _Position_worldspace;

varying vec4 _vertexPosition_cameraspace2;
varying vec3 _Normal_cameraspace;
varying vec3 _EyeDirection_cameraspace;
varying vec3 _LightDirection_cameraspace;
varying vec4 _pnormal;
varying vec4 _ShadowCoord;
//// bump mapping 
varying vec3 _lightVec;
varying vec3 _eyeVec;
varying vec2 _FtexCoord;
//////////////////

void main()
{
/*
	if (use_texture == 1){
    vec3 envColor = vec3 (textureCube( env, normalize(R)));
    gl_FragColor = vec4(envColor, 1.0);
    }
    else{
    vec4 texcolor = texture2D(tex, FtexCoord);
    gl_FragColor = texcolor;
	}
*/
/*
  	//// shadow mapping 
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	//vec4 shadowCoordinateWdivide = ShadowCoord;
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
 	float shadow = 1.0;
 	   vec4 texcolor = texture2D(grass, FtexCoord);
 	if (ShadowCoord.w > 0.0)
 		{
 			shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
    		gl_FragColor =	 shadow*texcolor;
    	}
	else if ( ShadowCoord.w == 0.0 ) {
			gl_FragColor = vec4(255.0,0.0,0.0,1.0);
	}
	else {
		gl_FragColor = vec4(0.0,255.0,255.0,1.0);
	}
	if (use_texture == 0){
 	gl_FragColor = vec4(0.0,255.0,255.0,1.0);
    }
*/	
	/*
	float distSqr = dot(lightVec, lightVec);
	float att = clamp(1.0 - 0.0001 * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = lightVec * inversesqrt(distSqr);
	vec3 vVec = normalize(eyeVec);
	
	vec4 base = texture2D(tex,FtexCoord);
	vec3 bump = normalize( texture2D(normal_tex, FtexCoord).xyz * 2.0 - 1.0);
	//vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	vec4 vAmbient = vec4(0.6,0.6,0.6,1.0);
	float diffuse = max( dot(lVec, bump), 0.0 );
	vec4 vDiffuse = vec4(1.0,1.0,1.0,1.0)*diffuse;
	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0),1.0);
	vec4 vSpecular = vec4(1.0,1.0,1.0,1.0) * specular;	
	
	gl_FragColor = ( vAmbient*base + 
					 vDiffuse*base + 
					 vSpecular) ;	
   */


 //// geom shader shadow 
 	vec4 shadowCoordinateWdivide = _ShadowCoord / _ShadowCoord.w ;
	// Used to lower moiré pattern and self-shadowing
shadowCoordinateWdivide.z += 0.0005;
	
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
 	float shadow = 1.0;
 	   vec4 texcolor = texture2D(tex, _FtexCoord);
 	if (_ShadowCoord.w > 0.0)
 		{
 			shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
 			///
 			float distSqr = dot(_lightVec, _lightVec);
	float att = clamp(1.0 - 0.0001 * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = _lightVec * inversesqrt(distSqr);
	vec3 vVec = normalize(_eyeVec);
	
	vec4 base = texture2D(tex,_FtexCoord);
	vec3 bump = normalize( texture2D(normal_tex, _FtexCoord).xyz * 2.0 - 1.0);
	//vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	vec4 vAmbient = vec4(0.6,0.6,0.6,1.0);
	float diffuse = max( dot(lVec, bump), 0.0 );
	vec4 vDiffuse = vec4(1.0,1.0,1.0,1.0)*diffuse;
	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0),1.0);
	vec4 vSpecular = vec4(1.0,1.0,1.0,1.0) * specular;	
	
	gl_FragColor = shadow*( vAmbient*base + 
					 vDiffuse*base + 
					 vSpecular) ;
 			///
    		//gl_FragColor =	 shadow*texcolor;
    	if (use_texture == 4)
    	{
    	 	vec4 base = texture2D(grass, _FtexCoord);
    		vec3 bump = normalize( texture2D(normal_tex2, _FtexCoord).xyz * 2.0 - 1.0);
			//vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
			vec4 vAmbient = vec4(0.6,0.6,0.6,1.0);
			float diffuse = max( dot(lVec, bump), 0.0 );
			vec4 vDiffuse = vec4(1.0,1.0,1.0,1.0)*diffuse;
			float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0),1.0);
			vec4 vSpecular = vec4(0.3,0.3,0.3,0.3) * specular;	
	
			gl_FragColor = shadow*( vAmbient*base + 
					 vDiffuse*base + 
					 vSpecular) ;
    
    	}
    	if (use_texture == 7 )
    	{
    		gl_Color = vec4(1.0,0.0,0.0,1.0);
    	}
    	
    	}
   
  
}
