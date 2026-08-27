#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 moveRight;
void main()
{
   gl_Position = vec4(aPos.x+ moveRight.x, aPos.y+ moveRight.y, aPos.z+ moveRight.z, 1.0);
}
