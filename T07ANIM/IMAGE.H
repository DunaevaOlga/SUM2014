/* FILENAME: IMAGE.H
 * PROGRAMMER: OD6
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Структура хранения изображения */
typedef struct tagIMAGE
{
  INT W, H;    /* Размер изображения */
  HBITMAP hBm; /* Дескриптор загруженного изображения */
  HDC hDC;
  DWORD *Bits; /* Цвета точек */
} IMAGE;

/* Функция загрузки изображения */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Функция освобождения памяти из-под изображения */
VOID ImageFree( IMAGE *Img );

/* Функция рисования изображения.*/
VOID ImageDraw( IMAGE *Img, HDC hDC, int X, int Y, int LogOp);

/* Функция получения цвета точки изображения */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
