#include "comshad.h"

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

out vec3 DrawPos;
out vec3 DrawNormal;
out vec2 DrawTex;

void main( void )
{
  DrawTex = InTexCoord;
  DrawPos = vec3(MatrWorld * vec4(InPosition, 1));
  DrawNormal = mat3(MatrWorldInverseTranspose) * InNormal;
  gl_Position = MatrWVP * vec4(InPosition + 0 * vec3(0, abs(sin(Time * 5 + indI + 20 * indJ)), 0), 1);
  //gl_Position = InPosition;
}
