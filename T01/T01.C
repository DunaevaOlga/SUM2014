#include <windows.h>
#include <math.h>
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
    2000, 100,
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
  int xx, yy;
  float 
    len, si, co,
    ratio = (float)W / H;

  len = sqrt(SQR(Xc - Xm) + SQR((Yc - Ym) * ratio));

  if ( len == 0)
    si = 0, co = 0;
  else
    co = (Xm - Xc) / len, si = (Ym - Yc) * ratio / len;
 
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

 
  if (len > W / 2 - R)
    len = W / 2 - R;
  xx = Xc + co * len;
  yy = Yc + si * len / ratio;

  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, xx - R, yy - R / ratio, xx + R, yy + R / ratio);

  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(0, 0, 0));

  Ellipse(hDC, Xc - W / 2, Yc - H / 2, Xc + W / 2, Yc + H / 2);

}                        


float ro( void )
{
  return (float)rand() / RAND_MAX;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC, hMemDC, hMemDC1;
  PAINTSTRUCT pc;
  RECT rc;
  POINT pt;

  int x0, y0, x1, y1, i;

  switch(Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 50, NULL);
    return 0;
 
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &pc);
    GetClientRect(hWnd, &rc);

    
   
    srand(30);
    SelectObject(hDC, GetStockObject(NULL_PEN));
   /* SelectObject(hDC, GetStockObject(DC_BRUSH)); */

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
     
    DrawEye(hDC, pt.x, pt.y, ro() * rc.right, ro() * rc.bottom, rand() % 20, rand() % 20, rand() % 20);
  
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