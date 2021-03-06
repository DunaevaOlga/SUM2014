/* FILENAME: SAMPLE.C
 * PROGRAMMER: OD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagod6UNIT_COW
{
  OD6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
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
} /* End of 'CowUnitResponse' function */

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
  DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * 30,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * 30;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
} /* End of 'OD6_AnimUnitRender' function */

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
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (od6UNIT *)Unit;
} /* End of 'OD6_CowUnitCreate' function */

/***********************************************************/

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( od6UNIT *Unit, od6ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'OD6_AnimUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (od6UNIT *) ��������� �� ��������� ������ ��������.
 */
od6UNIT * OD6_InfoUnitCreate( VOID )
{
  od6UNIT *Unit;

  if ((Unit = OD6_AnimUnitCreate(sizeof(od6UNIT))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'OD6_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */
