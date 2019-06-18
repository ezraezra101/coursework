#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform int colored;
uniform float twisted;

in  vec4 vPosition;
in  vec4 vColor;
out vec4 interpolatedColor;

vec4 twist(vec4 pos) {

	float s = sin(twisted*pos.y);
	float c = cos(twisted*pos.y);

	return
		mat4(c, 0, -s, 0,
			 0, 1, 0, 0,
			 s, 0, c, 0,
			 0, 0, 0, 1)
		 * pos;
}


void main() 
{
  // PROTIP #3
  // can extend to gl_Position = projection * camera * models * vertex
  // you may wish to view camera motion as a transform on the projection (projection * camera) * models ...
  // this could be thought of as moving the observer in space
  // or you may view camera motion as transforming all objects in the world while the observer remains still
  // projection * (camera * models) * ...
  // notice mathematically there is no difference
  // however in rare circumstances the order of the transform may affect the numerical stability
  // of the overall projection
  gl_Position = projection_matrix*model_matrix*twist(vPosition);


  if(colored > 0.5) {
	interpolatedColor = vColor;
  } else {
	interpolatedColor = vec4(1,0,0,1);
  }
} 