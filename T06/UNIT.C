/* FILENAME: UNIT.C
 * PROGRAMMER: OD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OD6_AnimUnitInit( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OD6_AnimUnitClose( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OD6_AnimUnitResponse( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       od6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       od6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OD6_AnimUnitRender( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (od6UNIT *) ��������� �� ��������� ������ ��������.
 */
od6UNIT * OD6_AnimUnitCreate( INT Size )
{
  od6UNIT *Unit;

  if (Size < sizeof(od6UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* ��������� ���� ��-��������� */
  Unit->Size = Size;
  Unit->Init = OD6_AnimUnitInit;
  Unit->Close = OD6_AnimUnitClose;
  Unit->Response = OD6_AnimUnitResponse;
  Unit->Render = OD6_AnimUnitRender;
  return Unit;
} /* End of 'OD6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

