#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform float texture_scale;

in  vec4 vTexCoord;
out vec2 texCoord;

in  vec4 vPosition;
//out vec4 interpolatedColor;

in vec4 vNormal;
out vec4 fNormal;
out vec3 fPos;

void main()
{
  // We only need the s and t values of the texture coordinate
  texCoord = vTexCoord.st * texture_scale;
  gl_Position = projection_matrix*model_matrix*vPosition;

  fNormal = normalize(model_matrix*vec4(vNormal.xyz,0.0));
  fPos    = normalize((model_matrix * vPosition).xyz).xyz;

}
