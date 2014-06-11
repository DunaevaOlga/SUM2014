 /* FILENAME: CLOCK.C
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
typedef struct tagod6UNIT_CLOCK
{
  OD6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
} od6UNIT_CLOCK;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitInit( od6UNIT_CLOCK *Unit, od6ANIM *Ani )
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
static VOID ClockUnitClose( od6UNIT_CLOCK *Unit, od6ANIM *Ani )
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
static VOID ClockUnitResponse( od6UNIT_CLOCK *Unit, od6ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */


 /* Функция рисования стрелки.
 * АРГУМЕНТЫ:
 *   - контекст:
 *       HDC hDC;
 *   - координаты центра:
 *       INT Xc, Yc;
 *   - длина и ширина стрелки:
 *       INT L, W;
 *   - угол рисования:
 *       FLOAT Angle;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitRender( od6UNIT_CLOCK *Unit, od6ANIM *Ani )
{
  static DBL X, Y;
  
  SYSTEMTIME st;
  FLOAT PI = 3.14159265358979323846;


  GetLocalTime(&st);

  X += Ani->JX;
  Y += Ani->JY;

  DrawArrow(Ani->hDC, Ani->W / 2 + X * 3, Ani->H / 2 + Y * 3, Ani->W * Ani->H / 9000, 20, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * PI);
  DrawArrow(Ani->hDC, Ani->W / 2 + X * 3, Ani->H / 2 + Y * 3, Ani->W * Ani->H / 7500, 15, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * PI);
  DrawArrow(Ani->hDC, Ani->W / 2 + X * 3, Ani->H / 2 + Y * 3, Ani->W * Ani->H / 6000, 10, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * PI);
  DrawArrow(Ani->hDC, Ani->W / 2 + X * 3, Ani->H / 2 + Y * 3, Ani->W * Ani->H / 5000, 3, (-st.wMilliseconds / 1000.0) * 2 * PI);
 /* DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * 30,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * 30;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30); */
} /* End of 'ClockUnitRender' function */



/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_ClockUnitCreate( VOID )
{
  od6UNIT_CLOCK *Unit;

  if ((Unit = (od6UNIT_CLOCK *)OD6_AnimUnitCreate(sizeof(od6UNIT_CLOCK))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)ClockUnitInit;
  Unit->Close = (VOID *)ClockUnitClose;
  Unit->Response = (VOID *)ClockUnitResponse;
  Unit->Render = (VOID *)ClockUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (od6UNIT *)Unit;                     
} /* End of 'OD6_ClockUnitCreate' function */

/***********************************************************/

typedef struct tagod6UNIT_INFO
{
  OD6_UNIT_BASE_FIELDS;
  IMAGE And, Xor;
}od6UNIT_INFO;
/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID InfoUnitInit( od6UNIT_INFO *Unit, od6ANIM *Ani ) 
{
  ImageLoad(&Unit->And,"And.bmp");
  ImageLoad(&Unit->Xor,"Xor.bmp");

 
}
static VOID InfoUnitRender( od6UNIT_INFO *Unit, od6ANIM *Ani )
{
  static DBL Z, R;
  INT x, y;
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));

  Z += Ani->JZ;
  R += Ani->JR;
  x = Ani->W - Unit->And.W;
  y = 0;

  ImageDraw(&Unit->And, Ani->hDC, x + R * 3, y + Z * 3, SRCAND);
  ImageDraw(&Unit->Xor, Ani->hDC, x + R * 3, y + Z * 3, SRCINVERT);
} /* End of 'OD6_AnimUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_InfoUnitCreate( VOID )
{
  od6UNIT *Unit;

  if ((Unit = (od6UNIT_INFO *)OD6_AnimUnitCreate(sizeof(od6UNIT_INFO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'OD6_InfoUnitCreate' function */
/* END OF 'SAMPLE.C' FILE */