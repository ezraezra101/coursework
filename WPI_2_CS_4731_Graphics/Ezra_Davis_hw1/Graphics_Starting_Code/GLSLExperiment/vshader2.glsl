#version 150

//Vertex shader for Hilbert curve's background

attribute vec4 vPosition;
out vec3 color;

void
main()
{
    gl_Position = vPosition;
}
