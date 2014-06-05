#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#pragma warning(disable: 4244)
#define N 4


void Swap( int *A, int *B)
{
  int sw = *A;

  *A = *B;
  *B = sw;
}

void Write ( int *P, int n )
{
  FILE *F;
  int i;
  
  if ((F = fopen("a.log", "a")) != NULL)
  {
  for (i = 0; i < N; i++) 
    fprintf (F, "%i ", P[i]);
  if (n == 0)
    fprintf (F, "Parity");
  else
    fprintf (F, "Not parity");
  fprintf (F, "\n");
  fclose (F);
  }
}

void Go (int Pos, int *P, int Parity)   
{
  int i;

  if (Pos == N)
    Write (P, Parity);
  else
  {
    Go(Pos + 1, P, Parity);
    for (i = Pos + 1; i < N; i++)
    {
      Swap (&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1, P, Parity);
      Swap (&P[Pos], &P[i]);
      Parity = !Parity; 
    } 
  }
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  static int P[N], i;
  static int Parity = 0;
  for (i = 0; i < N; i++)
    P[i] = i+1;
  Go(0, P, Parity);
  return 0;
}