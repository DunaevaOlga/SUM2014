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

/* Структура описания объекта анимации */
typedef struct tagod6UNIT_SPRAIT
{
  OD6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;
  INT Type;
} od6UNIT_SPRAIT;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SpraitUnitInit( od6UNIT_SPRAIT *Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SpraitUnitClose( od6UNIT_SPRAIT*Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SpraitUnitResponse( od6UNIT_SPRAIT *Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */



/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SpraitUnitRender( od6UNIT_SPRAIT *Unit, od6ANIM *Ani )
{

} /* End of 'ClockUnitRender' function */



/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_SpaitUnitCreate( VOID )
{
  od6UNIT_SPAIT *Unit;

  if ((Unit = (od6UNIT_SPAIT *)OD6_AnimUnitCreate(sizeof(od6UNIT_SPAIT))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
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

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SpraitUnitRender( od6UNIT *Unit, od6ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 20, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'OD6_AnimUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_SpraitUnitCreate( VOID )
{
  od6UNIT *Unit;

  if ((Unit = OD6_AnimUnitCreate(sizeof(od6UNIT))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Render = (VOID *)SpraitUnitRender;
  return Unit;
} /* End of 'OD6_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */