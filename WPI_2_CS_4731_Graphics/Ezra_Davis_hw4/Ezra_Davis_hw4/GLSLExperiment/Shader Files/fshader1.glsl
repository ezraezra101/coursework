#version 150

uniform int textured;

uniform samplerCube cubeTex;
uniform sampler2D texture;
in  vec2 texCoord;
in  vec4  interpolatedColor;
out vec4  fColor;

in vec4 fNormal;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;

uniform float Shininess;

uniform vec4 color;

uniform float IOR;

in vec3 fPos;


/*
vec4 mix(vec4 v1, vec4 v2, float fac) {
	return v1 * (1-fac) + v2*fac;
}
*/

vec4 lighten(vec4 v1) {
	return mix(v1, vec4(1.0,1.0,1.0,1.0), 0.3);
}

void main() 
{

	//Phong shading
  //Phong lighting model:
  //  To light source
  //  To viewer
  //  normal
  // mirror

  //Creating vectors for lighting:
  vec3 view   = normalize(-fPos);
  vec3 light  = normalize(LightPosition.xyz - fPos);
  vec3 halfway= normalize(light+view);
  vec3 normal = normalize(fNormal.xyz);


  //Lighting:
  vec4 ambient = AmbientProduct;
  vec4 diffuse = max(dot(-light, -normal),0.0)*DiffuseProduct;

  float cos_phi = pow(max(dot(normal,halfway), 0.0), Shininess);
  vec4 specular = cos_phi * SpecularProduct;

  vec4 lighting = (ambient + diffuse + specular);

  //Texturing!
  vec4 texColor = color;
  if(textured == 0) {
	texColor = texture2D( texture, texCoord );
  }
  else if( (textured & 4 | textured & 2) == 6) {
	texColor = (textureCube(cubeTex, reflect(view,normal))+
				mix(textureCube(cubeTex, refract(view, normal, IOR)), vec4(1,1,1,1), 0.3))
				/2;
  }
  else if(textured & 4) { //reflections
	texColor = textureCube(cubeTex, reflect(view, normal));
  }
  else if(textured & 2) { //refractions
	texColor = mix(textureCube(cubeTex, refract(view, normal, IOR)), vec4(1,1,1,1), 0.3);
  }


  vec4 shadedColor = vec4(0,0,0,1);
  if(textured & 6)
	lighting = lighten(lighting);

  shadedColor.r = lighting.r * texColor.r;
  shadedColor.g = lighting.g * texColor.g;
  shadedColor.b = lighting.b * texColor.b;



	
	fColor = /*texColor */ shadedColor;



}
