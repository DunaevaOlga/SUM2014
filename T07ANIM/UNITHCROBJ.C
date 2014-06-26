/* FILENAME: UNITHCROBJ.C
 * PROGRAMMER: OD6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 26.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "render.h"
#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagod6UNIT_HCR
{
  OD6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
  od6GEOM Hcr;
} od6UNIT_HCR;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HCR *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HcrUnitInit( od6UNIT_HCR *Unit, od6ANIM *Ani )
{
  //OD6_GeomLoad(&Unit->Hcr, "e:\\Models\\Red Fox\\RED_FOX.OBJECT");
  //OD6_GeomLoad(&Unit->Hcr, "e:\\Models\\Raccoon\\RACCON.OBJECT");
  OD6_GeomLoad(&Unit->Hcr, "E:/Models/Headcrab/headcrab.obj");
  OD6_GeomTransform(&Unit->Hcr, MatrScale(0.050, 0.050, 0.050));

} /* End of 'HcrUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HCR *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HcrUnitClose( od6UNIT_HCR *Unit, od6ANIM *Ani )
{
  OD6_GeomFree(&Unit->Hcr);
} /* End of 'HcrUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HCR *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HcrUnitResponse( od6UNIT_HCR *Unit, od6ANIM *Ani )
{
}
 /* End of 'HcrUnitResponse' function */


/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HCR *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HcrUnitRender( od6UNIT_HCR *Unit, od6ANIM *Ani )
{
  MATR WVP;
  static DBL x = 0, y = 0, z  , u, h, Dist;
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
    x = 100, z = 100,h = 0, u = 0, z = 0;

  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x, h, 0 ), MatrMulMatr((MatrRotateY(u )), MatrRotateX(z)));
  Ani->MatrView = MatrViewLookAt(VecSet(3 + Dist, 3 + Dist, 3 + Dist), VecSet(0, 0, 0), VecSet(0, 1, 0));

  if ((time += Ani->GlobalDeltaTime) > 1)
  {
    time = 0;
    OD6_ShaderProg = OD6_ShadProgInit("b.vert", "b.frag");
  }

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (i = -3; i <= 3; i++)
      for (j = -3; j <= 3; j++)
    {
      INT loc;

      glUseProgram(OD6_ShaderProg); 
      loc = glGetUniformLocation(OD6_ShaderProg, "indI");
      if (loc != -1)
        glUniform1f(loc, i);
      loc = glGetUniformLocation(OD6_ShaderProg, "indJ");
      if (loc != -1)
        glUniform1f(loc, j);

      Ani->MatrWorld = MatrMulMatr(MatrTranslate(i + x, 0, j +  y), MatrMulMatr(MatrRotateY (u + ((30 * Ani->GlobalTime ) * rand())), MatrRotateX(z)));;
      /* отладочный вывод */
      WVP = MatrMulMatr(Ani->MatrWorld,
        MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
      glLoadMatrixf(WVP.A[0]);

      OD6_GeomDraw(&Unit->Hcr);
    }             
} /* End of 'HcrUnitRender' function */



/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_HcrUnitCreate( VOID )
{
  od6UNIT_HCR *Unit;

  if ((Unit = (od6UNIT_HCR *)OD6_AnimUnitCreate(sizeof(od6UNIT_HCR))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)HcrUnitInit;
  Unit->Close = (VOID *)HcrUnitClose;
  Unit->Response = (VOID *)HcrUnitResponse;
  Unit->Render = (VOID *)HcrUnitRender;
  return (od6UNIT *)Unit;

} /* End of 'OD6_HcrUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */