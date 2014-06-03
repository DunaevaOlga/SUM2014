#include <windows.h>
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


float ro( void )
{
  return (float)rand() / RAND_MAX;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT pc;
  RECT rc;

  int x0, y0, x1, y1, i;

  switch(Msg)
  {
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &pc);
    GetClientRect(hWnd, &rc);
   
    srand(30);
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
       
    for (i = 0; i < 10; i++)
    { 
      SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256)); 
      x0 = ro() * rc.right;
      x1 = ro() * rc.right;
      y0 = ro() * rc.bottom;
      y1 = ro() * rc.bottom;
      Ellipse(hDC, x0, y0, x1, y1);
    }
    EndPaint(hWnd, &pc);
    return 0;

  /*case WN_CHAR:
    if ((CHAR) wParam == 27)
      DestroyWindow(hWnd);
    return 0; */

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/*End of 'MyWindowFunc' function*/