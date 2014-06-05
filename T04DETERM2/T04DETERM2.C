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


void Swap( double *C, double *B )
{
  int sw = *C;

  *C = *B;
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

/*void Go (int Pos )   
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
} */

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  int i, j, ki, kj, k, maxi, maxj, sign = 1;
  
  Load("a.log");
  /*for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);*/
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
    {
      maxi = i;
      maxj = j;

      for (ki = i + 1; ki < N; ki++)
        for (kj = i + 1; kj < N; kj++)
          if (A[ki][kj] > A[maxi][maxj])
            maxi = ki; maxj = kj;

      if (i != maxi)
        for (j = 0; j < N; j++)
          Swap(&A[i][j], &A[maxi][j]), sign = -sign;
      if (j != maxj)
        for (i = 0; i < N; i++)
          Swap(&A[i][j], &A[i][maxj]), sign = -sign;
      if (A[i][i] == 0)
        return 0;

      for (k = i + 1; k < N; k++)
        for (j = i; j < N; j++)
          A[k][j] = A[i][j] * A[k][i] / A[i][i]; 
    }
   
  for (i = 0, j = 0; i < N; i++, j++)
      GlobaDeterminant += A[i][j];
  Write(GlobaDeterminant * sign); 
  return 0;
}



