#include <windows.h>
#include <math.h>
#pragma warning(disable: 4244)
#define Pi  3.14159265358979323846
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



void DrawGlobe (HDC hDC, int R, int W, int H, int T)
{
  static HANDLE hCons;

  double  fi, teta;

  SelectObject(hDC, GetStockObject(NULL_PEN));
  
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCBrushColor(hDC, RGB(0, 0, 0));

  Rectangle(hDC, 0, 0, W, H);
  
 
  for (fi = 0; fi <=  Pi * 2 ; fi += 0.05)
    for (teta = 0; teta <= Pi  ; teta += 0.05)
      if (R * sin(teta) * cos(fi + T) > 0)
        SetPixel(hDC, R * sin(fi + T)* sin(teta) + W / 2 , R * cos(teta) + H / 2, RGB(0, 255, 0));
  
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC/*, hMemDC, hMemDC1*/;
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;
  SYSTEMTIME st;
  static int W, H;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch(Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 60, NULL);
    return 0;

  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;


  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);

    GetClientRect(hWnd, &rc);
 
    GetLocalTime(&st);
    DrawGlobe(hMemDC, 0.3 * rc.right , rc.right, rc.bottom, st.wMilliseconds);
                                                            
  
    EndPaint(hWnd, &ps);
    return 0;

  /*case WN_CHAR:
    if ((CHAR) wParam == 27)
      DestroyWindow(hWnd);
    return 0; */
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject (hBm);
    DeleteDC(hMemDC);
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/*End of 'MyWindowFunc' function*/