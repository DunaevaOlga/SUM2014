 /* FILENAME: IMAGE.H
 * PROGRAMMER: OD6
 * PURPOSE: Image handle declaration module.
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* ��������� �������� ����������� */
typedef struct tagIMAGE
{
  INT W, H;    /* ������ ����������� */
  HBITMAP hBm; /* ���������� ������������ ����������� */
  DWORD *Bits; /* ����� ����� */
} IMAGE;

/* ������� �������� ����������� */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* ������� ������������ ������ ��-��� ����������� */
VOID ImageFree( IMAGE *Img );

/* ������� ��������� ����� ����� ����������� */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
