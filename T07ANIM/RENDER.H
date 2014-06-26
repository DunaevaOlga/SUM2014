/* FILENAME: RENDER.H
 * PROGRAMMER: OD6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "shader.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} od6CAMERA;


typedef struct tagod6UV
{
  FLT U, V; /* компоненты текстурных координат */
} od6UV;

/* Функиця инициализации текстурных координат */
__inline od6UV OD6_UVSet( FLT U, FLT V )
{
  od6UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
}

/* Тип представления вершины */
typedef struct tagod6VERTEX
{
  VEC P;    /* координаты */
  od6UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} od6VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagod6PRIM_TYPE
{
  OD6_PRIM_TRIMESH, OD6_PRIM_GRID
} od6PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagod6PRIM
{
  od6PRIM_TYPE Type; /* тип примитива */
  INT Id;       /* Идентификатор примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  od6VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} od6PRIM;

/* Шейдерная программа */
extern UINT OD6_ShaderProg;

/* Цвет по-умолчанию */
extern COLOR OD6_DefaultColor;

/* Функция создания примитива */
BOOL OD6_PrimCreate( od6PRIM *P, od6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL OD6_PrimCreateSphere( od6PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция создания цилиндра */
BOOL OD6_PrimCreateCylinder( od6PRIM *P,
       FLT H, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID OD6_PrimFree( od6PRIM *P );

/* Функция отрисовки примитива */
VOID OD6_PrimDraw( od6PRIM *P );

/* Функция перевычисления нормалей у примитива */
VOID OD6_PrimAutoNormals( od6PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagod6MATERIAL
{
  INT Id;          /* идентификатор материала */
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} od6MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagod6GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  od6MATERIAL *Mtls;         /* таблица материалов */
  od6PRIM *Prims;            /* массив примитивов */
} od6GEOM;

/* Функция добавления материала к объекту */
INT OD6_GeomAddMaterial( od6GEOM *G,
                         od6MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT OD6_GeomAddPrim( od6GEOM *G, od6PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID OD6_GeomFree( od6GEOM *G );

/* Функция рисования */
VOID OD6_GeomDraw( od6GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL OD6_GeomLoad( od6GEOM *G, CHAR *FileName );

/* Функция создания карты высот.*/
BOOL OD6_PrimCreateHeightField( od6PRIM *P, CHAR *FileName, FLT Height, FLT Scale );

/* Функция преобразования вершин */
VOID OD6_GeomTransform( od6GEOM *G, MATR M );



#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
