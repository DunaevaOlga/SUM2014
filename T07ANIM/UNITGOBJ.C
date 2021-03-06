 /* FILENAME: UNITINFO.C
 * PROGRAMMER: OD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "render.h"
#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagod6UNIT_COW
{
  OD6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
  od6GEOM Cow;
  od6GEOM Land;
  od6GEOM Water;
} od6UNIT_COW;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitInit( od6UNIT_COW *Unit, od6ANIM *Ani )
{
  od6PRIM p;
  od6MATERIAL mtl;
  //OD6_GeomLoad(&Unit->Cow, "e:\\Models\\Red Fox\\RED_FOX.OBJECT");
  //OD6_GeomLoad(&Unit->Cow, "e:\\Models\\Raccoon\\RACCON.OBJECT");

  /* �������� ��������� */
  memset(&Unit->Land, 0, sizeof(od6GEOM));
  OD6_PrimCreateHeightField(&p, "hf.bmp", 3.0, 10);

  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 1;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "map2-hf.bmp");
  strcpy(mtl.MapD, "r.bmp");
  strcpy(mtl.Name, "Height Field Material");
  p.Mtl = OD6_GeomAddMaterial(&Unit->Land, &mtl);

  OD6_GeomAddPrim(&Unit->Land, &p);


  /* �������� ���� */
  memset(&Unit->Water, 0, sizeof(od6GEOM));
  OD6_DefaultColor = ColorSet(1, 0, 0);
  OD6_PrimCreate(&p, OD6_PRIM_GRID, 30, 30);

  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 0.5;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "1map2-hf.bmp");
  strcpy(mtl.MapD, "1r.bmp");
  strcpy(mtl.Name, "Water Material");
  p.Mtl = OD6_GeomAddMaterial(&Unit->Water, &mtl);

  OD6_GeomAddPrim(&Unit->Water, &p);
  OD6_GeomTransform(&Unit->Water, MatrMulMatr(MatrScale(60, 1, 60), MatrTranslate(-30, 0.8, -30)));
} /* End of 'CowUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitClose( od6UNIT_COW *Unit, od6ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitResponse( od6UNIT_COW *Unit, od6ANIM *Ani )
{
}
 /* End of 'CowUnitResponse' function */


/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitRender( od6UNIT_COW *Unit, od6ANIM *Ani )
{
  MATR WVP;
  static DBL x, y, z, u, h, Dist;
  INT i, j;
  static DBL time;
  
  x += Ani->JX * Ani->GlobalDeltaTime * 0.30;
  y += Ani->JY * Ani->GlobalDeltaTime * 0.30;
  z += Ani->JZ * Ani->GlobalDeltaTime * 3 * 30;
  u += Ani->JR * Ani->GlobalDeltaTime * 3 * 30;
  if (Ani->JButs[0])
  Dist += 10 * Ani->GlobalDeltaTime;;
  if (Ani->JButs[3])
  Dist -= 10 * Ani->GlobalDeltaTime;;
  if (Ani->JButs[2])
  h += 10 * Ani->GlobalDeltaTime;;
  if (Ani->JButs[1])
  h -= 10 * Ani->GlobalDeltaTime;;
  if (Ani->KeysClick['R'])
    x = 0, z = 0,h = 0, u = 0, z = 0;

  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x, h, y), MatrMulMatr(MatrRotateY(u), MatrRotateX(z)));
  Ani->MatrView = MatrViewLookAt(VecSet(3 + Dist, 3 + Dist, 3 + Dist), VecSet(0, 0, 0), VecSet(0, 1, 0));

  if ((time += Ani->GlobalDeltaTime) > 1)
  {
    time = 0;
    OD6_ShaderProg = OD6_ShadProgInit("a.vert", "a.frag");
  }

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
  /*for (i = -5; i <= 5; i++)
    for (j = -5; j <= 5; j++)
    {
      INT loc;

      glUseProgram(OD6_ShaderProg); 
      loc = glGetUniformLocation(OD6_ShaderProg, "indI");
      if (loc != -1)
        glUniform1f(loc, i);
      loc = glGetUniformLocation(OD6_ShaderProg, "indJ");
      if (loc != -1)
        glUniform1f(loc, j);

      Ani->MatrWorld = MatrMulMatr(MatrTranslate(i * 2 +  x, 0, j * 3 +  z), MatrMulMatr(MatrRotateY (y), MatrRotateX(u)));;
      /* ���������� ����� */      /*
      WVP = MatrMulMatr(Ani->MatrWorld,
        MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
      glLoadMatrixf(WVP.A[0]);

      OD6_GeomDraw(&Unit->Cow);
    }             */

  WVP = MatrMulMatr(Ani->MatrWorld,
     MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
  glLoadMatrixf(WVP.A[0]);  

  OD6_GeomDraw(&Unit->Land);
  OD6_GeomDraw(&Unit->Water);
} /* End of 'CowUnitRender' function */



/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (od6UNIT *) ��������� �� ��������� ������ ��������.
 */
od6UNIT * OD6_CowUnitCreate( VOID )
{
  od6UNIT_COW *Unit;

  if ((Unit = (od6UNIT_COW *)OD6_AnimUnitCreate(sizeof(od6UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  return (od6UNIT *)Unit;

} /* End of 'OD6_CowUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */