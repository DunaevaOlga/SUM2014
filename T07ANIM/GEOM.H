/* FILENAME: GEOM.H
 * PROGRAMMER: OD6
 * PURPOSE: ������ � ��������������� ���������
 * LAST UPDATE: 23.06.2014
 */

#ifndef __GEOM_H_
#define __GEOM_H_

#include "def.h"

/* ��� ������������� ���������� ��������� */
typedef struct tagod6UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} od6UV;

/* ������� ������������� ���������� ��������� */
__inline od6UV OD6_UVSet( FLT U, FLT V )
{
  od6UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'VG4_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagod6VERTEX
{
  VEC P;    /* ���������� */
  od6UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} od6VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagod6PRIM_TYPE
{
  OD6_PRIM_TRIMESH, OD6_PRIM_GRID
} od6PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagod6PRIM
{
  od6PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  od6VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} od6PRIM;

/* ���� ��-��������� */
extern COLOR OD6_DefaultColor;

/* ������� �������� ��������� */
BOOL OD6_PrimCreate( od6PRIM *P, od6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL OD6_PrimCreateSphere( od6PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID OD6_PrimFree( od6PRIM *P );

/* ������� ��������� ��������� */
VOID OD6_PrimDraw( od6PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagod6MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} od6MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagod6GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  od6MATERIAL *Mtls;         /* ������� ���������� */
  od6PRIM *Prims;            /* ������ ���������� */
} od6GEOM;

/* ������� ���������� ��������� � ������� */
INT OD6_GeomAddMaterial( od6GEOM *G,
                         od6MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT OD6_GeomAddPrim( od6GEOM *G, od6PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID OD6_GeomFree( od6GEOM *G );

/* ������� ��������� */
VOID OD6_GeomDraw( od6GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL OD6_GeomLoad( od6GEOM *G, CHAR *FileName );

