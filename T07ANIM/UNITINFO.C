/* FILENAME: UNITINFO.C
 * PROGRAMMER: OD6
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagod6UNIT_INFO
{
  OD6_UNIT_BASE_FIELDS; /* Включение базовых полей */
} od6UNIT_INFO;

/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitInit( od6UNIT_INFO *Unit, od6ANIM *Ani )
{
  wglUseFontBitmaps(Ani->hDC, 0, 256, 111);
} /* End of 'InfoUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitClose( od6UNIT_INFO *Unit, od6ANIM *Ani )
{
} /* End of 'InfoUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitResponse( od6UNIT_INFO *Unit, od6ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    OD6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    OD6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
 /* if (Ani->JButsClick[2]);
  Ani->MatrWorld = MatrScale  */

} /* End of 'InfoUnitResponse' function */

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       od6UNIT_INFO *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( od6UNIT_INFO *Unit, od6ANIM *Ani )
{
  MATR mi = MatrIdenity();
  INT len;
  static CHAR Buf[1000];

  len = sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glUseProgram(0);
  glPushMatrix();
  glLoadMatrixf(mi.A[0]);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glRasterPos2d(-1, 0.95);
  glColor3d(1, 1, 1);
  glListBase(111);
  glCallLists(len, GL_UNSIGNED_BYTE, Buf);

  glRasterPos2d(-1, 0.88);
  glColor3d(1, 0.7, 0.5);
  glCallLists(sprintf(Buf, "J(%.3f %.3f %.3f [%.3f])-[%i]", Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JPOV), GL_UNSIGNED_BYTE, Buf);


  glPopMatrix();
  glPopAttrib();
} /* End of 'InfoUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT_INFO *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_InfoUnitCreate( VOID )
{
  od6UNIT_INFO *Unit;

  if ((Unit = (od6UNIT_INFO *)OD6_AnimUnitCreate(sizeof(od6UNIT_INFO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  return (od6UNIT *)Unit;
} /* End of 'OD6_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
