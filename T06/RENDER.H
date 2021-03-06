/* FILENAME: RENDER.H
 * PROGRAMMER: OD6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} od6CAMERA;

typedef struct tagod6GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} od6GOBJ;


/* ���������� ������ */
extern od6CAMERA OD6_RndCam;

/* ��������� ������������� */
extern DBL
  OD6_RndWs, OD6_RndHs, /* ������� ����� � ������ */
  OD6_RndWp, OD6_RndHp, /* ������� ������� ������������� */
  OD6_RndProjDist;      /* ���������� �� ��������� �������� */

/* ������� */
extern MATR
  OD6_RndMatrWorld, /* ������� �������������� ������� �� */
  OD6_RndMatrView,  /* ������� �������������� ������� �� */                          
  OD6_RndMatrProjection;



/* ������� �������������� �� ������� ������� ��������� � ���� */
POINT OD6_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� ������� */
BOOL OD6_RndGObjLoad( od6GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ��������������� ������� */
VOID OD6_RndGObjFree( od6GOBJ *GObj );

/* ������� ��������� ��������������� ������� */
VOID OD6_RndGObjDraw( od6GOBJ *GObj, HDC hDC );

VOID OD6_RndMatrSetup( VOID );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
