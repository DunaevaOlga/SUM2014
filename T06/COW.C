/* FILENAME: CLOCK.C
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
  od6GOBJ Cow;
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
  OD6_RndGObjLoad(&Unit->Cow, "cow.object");
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
  OD6_RndGObjFree(&Unit->Cow);
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
  OD6_RndWs = Ani->W;
  OD6_RndHs = Ani->H;
  OD6_RndWp = OD6_RndHp * Ani->W / Ani->H;
  OD6_RndMatrProjection = MatrProjection(-OD6_RndWp / 2, OD6_RndWp / 2, -OD6_RndHp / 2, OD6_RndHp / 2, OD6_RndProjDist, 1000.0);
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
  INT i, N = 0;
  VEC p = {1, 0, 0};
  POINT pt;
  static DBL Delta = 0.1;

  Delta += Ani->JZ * Ani->GlobalDeltaTime;


  OD6_RndMatrView = MatrViewLookAt(VecMulMatr(VecSet(0, 0, Ani->JX * Delta + 15), MatrRotateX(90 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  OD6_RndMatrWorld = MatrRotateZ(-Ani->Time);
  OD6_RndMatrWorld = MatrMulMatr(OD6_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));

  srand(30);
  for (i = 0; i < N; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1 ;
    p.Y = 2.0 * rand() / RAND_MAX - 1 ;
    p.Z = 2.0 * rand() / RAND_MAX - 1 ;
    pt = OD6_RndWorldToScreen(p);
    if (i == 0)
      MoveToEx(Ani->hDC, pt.x , pt.y, NULL);
    else
      LineTo(Ani->hDC, pt.x , pt.y);
  }
  OD6_RndGObjDraw(&Unit->Cow, Ani->hDC);
 /*
  OD6_RndMatrWorld = MatrRotateZ(-Ani->GlobalTime / 60);
  OD6_RndMatrWorld = MatrMulMatr(OD6_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SetDCBrushColor(Ani->hDC, RGB(0, 255, 255));
  SetDCPenColor(Ani->hDC, RGB(0, 255, 255));

  OD6_RndGObjDraw(&Unit->Cow, Ani->hDC);

  OD6_RndMatrWorld = MatrRotateZ(-Ani->GlobalTime / 60 / 60);
  OD6_RndMatrWorld = MatrMulMatr(OD6_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SetDCBrushColor(Ani->hDC, RGB(255, 255, 0));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 0));

  OD6_RndGObjDraw(&Unit->Cow, Ani->hDC);

  OD6_RndMatrWorld = MatrRotateY(Ani->Time * 30);

  OD6_RndMatrWorld = MatrMulMatr(OD6_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));
  OD6_RndMatrWorld = MatrMulMatr(MatrTranslate(0.0, 0.0, 3 * 3.30), OD6_RndMatrWorld);
  OD6_RndGObjDraw(&Unit->Cow, Ani->hDC); */  
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

/* END OF 'COW.C' FILE */