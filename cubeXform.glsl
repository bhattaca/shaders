/*
#version 150
#extension GL_EXT_geometry_shader4 : enable
*/
#version 330 compatibility
uniform mat4 mvp;
uniform vec3 LightPosition_worldspace;
uniform mat4 model;
uniform mat4 view;
uniform int use_texture;
uniform mat4 bias;
uniform mat4 depthMVP;

varying in vec4 pcolor[3];
varying out vec4 _pcolor;

varying vec3 Position_worldspace[3];

varying in vec4 vertexPosition_cameraspace[3];
varying in vec3 Normal_cameraspace[3];
varying in vec3 EyeDirection_cameraspace[3];
varying in vec3 LightDirection_cameraspace[3];
varying in vec4 pnormal[3];
//varying vec4 ShadowCoord[3];
//// bump mapping 
varying in vec3 lightVec[3];
varying in vec3 eyeVec[3];
varying in vec2 FtexCoord[3];
////////////////
varying out  vec3 _Position_worldspace;
varying out vec4 _vertexPosition_cameraspace;
varying out vec3 _Normal_cameraspace;
varying out vec3 _EyeDirection_cameraspace;
varying out vec3 _LightDirection_cameraspace;
varying out vec4 _pnormal;
varying out vec4 _ShadowCoord;
//// bump mapping 
varying out vec3 _lightVec;
varying out vec3 _eyeVec;
varying out vec2 _FtexCoord;

layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;

float random( vec2 p )
{
  // We need irrationals for pseudo randomness.
  // Most (all?) known transcendental numbers will (generally) work.
  const vec2 r = vec2(
    23.1406926327792690,  // e^pi (Gelfond's constant)
     2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
  return fract( cos( mod( 123456789., 1e-7 + 256. * dot(p,r) ) ) );  
}



void main() {
	
  for(int i = 0; i < 3; i++) { // You used triangles, so it's always 3
		gl_Position = gl_in[i].gl_Position;
		_pcolor = pcolor[i];
		/*
		_vertexPosition_cameraspace = (view*model*position).xyz;
		_Normal_cameraspace=normalize(gl_NormalMatrix * pnormal[i].xyz);
		_EyeDirection_cameraspace=EyeDirection_cameraspace[i];
		_LightDirection_cameraspace=LightDirection_cameraspace[i];
		
		*/
		_pnormal=pnormal[i];
		_ShadowCoord.xyzw=bias*mvp*gl_in[i].gl_Position;
		//// bump mapping 
		_lightVec=lightVec[i];
		_eyeVec=eyeVec[i]; 
		_FtexCoord = FtexCoord[i];
		gl_Position = mvp*gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	
 for (int j=0;j <30;j+=3){
 float xloc = random( vec2(j,j*1000%49) );
 float yloc = random (vec2(xloc,j*1000%70));
   for(int i = 0; i < 3; i++) { // You used triangles, so it's always 3
		gl_Position = gl_in[i].gl_Position;
		gl_Position = gl_Position + vec4(( (j+20*xloc)),5.0,(j-j*10*yloc),1.0);
		_pcolor = pcolor[i];
		_pnormal=pnormal[i];
		_ShadowCoord.xyzw=bias*mvp*gl_Position;
		//// bump mapping 
		_lightVec=lightVec[i];
		_eyeVec=eyeVec[i]; 
		_FtexCoord = FtexCoord[i];
		gl_Position = mvp*gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	}

 for (int j=0;j <30;j+=3){
 float xloc = random( vec2(j,j*1000%49) );
 float yloc = random (vec2(xloc,j*1000%70));
   for(int i = 0; i < 3; i++) { // You used triangles, so it's always 3
		gl_Position = gl_in[i].gl_Position;
		gl_Position = gl_Position + vec4( ( int(j-40*xloc)%20),5.0,((int(50*yloc))%20),1.0);
		_pcolor = pcolor[i];
		_pnormal=pnormal[i];
		_ShadowCoord.xyzw=bias*mvp*gl_Position;
		//// bump mapping 
		_lightVec=lightVec[i];
		_eyeVec=eyeVec[i]; 
		_FtexCoord = FtexCoord[i];
		gl_Position = mvp*gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	}

 
}

