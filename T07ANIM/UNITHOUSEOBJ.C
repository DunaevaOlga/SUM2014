/* FILENAME: UNITHOUSEOBJ.C
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
typedef struct tagod6UNIT_HOUSE
{
  OD6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
  od6GEOM House;
} od6UNIT_HOUSE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HOUSE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HouseUnitInit( od6UNIT_HOUSE *Unit, od6ANIM *Ani )
{
  //OD6_GeomLoad(&Unit->House, "e:\\Models\\Red Fox\\RED_FOX.OBJECT");
  //OD6_GeomLoad(&Unit->House, "e:\\Models\\Raccoon\\RACCON.OBJECT");
  OD6_GeomLoad(&Unit->House, "E:/Models/House/house.obj");
  OD6_GeomTransform(&Unit->House, MatrScale(0.030, 0.030, 0.030));


} /* End of 'HouseUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HOUSE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HouseUnitClose( od6UNIT_HOUSE *Unit, od6ANIM *Ani )
{
  OD6_GeomFree(&Unit->House);
} /* End of 'HouseUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HOUSE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HouseUnitResponse( od6UNIT_HOUSE *Unit, od6ANIM *Ani )
{
}
 /* End of 'HouseUnitResponse' function */


/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       od6UNIT_HOUSE *Unit;
 *   - указатель на контекст анимации:
 *       od6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID HouseUnitRender( od6UNIT_HOUSE *Unit, od6ANIM *Ani )
{
  MATR WVP;
  static DBL x, y, z, u, h, Dist;
  INT i, j;
  static DBL time;
  
  x += Ani->JX * Ani->GlobalDeltaTime * 3;
  y += Ani->JY * Ani->GlobalDeltaTime * 3;
  z += Ani->JZ * Ani->GlobalDeltaTime * 3 * 30;
  u += Ani->JR * Ani->GlobalDeltaTime * 3 * 30;
  if (Ani->JButs[0])
    Dist += 10 * Ani->GlobalDeltaTime;
  if (Ani->JButs[3])
    Dist -= 10 * Ani->GlobalDeltaTime;
  if (Ani->JButs[2])
    h += 10 * Ani->GlobalDeltaTime;
  if (Ani->JButs[1])
    h -= 10 * Ani->GlobalDeltaTime;
  if (Ani->KeysClick['R'])
    x = 0, z = 0,h = 0, u = 0, z = 0;

  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x, h + 1.35, y + 15), MatrMulMatr(MatrRotateY(u + 180), MatrRotateX(z)));
  Ani->MatrView = MatrViewLookAt(VecSet(3 + Dist, 3 + Dist, 3 + Dist), VecSet(0, 0, 0), VecSet(0, 1, 0));

  if ((time += Ani->GlobalDeltaTime) > 1)
  {
    time = 0;
    OD6_ShadProgClose(OD6_ShaderProg);
    OD6_ShaderProg = OD6_ShadProgInit("b.vert", "b.frag");
  }

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  /*for (i = -5; i <= 5; i++)
    for (j = -5; j <= 5; j++)
    {
      INT loc;

      glUseProgram(OD6_ShaderProg); 
      loc = glGetUniformLocation(OD6_ShaderProg, "indI");
      if (loc != -1)
        glUniform1f(loc, i);
      loc = glGetUniformLocation(OD6_ShaderProg, "indJ");
      if (loc != -1)
        glUniform1f(loc, j);

      Ani->MatrWorld = MatrMulMatr(MatrTranslate(i * 2 +  x, 0, j * 3 +  z), MatrMulMatr(MatrRotateY (y), MatrRotateX(u)));;
      /* отладочный вывод */      /*
      WVP = MatrMulMatr(Ani->MatrWorld,
        MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
      glLoadMatrixf(WVP.A[0]);

      OD6_GeomDraw(&Unit->House);
    }             */

  WVP = MatrMulMatr(Ani->MatrWorld,
     MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
  glLoadMatrixf(WVP.A[0]);  

  OD6_GeomDraw(&Unit->House);
} /* End of 'HouseUnitRender' function */



/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (od6UNIT *) указатель на созданный объект анимации.
 */
od6UNIT * OD6_HouseUnitCreate( VOID )
{
  od6UNIT_HOUSE *Unit;

  if ((Unit = (od6UNIT_HOUSE *)OD6_AnimUnitCreate(sizeof(od6UNIT_HOUSE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)HouseUnitInit;
  Unit->Close = (VOID *)HouseUnitClose;
  Unit->Response = (VOID *)HouseUnitResponse;
  Unit->Render = (VOID *)HouseUnitRender;
  return (od6UNIT *)Unit;

} /* End of 'OD6_HouseUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */