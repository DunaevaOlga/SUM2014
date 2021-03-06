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
typedef struct tagod6UNIT_SPRAIT
{
  OD6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;
  INT Type;
} od6UNIT_SPRAIT;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID SpraitUnitInit( od6UNIT_SPRAIT *Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID SpraitUnitClose( od6UNIT_SPRAIT*Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID SpraitUnitResponse( od6UNIT_SPRAIT *Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */



/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID SpraitUnitRender( od6UNIT_SPRAIT *Unit, od6ANIM *Ani )
{

} /* End of 'ClockUnitRender' function */



/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (od6UNIT *) ��������� �� ��������� ������ ��������.
 */
od6UNIT * OD6_SpaitUnitCreate( VOID )
{
  od6UNIT_SPAIT *Unit;

  if ((Unit = (od6UNIT_SPAIT *)OD6_AnimUnitCreate(sizeof(od6UNIT_SPAIT))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)SpaitUnitInit;
  Unit->Close = (VOID *)SpaitUnitClose;
  Unit->Response = (VOID *)SpaitUnitResponse;
  Unit->Render = (VOID *)SpaitUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (od6UNIT *)Unit;
} /* End of 'OD6_ClockUnitCreate' function */

/***********************************************************/

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID SpraitUnitRender( od6UNIT *Unit, od6ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 20, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'OD6_AnimUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (od6UNIT *) ��������� �� ��������� ������ ��������.
 */
od6UNIT * OD6_SpraitUnitCreate( VOID )
{
  od6UNIT *Unit;

  if ((Unit = OD6_AnimUnitCreate(sizeof(od6UNIT))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Render = (VOID *)SpraitUnitRender;
  return Unit;
} /* End of 'OD6_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */