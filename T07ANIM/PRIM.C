/* FILENAME: PRIM.C
 * PROGRAMMER: OD6
 * PURPOSE: Primtive handle functions.
 * LAST UPDATE: 23.06.2014
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* ������� */
#define power(A, B) ((A) < 0 ? -pow(-A, B) : pow(A, B))

/* ���� ��-��������� */
COLOR OD6_DefaultColor = {1, 1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ����������� ��������:
 *      od6PRIM *P;
 *   - ��� ���������:
 *       od6PRIM_TYPE Type;
 *   - ��������� ����� (������, ������) ���
 *     Type == OD6_PRIM_GRID ��� ���������� ������
 *     � ��������:
 *       INT NumOfV_GW, NumOfI_GH;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL OD6_PrimCreate( od6PRIM *P, od6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH )
{
  INT i, j, nv, ni, size;

  memset(P, 0, sizeof(od6PRIM));
  /* ����������� ��������� */
  switch (Type)
  {
  case OD6_PRIM_TRIMESH:
    nv = NumOfV_GW;
    ni = NumOfI_GH;
    break;
  case OD6_PRIM_GRID:
    nv = NumOfV_GW * NumOfI_GH;
    ni = NumOfV_GW * 2;
    break;
  }
  /* �������� ������ */
  size = sizeof(od6VERTEX) * nv + sizeof(INT) * ni;
  P->V = malloc(size);
  if (P->V == NULL)
    return FALSE;
  memset(P->V, 0, size);
  
  /* ��������� �������� */
  P->Size = size;
  P->Type = Type;
  P->NumOfV = nv;
  P->NumOfI = ni;
  P->GW = NumOfV_GW;
  P->GH = NumOfI_GH;
  P->I = (INT *)(P->V + nv);
  /* ��������� ��� ������� ��-��������� */
  for (i = 0; i < nv; i++)
  {
    P->V[i].C = OD6_DefaultColor;
    P->V[i].N = VecSet(0, 1, 0);
  }

  /* ��������� ������� � �������� ��� ���. ����� */
  if (Type == OD6_PRIM_GRID)
  {
    for (i = 0; i < NumOfI_GH; i++)
      for (j = 0; j < NumOfV_GW; j++)
      {
        P->V[i * NumOfV_GW + j].T =
          OD6_UVSet(j / (NumOfV_GW - 1.0),
                    i / (NumOfI_GH - 1.0));
        P->V[i * NumOfV_GW + j].P = VecSet(j - NumOfV_GW / 2, 0, i - NumOfI_GH / 2);
      }
    for (i = 0; i < NumOfV_GW; i++)
    {
      P->I[2 * i + 0] = NumOfV_GW + i;
      P->I[2 * i + 1] = i;
    }
  }
  return TRUE;
} /* End of 'OD6_PrimCreate' function */

/* ������� �������� �����.
 * ���������:
 *   - ����������� ��������:
 *       od6PRIM *P;
 *   - ����� �����:
 *       VEC C;
 *   - ������ �����:
 *       FLT R;
 *   - ��������� ����� (������, ������):
 *       INT M, N;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL OD6_PrimCreateSphere( od6PRIM *P, VEC C, FLT R, INT M, INT N )
{
  INT i, j;
  DBL t = clock() / 1000.0, t1;

  t1 = 2 + 2 * cos(t);
  t = 2 + 2 * sin(t * 1.01 + 2);

  if (!OD6_PrimCreate(P, OD6_PRIM_GRID, M, N))
    return FALSE;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      od6VERTEX *V = P->V + i * M + j;
      DBL
        theta = i / (N - 1.0) * OD6_PI,
        phi = j / (M - 1.0) * 2 * OD6_PI;

      V->N = VecSet(power(sin(theta), t1) * power(sin(phi), t),
                    power(cos(theta), t1),
                    power(sin(theta), t1) * power(cos(phi), t));
      V->P = VecAddVec(VecMulNum(V->N, R), C);
    }
  return TRUE;
} /* End of 'OD6_PrimCreateSphere' function */

/* ������� �������� ��������.
 * ���������:
 *   - ����������� ��������:
 *       od6PRIM *P;
 *   - ������ ��������:
 *       FLT H;
 *   - ������ ��������:
 *       FLT R;
 *   - ��������� ����� (������, ������):
 *       INT M, N;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL OD6_PrimCreateCylinder( od6PRIM *P, FLT H, FLT R, INT M, INT N )
{
  INT i, j;

  if (!OD6_PrimCreate(P, OD6_PRIM_GRID, M, N))
    return FALSE;
  /* ������ ���������� ����� */
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      od6VERTEX *V = P->V + i * M + j;
      FLT
        phi = j / (M - 1.0) * 2 * OD6_PI;

      V->N = VecSet(0, sin(phi), cos(phi));
      V->P = VecSet(-H / 2 + i * H / (N - 1), V->N.Y * R, V->N.Z * R);
      V->T.U *= 10;
      V->T.V *= 8;  /* ���������� ����� */
    }
  return TRUE;
} /* End of 'OD6_PrimCreateCylinder' function */

