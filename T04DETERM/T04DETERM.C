#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#pragma warning(disable: 4244)

#define MAX 10

double A[MAX][MAX];
int P[MAX], Parity;
int N;
double GlobaDeterminant;

void Load( char *FileName )
{
  FILE *F;
  int i, j;

  N = 0; 
  F = fopen(FileName, "r");
  if (F == NULL)
    return;
  fscanf(F, "%i", &N);
  if (N < 0)
    N = 0;
  else
    if (N > MAX)
      N = MAX;
  
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
}


void Swap( int *A, int *B )
{
  int sw = *A;

  *A = *B;
  *B = sw;
}

void Write ( double x )
{
  FILE *F;
  if ((F = fopen("a.log", "a")) != NULL)
  {
    fprintf (F, "\n"); 
    fprintf (F, "%f ", x);
    fprintf (F, "\n");
    fclose (F);
  }
}

void Go (int Pos )   
{
  int i;
  double prod = 1;

  if ( Pos == N )
  {
    for (i = 0; i < N; i++)
      prod *=A[i][P[i]];
    if (!Parity)
      GlobaDeterminant +=prod;
    else
      GlobaDeterminant -=prod;
  } 
  else
  {
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++)
    {
      Swap (&P[Pos], &P[i]);
      Parity = !Parity;
      Go(Pos + 1);
      Swap (&P[Pos], &P[i]);
      Parity = !Parity; 
    } 
  }
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  int i, j;
  
  Load("a.log");
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  Write(GlobaDeterminant); 
  return 0;
}



