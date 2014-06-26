/* FILENAME: ANIM.H
 * PROGRAMMER: OD6
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"
#include "image.h"


/* Максимальное количество элементов анимации */
#define OD6_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagod6UNIT od6UNIT;

/* Структура хранения контекста анимации */
typedef struct tagod6ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HDC hDC;            /* Контекст окна */
  HGLRC hRC;          /* Контекст построения */

  /* Массив элементов анимации и их количество */
  od6UNIT *Units[OD6_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE 
    Keys[256],
    KeysOld[256], /* Сотояние клавиш на предыдущем кадре */
    KeysClick[256]; /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты */
    MsX, MsY, /* Абсолютные координаты */
    MsWheel; /* Состояние колеса мыши */
  BYTE
    JButs[32],
    JButsOld[32],
    JButsClick[32],
    JPOV;
   DBL
    JX, JY, JZ, JR, JU;
   /* Параметры проецирования */
   DBL
    Wp, Hp, /* размеры обрасти проецирования */
    ProjDist, ProjSize, FarClip;      /* расстояние до плоскости проекции */

    /* Матрицы */
    MATR
      MatrWorld, /* матрица преобразования мировой СК */
      MatrView,  /* матрица преобразования видовой СК */                          
      MatrProjection;

    VEC PosCam;
} od6ANIM;

extern od6ANIM OD6_Anim;


/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( od6UNIT *Unit, od6ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( od6UNIT *Unit, od6ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( od6UNIT *Unit, od6ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( od6UNIT *Unit, od6ANIM *Ani );
 */
#define OD6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( od6UNIT *Unit, od6ANIM *Ani );       \
  VOID (*Close)( od6UNIT *Unit, od6ANIM *Ani );      \
  VOID (*Response)( od6UNIT *Unit, od6ANIM *Ani );   \
  VOID (*Render)( od6UNIT *Unit, od6ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagod6UNIT
{
  OD6_UNIT_BASE_FIELDS; /* Базовые поля */
};

/* Функция инициализации анимации */
BOOL OD6_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID OD6_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID OD6_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID OD6_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID OD6_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID OD6_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID OD6_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID OD6_AnimAddUnit( od6UNIT *Unit );

/* Функция создания объекта анимации */
od6UNIT * OD6_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
