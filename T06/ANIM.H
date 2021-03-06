/* FILENAME: ANIM.H
 * PROGRAMMER: OD6
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_


#include "image.h"

/* ������������ ���������� ��������� �������� */
#define OD6_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagod6UNIT od6UNIT;

/* ��������� �������� ��������� �������� */
typedef struct tagod6ANIM
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

  /* ������ ��������� �������� � �� ���������� */
  od6UNIT *Units[OD6_MAX_UNITS]; 
  INT NumOfUnits;

  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
  BYTE Keys[256];  /* ��c������ ������ ���������� � ���� */
  BYTE
    JButs[32],
    JButsOld[32],
    JButsClick[32],
    JPOV;
   DBL
    JX, JY, JZ, JR, JU;
  
} od6ANIM;


/*extern static od6ANIM OD6_Anim; */
                     
/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( od6UNIT *Unit, od6ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( od6UNIT *Unit, od6ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( od6UNIT *Unit, od6ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( od6UNIT *Unit, od6ANIM *Ani );
 */
#define OD6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( od6UNIT *Unit, od6ANIM *Ani );       \
  VOID (*Close)( od6UNIT *Unit, od6ANIM *Ani );      \
  VOID (*Response)( od6UNIT *Unit, od6ANIM *Ani );   \
  VOID (*Render)( od6UNIT *Unit, od6ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagod6UNIT
{
  OD6_UNIT_BASE_FIELDS; /* ������� ���� */
};

/***
 * ������� ���������� ���������
 ***/

/* ������� ������������� �������� */
VOID OD6_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID OD6_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������ */
VOID OD6_AnimResize( INT W, INT H );

/* ������� ���������� ����� �������� */
VOID OD6_AnimRender( VOID );

/* ������� ������ ����� �������� */
VOID OD6_AnimCopyFrame( VOID );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ��������� */
VOID OD6_AnimFlipFullScreen( VOID );

/* ������� ��������� ����� �������� */
VOID OD6_AnimSetPause( BOOL NewPauseFlag );

/***
 * ������� ��������� �������� ��������
 ***/

/* ������� ���������� � ������� ������� �������� */
VOID OD6_AnimAddUnit( od6UNIT *Unit );

/* ������� �������� ������� �������� */
od6UNIT * OD6_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
