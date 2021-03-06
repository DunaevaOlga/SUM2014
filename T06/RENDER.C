/* FILENAME: RENDER.C
 * PROGRAMMER: OD6
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

/* ���������� ������ */
od6CAMERA OD6_RndCam;

/* ��������� ������������� */
DBL
  OD6_RndWs = 30, OD6_RndHs = 30,   /* ������� ����� � ������ */
  OD6_RndWp = 4, OD6_RndHp = 3,     /* ������� ������� ������������� */
  OD6_RndProjDist = 5;              /* ���������� �� ��������� �������� */

/* ������� */
MATR
  OD6_RndMatrWorld = OD6_UNIT_MATR, /* ������� �������������� ������� �� */
  OD6_RndMatrView = OD6_UNIT_MATR,
  OD6_RndMatrProjection = OD6_UNIT_MATR; /* ������� �������������� ������� �� */

static MATR
  OD6_RndMatrWorldViewProj;              /* �������� ������� �������������� */

/* ������� �������������� �������� ������� ��������������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID OD6_RndMatrSetup( VOID )
{
  OD6_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(OD6_RndMatrWorld, OD6_RndMatrView),
      OD6_RndMatrProjection);
} /* End of 'VG4_RndMatrSetup' function */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT OD6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* �������������� �� */
  Pp = VecMulMatr(P, OD6_RndMatrWorldViewProj);
  
  /* ������������� */
  Ps.x = ( Pp.X + 0.5) * (OD6_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5) * (OD6_RndHs - 1);
  return Ps;
} /* End of 'VG4_RndWorldToScreen' function */

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       od6GOBJ *GObj;
 *   - ��� ����� (*.OBJ):
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL OD6_RndGObjLoad( od6GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* �������� ������ */
  memset(GObj, 0, sizeof(od6GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* ������� ���������� ������ � ������ */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* �������� ������ ��� ������� � ����� ��� ������ ������� ������
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* ��������� ������ */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
      sscanf(Buf, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf, "%i %i %i", &a, &b, &c); 
      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndGObjLoad' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID OD6_RndGObjFree( od6GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(od6GOBJ));
} /* End of 'OD6_RndGObjFree' function */

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       od6GOBJ *GObj;
 *   - �������� ���������� ������:
 *       HDC hDC;
 * ������������ ��������: ���.
 */
VOID OD6_RndGObjDraw( od6GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 1;
  POINT *pts; 

  if ((pts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  OD6_RndMatrSetup();

   for (i = 0; i < GObj->NumOfV; i++)
  {
    pts[i] = OD6_RndWorldToScreen(GObj->V[i]);
  }

  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT n0 = GObj->F[i][0], n1 = GObj->F[i][1], n2 = GObj->F[i][2];
    MoveToEx(hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(hDC, pts[n1].x, pts[n1].y);
    LineTo(hDC, pts[n2].x, pts[n2].y);
    LineTo(hDC, pts[n0].x, pts[n0].y);
  }

  free(&pts);
} /* End of 'OD6_RndGObjDraw' function */

/* END OF 'RENDER.C' FILE */
