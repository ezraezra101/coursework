#version 150

//Fragment shader for Hilbert curve's background

out vec4  fColor;
uniform vec3 vColor;

void
main()
{
    fColor = vec4(vColor.r, vColor.g, vColor.b, 1.0);//vec4(color.r, color.g, color.b, 1.0);
}
