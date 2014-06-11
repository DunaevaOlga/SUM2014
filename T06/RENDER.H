/* FILENAME: RENDER.H
 * PROGRAMMER: OD6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} od6CAMERA;

typedef struct tagod6GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} od6GOBJ;


/* Глобальная камера */
extern od6CAMERA OD6_RndCam;

/* Параметры проецирования */
extern DBL
  OD6_RndWs, OD6_RndHs, /* размеры кадра в точках */
  OD6_RndWp, OD6_RndHp, /* размеры обрасти проецирования */
  OD6_RndProjDist;      /* расстояние до плоскости проекции */

/* Матрицы */
extern MATR
  OD6_RndMatrWorld, /* матрица преобразования мировой СК */
  OD6_RndMatrView,  /* матрица преобразования видовой СК */                          
  OD6_RndMatrProjection;



/* Функция преобразования из мировой системы коорлинат в кадр */
POINT OD6_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта */
BOOL OD6_RndGObjLoad( od6GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID OD6_RndGObjFree( od6GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID OD6_RndGObjDraw( od6GOBJ *GObj, HDC hDC );

VOID OD6_RndMatrSetup( VOID );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
