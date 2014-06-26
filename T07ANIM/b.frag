#include "comshad.h"

out vec4 UseColor;

in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTex;

void main( void )
{
  if (MtlId == 0)
    discard;
  if (PartNo >= 1)
    discard;
  vec3 L = normalize(vec3(1 * cos(Time * 3.0), 5 * sin(Time * 5) * 0 + 0.2, 1 * sin(Time * 3.0)));

  UseColor = vec4(0, 0, 0, 1);

  float nl = dot(L, normalize(DrawNormal));
  if (nl > 0)
    UseColor.xyz += vec3(0.1, 0, 0) * nl;

  vec3 L1 = normalize(vec3(21, 5, 1));
  vec4 tc = texture2D(Tex, vec2(1, -1) + vec2(1, -1) * DrawTex.xy);

  nl = dot(L1, normalize(DrawNormal));
  if (nl > 0)
    UseColor.xyz += vec3(tc) * nl;
  //UseColor = tc;
}
