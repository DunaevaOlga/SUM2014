/* FILENAME: ANIM.C
 * PROGRAMMER: OD6
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>
#include <windows.h>

#pragma comment(lib, "winmm")  
#include <mmsystem.h>


#include "anim.h"

/* Системный контекст анимации */
static od6ANIM OD6_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  OD6_Anim.hWnd = hWnd;
  OD6_Anim.hDC = CreateCompatibleDC(hDC);
  OD6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  OD6_Anim.W = 30;
  OD6_Anim.H = 30;
  OD6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'OD6_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < OD6_Anim.NumOfUnits; i++)
  {
    OD6_Anim.Units[i]->Close(OD6_Anim.Units[i], &OD6_Anim);
    free(OD6_Anim.Units[i]);
  }
  DeleteObject(OD6_Anim.hBmFrame);
  DeleteDC(OD6_Anim.hDC);
  memset(&OD6_Anim, 0, sizeof(od6ANIM));
} /* End of 'OD6_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(OD6_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(OD6_Anim.hBmFrame);
  OD6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(OD6_Anim.hDC, OD6_Anim.hBmFrame);

  /* Сохранение размера */
  OD6_Anim.W = W;
  OD6_Anim.H = H;

  ReleaseDC(OD6_Anim.hWnd, hDC);
} /* End of 'OD6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Обновление ввода */
  GetKeyboardState(OD6_Anim.Keys);
  for (i = 0; i < 256; i++)
    OD6_Anim.Keys[i] >>= 7;


  /*Таймер*/
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(OD6_Anim.JButsOld, OD6_Anim.JButs, sizeof(OD6_Anim.JButs));
        for (i = 0; i < 32; i++)
          OD6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          OD6_Anim.JButsClick[i] = OD6_Anim.JButs[i] && !OD6_Anim.JButsOld[i];

        /* Оси */
        OD6_Anim.JX = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;
        OD6_Anim.JY = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASZ)
          OD6_Anim.JZ = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASR)
          OD6_Anim.JR = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASU)
          OD6_Anim.JU = 2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            OD6_Anim.JPOV = 0;
          else
            OD6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* Обновление таймера */
  OD6_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  OD6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  OD6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (OD6_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    OD6_Anim.DeltaTime = 0;
  }
  else
    OD6_Anim.DeltaTime = OD6_Anim.GlobalDeltaTime;

  OD6_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    OD6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* очистка фона */
  SelectObject(OD6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(OD6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(OD6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(OD6_Anim.hDC, 0, 0, OD6_Anim.W, OD6_Anim.H);

  /* опрос на изменение состояний объектов */
  for (i = 0; i < OD6_Anim.NumOfUnits; i++)
    OD6_Anim.Units[i]->Response(OD6_Anim.Units[i], &OD6_Anim);

  /* рисование объектов */
  for (i = 0; i < OD6_Anim.NumOfUnits; i++)
  {
    SelectObject(OD6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(OD6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(OD6_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(OD6_Anim.hDC, RGB(55, 155, 255));
    OD6_Anim.Units[i]->Render(OD6_Anim.Units[i], &OD6_Anim);
  }

  FrameCounter++;
} /* End of 'OD6_AnimRender' function */


/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(OD6_Anim.hWnd);
  BitBlt(hDC, 0, 0, OD6_Anim.W, OD6_Anim.H, OD6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(OD6_Anim.hWnd, hDC);
} /* End of 'OD6_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       od6UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimAddUnit( od6UNIT *Unit )
{
  if (OD6_Anim.NumOfUnits < OD6_MAX_UNITS)
  {
    OD6_Anim.Units[OD6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &OD6_Anim);
  }
} /* End of 'OD6_AnimAddUnit' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(OD6_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(OD6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(OD6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(OD6_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(OD6_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'OD6_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimSetPause( BOOL NewPauseFlag )
{
  OD6_Anim.IsPause = NewPauseFlag;
} /* End of 'OD6_AnimSetPause' function */




/* END OF 'ANIM.C' FILE */