/* ������� �������� ����� �����.
 * ���������:
 *   - ����������� ��������:
 *       od6PRIM *P;
 *   - ��������� ����������� � ��������:
 *       CHAR *FileName;
 *   - ������������ ������ � ���������������:
 *       FLT Height, Scale;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL OD6_PrimCreateHeightField( od6PRIM *P, CHAR *FileName, FLT Height, FLT Scale )
{
  INT i, j;
  IMAGE Img;

  memset(P, 0, sizeof(od6PRIM));
  if (!ImageLoad(&Img, FileName))
    return FALSE;

  if (!OD6_PrimCreate(P, OD6_PRIM_GRID, Img.W, Img.H))
    return FALSE;
  /* ������ ���������� ����� */
  for (i = 0; i < Img.H; i++)
    for (j = 0; j < Img.W; j++)
    {
      od6VERTEX *V = P->V + i * Img.W + j;
      BYTE rgb[4], h;
      DWORD *col = (DWORD *)rgb;

      *col = ImageGetP(&Img, j, i);
      h = (rgb[2] * 30 + rgb[1] * 59 + rgb[0] * 11) / 100;
      V->P = VecSet((j - Img.W / 2) / Scale, h * Height / 255, (i - Img.H / 2) / Scale);
    }
  /* ������ ������� ����� */
  for (i = 1; i < Img.H - 1; i++)
    for (j = 1; j < Img.W - 1; j++)
    {
      VEC
        p00 = P->V[i * Img.W + j].P,
        p0_1 = P->V[i * Img.W + (j - 1)].P,
        p01 = P->V[i * Img.W + (j + 1)].P,
        p_10 = P->V[(i - 1) * Img.W + j].P,
        p10 = P->V[(i + 1) * Img.W + j].P,
        du0 = VecNormalize(VecSet((p0_1.Y - p00.Y), (p00.X - p0_1.X), 0)),
        du1 = VecNormalize(VecSet((p00.Y - p01.Y), (p01.X - p00.X), 0)),
        dv0 = VecNormalize(VecSet(0, (p00.Z - p_10.Z), (p_10.Y - p00.Y))),
        dv1 = VecNormalize(VecSet(0, (p10.Z - p00.Z), (p00.Y - p10.Y)));

      P->V[i * Img.W + j].N = VecNormalize(VecAddVec(VecAddVec(du0, du1), VecAddVec(dv0, dv1)));
      /// P->V[i * Img.W + j].N = VecNormalize(VecAddVec(du0, du1));
    }
  return TRUE;
} /* End of 'OD6_PrimCreateHeightField' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� ��������:
 *       od6PRIM *P;
 * ������������ ��������: ���.
 */
VOID OD6_PrimFree( od6PRIM *P )
{
  if (P->VertexBuffer != 0)
    glDeleteVertexArrays(1, &P->VertexBuffer);
  if (P->Buffers[0] != 0)
    glDeleteBuffers(2, P->Buffers);
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(od6PRIM));
} /* End of 'OD6_PrimFree' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ��������:
 *       od6PRIM *P;
 * ������������ ��������: ���.
 */
VOID OD6_PrimDraw( od6PRIM *P )
{
  INT i;

  if (P->V == NULL)
    return;

  if (P->Buffers[0] == 0)
  {
    /* ������� � ��������� ������ ������ */
    glGenBuffers(2, P->Buffers);
    glGenVertexArrays(1, &P->VertexBuffer);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(od6VERTEX) * P->NumOfV,
      P->V, GL_STATIC_DRAW);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * P->NumOfI,
      P->I, GL_STATIC_DRAW);
  }

  /* ���������� ������ */
  glBindVertexArray(P->VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P->Buffers[1]);

  /* ������ ������� ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(od6VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(od6VERTEX), (VOID *)(sizeof(VEC)));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(od6VERTEX), (VOID *)(sizeof(VEC) + sizeof(od6UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(od6VERTEX), (VOID *)(2 * sizeof(VEC) + sizeof(od6UV)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glUseProgram(OD6_ShaderProg);
  if (P->Type == OD6_PRIM_TRIMESH)
  {
    /*
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfI; i++)
      glVertex3fv(&P->V[P->I[i]].P.X);
    glEnd();
    */
    glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  else
  {
    for (i = 0; i < P->GH - 1; i++)
    {
      /*
      INT start = i * P->GW, j;

      glBegin(GL_TRIANGLE_STRIP);
      for (j = 0; j < P->NumOfI; j++)
        glVertex3fv(&P->V[start + P->I[j]].P.X);
      glEnd();
      */
      glDrawElementsBaseVertex(GL_TRIANGLE_STRIP,
        P->NumOfI, GL_UNSIGNED_INT, (VOID *)0,
        i * P->GW);
    }
  }
  glUseProgram(0);
} /* End of 'OD6_PrimFree' function */

/* ������� �������������� �������� � ���������.
 * ���������:
 *   - ��������:
 *       od6PRIM *P;
 * ������������ ��������: ���.
 */
VOID OD6_PrimAutoNormals( od6PRIM *P )
{
  INT i;

  if (P->Type == OD6_PRIM_TRIMESH)
  {
    /* �������� ��� ������� ������ */
    for (i = 0; i < P->NumOfV; i++)
      P->V[i].N = VecSet(0, 0, 0);

    /* ��������� ������� ���� ������ � �������� �� � ���������� �� �������� */
    for (i = 0; i < P->NumOfI / 3; i++)
    {
      INT *n = P->I + i * 3;
      VEC norm;

      norm =
        VecNormalize(VecCrossVec(VecSubVec(P->V[n[1]].P, P->V[n[0]].P),
                                 VecSubVec(P->V[n[2]].P, P->V[n[0]].P)));
      P->V[n[0]].N = VecAddVec(P->V[n[0]].N, norm);
      P->V[n[1]].N = VecAddVec(P->V[n[1]].N, norm);
      P->V[n[2]].N = VecAddVec(P->V[n[2]].N, norm);
    }

    /* ��������� */
    for (i = 0; i < P->NumOfV; i++)
      P->V[i].N = VecNormalize(P->V[i].N);
  }
  else
  {
  }
} /* End of 'OD6_PrimAutoNormals' function */

/* END OF 'PRIM.C' FILE */
