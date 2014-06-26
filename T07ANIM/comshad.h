#version 420

uniform vec4 CommonColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorld;
uniform mat4 MatrWorldInverseTranspose;

uniform sampler2D Tex;

uniform float indI;
uniform float indJ;
uniform float Time;

uniform float PartNo;

uniform int PrimId;
uniform int MtlId;

