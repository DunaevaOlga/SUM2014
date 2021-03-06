/* FILENAME: ANIM.C
 * PROGRAMMER: OD6
 * PURPOSE: Base animation module
 * LAST UPDATE: 13.06.2014
 */

#include <stdlib.h>
#include <time.h>
#include <windows.h>

#pragma comment(lib, "winmm")  
#include <mmsystem.h>


#include "anim.h"


/* Системный контекст анимации */
od6ANIM OD6_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

static INT
  OD6_MouseGlobalX, OD6_MouseGlobalY, 
  OD6_MouseXOld, OD6_MouseYOld, 
  OD6_MouseGlobalWheel; 

static HHOOK OD6_hMouseHook;

UINT OD6_ShaderProg;

static LRESULT CALLBACK OD6_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    OD6_MouseGlobalX = hs->pt.x;
    OD6_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    OD6_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return CallNextHookEx(OD6_hMouseHook, Code, wParam, lParam);;
} /* End of 'OD6_MouseHook' function */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
BOOL OD6_AnimInit( HWND hWnd )
{
  int i;
  DBL Dist = 1.0;
  LARGE_INTEGER li;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Устанавливаем оконные параметры */
  OD6_Anim.hDC = GetDC(hWnd);
  OD6_Anim.hWnd = hWnd;
  OD6_Anim.W = 30;
  OD6_Anim.H = 30;
  OD6_Anim.NumOfUnits = 0;

  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(OD6_Anim.hDC, &pfd);
  DescribePixelFormat(OD6_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(OD6_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  OD6_Anim.hRC = wglCreateContext(OD6_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(OD6_Anim.hDC, OD6_Anim.hRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(OD6_Anim.hRC);
    ReleaseDC(OD6_Anim.hWnd, OD6_Anim.hDC);
    memset(&OD6_Anim, 0, sizeof(od6ANIM));
    return FALSE;
  }
  /* параметры OpenGL по-умолчанию */
  glEnable(GL_DEPTH_TEST);

  /*glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);  */


  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  /* Начальные значения параметров проецирования */
  OD6_Anim.Wp = 4;
  OD6_Anim.Hp = 3;
  OD6_Anim.ProjDist = 1;
  OD6_Anim.ProjSize = 1;
  OD6_Anim.FarClip = 10000;
  OD6_Anim.MatrWorld = OD6_Anim.MatrView = OD6_Anim.MatrProjection = MatrIdenity();
  return TRUE;
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

  OD6_ShadProgClose(OD6_ShaderProg);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(OD6_Anim.hRC);
  ReleaseDC(OD6_Anim.hWnd, OD6_Anim.hDC);
/*  DeleteObject(OD6_Anim.hWnd, OD6_Anim.hDC); */
 //* ReleaseDC(OD6_Anim.hDC); */
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
  DBL Size = 1, ratio_x = 1, ratio_y = 1;
  /* Сохранение размера */
  OD6_Anim.W = W;
  OD6_Anim.H = H;
  /* Поле просмотра */
  glViewport(0, 0, W, H);

  if (W > H)
    ratio_x = (DBL)W / H;
  else 
    ratio_y = (DBL)H / W;
  OD6_Anim.Wp = OD6_Anim.ProjSize * ratio_x;
  OD6_Anim.Hp = OD6_Anim.ProjSize * ratio_y;


  OD6_Anim.MatrProjection =
      MatrProjection(-OD6_Anim.Wp / 2, OD6_Anim.Wp / 2,
                     -OD6_Anim.Hp / 2, OD6_Anim.Hp / 2,
                      OD6_Anim.ProjDist, OD6_Anim.FarClip);


} /* End of 'OD6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  MATR WVP;


  /* Обновление ввода */
  GetKeyboardState(OD6_Anim.Keys);
  for (i = 0; i < 256; i++)
    OD6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    OD6_Anim.KeysClick[i] = OD6_Anim.Keys[i] && !OD6_Anim.KeysOld[i];
  memcpy(OD6_Anim.KeysOld, OD6_Anim.Keys, 256);

  OD6_Anim.MsWheel = OD6_MouseGlobalWheel;
  OD6_MouseGlobalWheel = 0;

  pt.x = OD6_MouseGlobalX;
  pt.y = OD6_MouseGlobalY;
  ScreenToClient(OD6_Anim.hWnd, &pt);
  OD6_Anim.MsX = pt.x;
  OD6_Anim.MsY = pt.y;
  
  OD6_Anim.MsDeltaX = OD6_MouseGlobalX - OD6_MouseXOld;
  OD6_Anim.MsDeltaY = OD6_MouseGlobalY - OD6_MouseYOld;
  OD6_MouseXOld = OD6_MouseGlobalX;
  OD6_MouseYOld = OD6_MouseGlobalY;

  /*Джойстик*/
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

  /* опрос на изменение состояний объектов */
  for (i = 0; i < OD6_Anim.NumOfUnits; i++)
    OD6_Anim.Units[i]->Response(OD6_Anim.Units[i], &OD6_Anim);

  /* очистка фона */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 /* /* рисуем оси координат */  /*
  glUseProgram(0);
  OD6_Anim.MatrWorld = MatrIdenity();
  WVP = MatrMulMatr(OD6_Anim.MatrWorld, MatrMulMatr(OD6_Anim.MatrView, OD6_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_TEXTURE_2D);
  glLineWidth(2);
  glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();
  glLineWidth(1);
  glBegin(GL_LINES);
    glColor3d(1, 1, 1);
    for (i = -100; i <= 100; i++)
    {
      glVertex3d(i, 0, -100);
      glVertex3d(i, 0, 100);
      glVertex3d(-100, 0, i);
      glVertex3d(100, 0, i);
    }
  glEnd();                 
  glPopAttrib();           */

  /* рисование объектов */
  for (i = 0; i < OD6_Anim.NumOfUnits; i++)
    OD6_Anim.Units[i]->Render(OD6_Anim.Units[i], &OD6_Anim);

  FrameCounter++;

   UnhookWindowsHookEx(OD6_hMouseHook);
} /* End of 'OD6_AnimRender' function */


/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_AnimCopyFrame( VOID )
{
  /* вывод кадра */
  glFinish();
  SwapBuffers(OD6_Anim.hDC);
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
      rc.right - rc.left, rc.bottom - rc.top,
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
