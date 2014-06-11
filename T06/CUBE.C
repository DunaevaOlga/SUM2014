/* FILENAME: CUBE.C
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

/* Структура описания объекта анимации */
typedef struct tagod6UNIT_CUBE
{
  OD6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
} od6UNIT_CUBE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitInit( od6UNIT_CUBE *Unit, od6ANIM *Ani )
{
} /* End of 'CubeUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitClose( od6UNIT_CUBE *Unit, od6ANIM *Ani )
{
} /* End of 'CubeUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitResponse( od6UNIT_CUBE *Unit, od6ANIM *Ani )
{
  OD6_RndWs = Ani->W;
  OD6_RndHs = Ani->H;
} /* End of 'CubeUnitResponse' function */


/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitRender( od6UNIT_CUBE *Unit, od6ANIM *Ani )
{
  INT i, s = 1;
  VEC p = {1, 0, 0};
  POINT pt;

  OD6_RndMatrView = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  OD6_RndMatrWorld = MatrRotateY(Ani->Time * 30);

  SelectObject(Ani->hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(Ani->hDC, RGB(255, 0, 255));
  srand(30);
  for (i = 0; i < 1000; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1;
    p.Y = 2.0 * rand() / RAND_MAX - 1;
    p.Z = 2.0 * rand() / RAND_MAX - 1;
    pt = OD6_RndWorldToScreen(p);
    Ellipse(Ani->hDC, pt.x - s, pt.y - s, pt.x + s, pt.y + s);
  }
} /* End of 'ClockUnitRender' function */



/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_CubeUnitCreate( VOID )
{
  od6UNIT_CUBE *Unit;

  if ((Unit = (od6UNIT_CUBE *)OD6_AnimUnitCreate(sizeof(od6UNIT_CUBE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;
  Unit->Render = (VOID *)CubeUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (od6UNIT *)Unit;                     
} /* End of 'OD6_CubeUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */