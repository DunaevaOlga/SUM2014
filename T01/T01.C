#include <windows.h>
#include <math.h>
#pragma warning(disable: 4244)
#define SQR(X) ((X) * (X))
#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );



INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = 
    CreateWindow(WND_CLASS_NAME,
    "Title",
    WS_OVERLAPPEDWINDOW,
    1920, 100,
    800, 800,
    NULL,
    NULL,
    hInstance,
    NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}/*End of 'WinMain' function*/



void DrawEye (HDC hDC, int Xm, int Ym, int Xc, int Yc, int W, int H, int R)
{
  static HANDLE hCons;

  int xx, yy;
  float 
    len, si, co,
    ratio = (float)W / H;

  len = (float)sqrt(SQR(Xc - Xm) + SQR((Yc - Ym) * ratio));

  if ( len == 0)
    si = 0, co = 0;
  else
    co = (Xm - Xc) / len, si = (Ym - Yc) * ratio / len;
 
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, Xc - W / 2, Yc - H / 2, Xc + W / 2, (Yc + H / 2));
 
  if (len > W / 2 - R)
    len = (float)(W / 2 - R);
  xx = (int)(Xc + co * len);
  yy = (int)(Yc + si * len / ratio);

  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, (int)(xx - R), (int)(yy - R / ratio), (int)(xx + R), (int)(yy + R / ratio));   
}


float ro( void )
{
  return (float)rand() / RAND_MAX;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC/*, hMemDC, hMemDC1*/;
  PAINTSTRUCT pc;
  RECT rc;
  POINT pt;
  INT n, a;

  switch(Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 50, NULL);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;


  case WM_PAINT:
    hDC = BeginPaint(hWnd, &pc);
    GetClientRect(hWnd, &rc);
    srand(5);

    SelectObject(hDC, GetStockObject(NULL_PEN));

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    
    n = 4;
    while (n-- > 0)
      DrawEye(hDC, pt.x, pt.y, (int)(ro() * rc.right), (int)(ro() * rc.bottom), (rand() % rc.right ) + 30, rand() % rc.bottom), 30);
  
    EndPaint(hWnd, &pc);
    return 0;

  /*case WN_CHAR:
    if ((CHAR) wParam == 27)
      DestroyWindow(hWnd);
    return 0; */

  case WM_DESTROY:
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/*End of 'MyWindowFunc' function*/