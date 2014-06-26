/* FILENAME: GEOMHAND.C
 * PROGRAMMER: OD6
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 25.06.2014
 */

#include <stdlib.h>

#include "anim.h"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       od6GEOM *G;
 *   - добавляемый материал:
 *       od6MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT OD6_GeomAddMaterial( od6GEOM *G, od6MATERIAL *Mtl )
{
  INT i;
  od6MATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(od6MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(od6MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'OD6_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       od6GEOM *G;
 *   - добавляемый примитив:
 *       od6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT OD6_GeomAddPrim( od6GEOM *G, od6PRIM *Prim )
{
  od6PRIM *P;

  if ((P = malloc(sizeof(od6PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(od6PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'OD6_GeomAddPrim' function */

/* Функция освобождения.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       od6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_GeomFree( od6GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    OD6_PrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(od6GEOM));
} /* End of 'OD6_GeomFree' function */

/* Функция рисования.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       od6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_GeomDraw( od6GEOM *G )
{
  INT i, loc;
  MATR WVP, MatrWorldInvTrans = {{{0}}};
  VEC V;

  /* вычислили матрицы преобразования */
  WVP = MatrMulMatr(OD6_Anim.MatrWorld,
    MatrMulMatr(OD6_Anim.MatrView, OD6_Anim.MatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(OD6_Anim.MatrWorld));

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(OD6_ShaderProg);
  loc = glGetUniformLocation(OD6_ShaderProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);

  loc = glGetUniformLocation(OD6_ShaderProg, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MatrWorldInvTrans.A[0]);

  loc = glGetUniformLocation(OD6_ShaderProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, OD6_Anim.MatrWorld.A[0]);

  loc = glGetUniformLocation(OD6_ShaderProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, OD6_Anim.MatrView.A[0]);

  loc = glGetUniformLocation(OD6_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, OD6_Anim.Time);

  V = VecSet(-OD6_Anim.MatrView.A[0][2],
             -OD6_Anim.MatrView.A[1][2],
             -OD6_Anim.MatrView.A[2][2]);
  loc = glGetUniformLocation(OD6_ShaderProg, "ViewDir");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);
  V = VecSet(OD6_Anim.MatrView.A[3][0],
             OD6_Anim.MatrView.A[3][1],
             OD6_Anim.MatrView.A[3][2]);
  loc = glGetUniformLocation(OD6_ShaderProg, "ViewPos");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);


  loc = glGetUniformLocation(OD6_ShaderProg, "NumOfParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrims);
  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    loc = glGetUniformLocation(OD6_ShaderProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);

    /* подготавливаем материал */
    if (G->Mtls != NULL && mtl >= 0 && mtl < G->NumOfMtls)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        IMAGE Img;
                       
        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* отправляем картинку в видеопамять */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(OD6_ShaderProg, "IsTextureUsed");
        if (loc != -1)
          glUniform1f(loc, 1);
        loc = glGetUniformLocation(OD6_ShaderProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        //glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      else
      {
        loc = glGetUniformLocation(OD6_ShaderProg, "IsTextureUsed");
        if (loc != -1)
          glUniform1f(loc, 0);
      }
      loc = glGetUniformLocation(OD6_ShaderProg, "MtlId");
      if (loc != -1)
        glUniform1i(loc, G->Mtls[mtl].Id);
      loc = glGetUniformLocation(OD6_ShaderProg, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(OD6_ShaderProg, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(OD6_ShaderProg, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(OD6_ShaderProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(OD6_ShaderProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    loc = glGetUniformLocation(OD6_ShaderProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    loc = glGetUniformLocation(OD6_ShaderProg, "PrimId");
    if (loc != -1)
      glUniform1i(loc, G->Prims[i].Id);
    OD6_PrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'OD6_GeomDraw' function */

/* Функция преобразования вершин.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       od6GEOM *G;
 *   - матрица преобразования:
 *       MATR M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OD6_GeomTransform( od6GEOM *G, MATR M )
{
  INT i, j;
  MATR InvM = MatrTranspose(MatrInverse(M));

  InvM.A[3][0] = InvM.A[3][1] = InvM.A[3][2] = 0;

  for (i = 0; i < G->NumOfPrims; i++)
    for (j = 0; j < G->Prims[i].NumOfV; j++)
    {
      G->Prims[i].V[j].P = VecMulMatr(G->Prims[i].V[j].P, M);
      G->Prims[i].V[j].N = VecMulMatr(G->Prims[i].V[j].N, InvM);
    }
} /* End of 'OD6_GeomTransform' function */

/* END OF 'GEOMHAND.C' FILE */
