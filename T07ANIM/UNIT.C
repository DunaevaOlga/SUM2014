/* FILENAME: UNIT.C
 * PROGRAMMER: OD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID OD6_AnimUnitInit( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID OD6_AnimUnitClose( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID OD6_AnimUnitResponse( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID OD6_AnimUnitRender( od6UNIT *Unit, od6ANIM *Ani )
{
} /* End of 'OD6_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_AnimUnitCreate( INT Size )
{
  od6UNIT *Unit;

  if (Size < sizeof(od6UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* заполняем поля по-умолчанию */
  Unit->Size = Size;
  Unit->Init = OD6_AnimUnitInit;
  Unit->Close = OD6_AnimUnitClose;
  Unit->Response = OD6_AnimUnitResponse;
  Unit->Render = OD6_AnimUnitRender;
  return Unit;
} /* End of 'OD6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

