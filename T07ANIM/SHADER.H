/* FILENAME: SHADER.H
 * PROGRAMMER: OD6
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 25.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Функция инициализации шейдерной программы */
UINT OD6_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Функция удаления шейдерной программы */
VOID OD6_ShadProgClose( UINT ProgId );

#endif