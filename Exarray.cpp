/* --------------------------------------------------------------
    ������������ ������� � ��������� ������� (������ 1.2c).
    ������� ������������� ������.

    http://www.imach.uran.ru/exarray

    Copyright 1998-2010, �.�.�������, ���� ��� PAH.
    All Rights Reserved.

    ��� ��������������� �� �������� �������� "public domain",
    �.�. ����������� ��������� ����������, ����������� �
    ��������������� ���� ��� ������� ���������� ���������
    � ���������� �������� � ���� ��������� ����������.

    ������ ����������� ����������� ��������������� "��� ����"
    ��� �����-���� ��������, ����� ��� ���������������. ��
    ����������� ��� �� ���� ����������� ����� � ����. �����
    �� ����� ����� ��������������� �� ������ ������, �����,
    ������ ��� ����� ������ ���� ������ ��� ����� ��������
    ������������� ������� ������������ �����������.
-------------------------------------------------------------- */
#include "stdafx.h"
#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#define EXALLOC_OUT_OF_LINE
#include "Exarray.h"

#ifdef  _MSC_VER
#pragma auto_inline (off)
#endif/*_MSC_VER*/

/* --------------------------------------------------------------
    ��������� ��������� ������������.
-------------------------------------------------------------- */

#ifndef _mm_malloc
#define _aligned_malloc(p,b,a)  malloc(b)
#define _aligned_realloc(p,b,a) realloc(p,b)
#define _aligned_free(p)        free(p)
#endif/*_mm_malloc*/

/* --------------------------------------------------------------
    ������� ������ ���������, ��� "C", �����
    ������������� �������������� ����.
    ��� ��������� ���������� ��������� ������
    ����� ���������� �������� ������������.
-------------------------------------------------------------- */

#ifdef  __cplusplus
extern "C" {
#endif/*__cplusplus*/

/* --------------------------------------------------------------
    �������� ������������� ������� ��� ������������
    ������������ ���������� �������� NULL.
-------------------------------------------------------------- */

struct  exblock_t VDECL exnull_c;

/* --------------------------------------------------------------
    ������� set_exalloc_handler() �������������
    ���������� ������ ������������� ������,
    ����������� set_new_handler().

    ���������:
    pvf     ��������� �� ������� ���� void name (void) ���
            NULL ���� ��� ��������� ������ ������� �������
            (*exalloc_status.size_handler)();

    ������: ���������� ���������.
-------------------------------------------------------------- */

void (* set_exalloc_handler (void (*pvf) (void))) (void)
{
    static void (* pvfFailHandler)(void) = NULL;
    void (*pvfPrev) (void) = pvfFailHandler;
    pvfFailHandler = pvf;
    return pvfPrev;
}

/* --------------------------------------------------------------
    ������� exalloc_size_fail() �������� ���������� ������
    ������������� ������ �� ��� ���, ���� �� �� ����� NULL,
    ����� �������� abort().
-------------------------------------------------------------- */

void exalloc_size_fail (void)
{
    for (;;)
    {
        void (*pvf) (void) = set_exalloc_handler (NULL);
        set_exalloc_handler (pvf);
        if (pvf == NULL) break;
        (*pvf)();
    }
    abort();
}

/* --------------------------------------------------------------
    ������� exalloc_stub() �� ������ ������.
-------------------------------------------------------------- */

void exalloc_stub (void) {}

/* --------------------------------------------------------------
    ������� exalloc_null_fail() �������� abort.
-------------------------------------------------------------- */

static void exalloc_null_fail (void)
{
    #define NULL_POINTER_INDIRECTION 0
    assert (NULL_POINTER_INDIRECTION);
    abort();
}

/* --------------------------------------------------------------
    ������� exalloc_range_fail() �������� abort.
-------------------------------------------------------------- */

static void exalloc_range_fail (void)
{
    #define INDEX_RANGE_ERROR 0
    assert (INDEX_RANGE_ERROR);
    abort();
}

/* --------------------------------------------------------------
    ����������� ������� ������ � ��������
    o ������������� ������.

    ��������� �������, ���������� ��� ������������
    ������ ������������� ������. ������� ������ ���������
    ���������� ��� ���������� ������ ���������.

    ��������� ��������������� �������, ����������
    ��� �������������� ���������� ������� �������
    ��� NULL, ���� ����� ��������������� �������
    �� ���������. ����� ����������� ��� ����������
    ��������������� ���������� �������.

    ��������� �������, ���������� ��� ���������
    �� NULL-���������. ������� ������ ���������
    ���������� ��� ���������� ������ ���������.

    ��������� �������, ���������� ��� ������
    ������� �� ������� ������� � ��� ������, ����
    �� ������������� �������������� ����������
    ������� �������. ������� ������ ���������
    ���������� ��� ���������� ������ ���������.
-------------------------------------------------------------- */

struct exalloc_status_t VDECL exalloc_status =
    { exalloc_size_fail, exalloc_stub,
      exalloc_null_fail, exalloc_range_fail,
      0, 0, 0, 0 };

/* --------------------------------------------------------------
    ������� exmalloc �������� ���� ������, ����������� ������.

    ��� ������������ ��������� ����� ��� �������� ������
    ���������� ���������� ������, ������������� ��
    exalloc_set_handler, ����� �����������
    exalloc_status.size_handler.

    ���������:
    blocksize       ������ ����� � ������.

    ������:         ��������� �� ���� ��� NULL.
-------------------------------------------------------------- */

void*   FCDECL  exmalloc (size_t blocksize)
{
    void* p = NULL;
    exaligned_mrealloc (&p, blocksize, 1, 0, blocksize);
    return (p);
}

/* --------------------------------------------------------------
    ������� exaligned_malloc �������� ����������� ���� ������,
    ����������� ������.

    ��� ������������ ��������� ����� ��� �������� ������
    ���������� ���������� ������, ������������� ��
    exalloc_set_handler, ����� �����������
    exalloc_status.size_handler.

    ���������:
    blocksize       ������ ����� � ������.
    blockalign      ������������ ����� � ������ (2**n).

    ������:         ��������� �� ���� ��� NULL.
-------------------------------------------------------------- */

void*   FCDECL  exaligned_malloc (size_t blocksize, size_t blockalign)
{
    void* p = NULL;
    exaligned_mrealloc (&p, blocksize, blockalign, 0, blocksize);
    return (p);
}

/* --------------------------------------------------------------
    ������� exmrealloc() ��������, ��������, ����������� ���
    ����������� ���� ������, � ����� �������� �������� �������.

    ��� ������������ ��������� ����� ��� �������� ������
    ���������� ���������� ������, ������������� ��
    exalloc_set_handler, ����� �����������
    exalloc_status.size_handler.

    ���������:
    p               ����� ��������� �����.
                    *p == NULL ��� �������������.
                    *p != NULL ��� �����������������.
                    �������� *p ��������������.
    blocksize       ����� ������ ����� ��� 0
                    ���� ���� ������ ���� ����������.
    memset_start    ������ ������� ���������.
    memset_stop     ����� ������� ��������� (�� �������).
-------------------------------------------------------------- */

void    FCDECL  exmrealloc (void** p, size_t blocksize,
                 size_t memset_start, size_t memset_stop)
{
    exaligned_mrealloc (p, blocksize, 1, memset_start, memset_stop);
}

/* --------------------------------------------------------------
    ������� exaligned_mrealloc() ��������, ��������, �����������
    ��� ����������� ����������� ���� ������, � ����� ��������
    �������� �������.

    ��� ������������ ��������� ����� ��� �������� ������
    ���������� ���������� ������, ������������� ��
    exalloc_set_handler, ����� �����������
    exalloc_status.size_handler.

    ���������:
    p               ����� ��������� �����.
                    *p == NULL ��� �������������.
                    *p != NULL ��� �����������������.
                    �������� *p ��������������.
    blocksize       ����� ������ ����� ��� 0
                    ���� ���� ������ ���� ����������.
    blockalign      ������������ ����� � ������ (2**n).
    memset_start    ������ ������� ���������.
    memset_stop     ����� ������� ��������� (�� �������).
-------------------------------------------------------------- */

void    FCDECL  exaligned_mrealloc (void** p,
                 size_t blocksize, size_t blockalign,
                 size_t memset_start, size_t memset_stop)
{
    void* pp = *p;                /* �������� p */

    /* ��������� ������, ���� blocksize != 0 */

    if (blocksize)
    {
        /* (����)���������� ���� � ��������� ��������� */

        while (blocksize > EXCALCBLOCKSIZE_MAX - blockalign ||
              (pp = _aligned_realloc (*p, blocksize, blockalign))
              == NULL)
        {
            /* ������� ���������� ������ �������������
               ������ � ���������� ������� ���������� */

            void (*pvf) (void) = set_exalloc_handler (NULL);
            set_exalloc_handler (pvf);
            if (pvf != NULL) { (*pvf)(); continue; }

            /* ������� ���������� ������������ ������
               ������������� ������ � ����� */

            (*exalloc_status.size_handler)();
            exalloc_status.nBlocksFailed++;
            return;
        }

        /* ��������� ������������ */

        if (((size_t)pp & (blockalign - 1)) != 0) {}

        /* ��������� ����� ����������� ������ */

        if (pp != NULL)
        {
            exalloc_status.nBlocksAllocated++;
        }

        /* ������������� */

        if (memset_stop > memset_start)
        {
             memset ((char*)pp + memset_start, 0,
                   memset_stop - memset_start);
        }

        /* �������� ���������� � ��������� ������ */

        if (exalloc_status.pMinAlloc - 1 >= (char*) pp)
        {
            exalloc_status.pMinAlloc = (char*)pp;
        }
        if (exalloc_status.pMaxAlloc < (char*)pp + blocksize)
        {
            exalloc_status.pMaxAlloc = (char*)pp + blocksize;
        }
    }

    /* ������������ ������ ���� blocksize == 0 � *p != NULL */

    else if (pp)
    {
        _aligned_free (pp); pp = NULL;
        exalloc_status.nBlocksAllocated--;
    }

    /* �������� *p */

    *p = pp;
}

/* --------------------------------------------------------------
    ������� exfree() ����������� ���� ������.

    ���������:
    p           ����� ����� ��� NULL.
-------------------------------------------------------------- */

void    FCDECL  exfree (void* p)
{
    exaligned_free (p);
}

/* --------------------------------------------------------------
    ������� exaligned_free() �����������
    ����������� ���� ������.

    ���������:
    p           ����� ����� ��� NULL.
-------------------------------------------------------------- */

void    FCDECL  exaligned_free (void* p)
{
    if (p)
    {
        _aligned_free (p);
        exalloc_status.nBlocksAllocated--;
    }
}

/* --------------------------------------------------------------
    ������� exrange() �������� exalloc_status.null_handler
    ��� exalloc_status.range_handler � ����������� �� p.

    ���������:
    p           ����� ����� ��� NULL.
-------------------------------------------------------------- */

void    FCDECL  exrange (void* p)
{
    if (p == NULL)
            (*exalloc_status.null_handler)();
    else    (*exalloc_status.range_handler)();
}

/* --------------------------------------------------------------
    ������� excalcblocksize() ������������
    ����������� ������ ����� ������ � ������,
    ������� ������ ���� �� ������ ����������.

    ��������� ������ EXCALCBLOCKSIZE_MOD
    ���������� ���������� �� 2 � 1.5, ����
    ������ ������ ������������.
    ��� ���������� ������������ ������������
    ������ ��� ������� ����������� ������
    ��������� ������ EXCALCBLOCKSIZE_SYS,
    �������� EXCALCBLOCKSIZE_MOD ��������
    ��� 64**n +/- 16, ������� ��������������
    ����� ����� ������ 16 ��� 32, �� �� 64.

    ���� ����������� ������ ������� �����,
    �� �������� EXSIZE_T_MAX.

    ���������:
    blocksize       ����������� ������ ����� � ������.

    ������:         ����������� ������ � ������ ���
                    EXSIZE_T_MAX.
-------------------------------------------------------------- */

size_t  FCDECL  excalcblocksize (size_t blocksize)
{
    size_t n = EXCALCBLOCKSIZE_MOD, k = 0;

    for (;; k = ~k, (k? (n <<= 1): (n += (n >> 1))))
    {
        n -= EXCALCBLOCKSIZE_SYS; if (n >= blocksize) break;
        n += EXCALCBLOCKSIZE_SYS; if ((ptrdiff_t) n < 0)
                               { n = EXSIZE_T_MAX; break; }
    }

    return (n);
}

/* --------------------------------------------------------------
    ������� exmuladd() ��������� s*n+k
    � �������� 0..EXSIZE_T_MAX.
-------------------------------------------------------------- */

size_t  FCDECL  exmuladd (size_t s, size_t n, size_t k)
{
    return ((n <= (EXSIZE_T_MAX - k) / s)? (n * s + k): EXSIZE_T_MAX);
}

/* --------------------------------------------------------------
    ������� exmul() ��������� s*n
    � �������� 0..EXSIZE_T_MAX.
-------------------------------------------------------------- */

size_t  FCDECL  exmul (size_t s, size_t n)
{
    return ((s == 1)? n: (n <= EXSIZE_T_MAX / s)? n * s: EXSIZE_T_MAX);
}

/* --------------------------------------------------------------
    ������� exadd() ��������� n+k
    � �������� 0..EXSIZE_T_MAX.
-------------------------------------------------------------- */

size_t  FCDECL  exadd (size_t n, size_t k)
{
    return ((n <= EXSIZE_T_MAX - k)? (n + k): EXSIZE_T_MAX);
}

/* --------------------------------------------------------------
    ������� exchkadd() ��������� n+k � �������� 0..max,
    ����� ���������� ���������� ������,
    ����������� exalloc_status.size_handler.
-------------------------------------------------------------- */

size_t  FCDECL  exchkadd (size_t n, size_t k, size_t max)
{
    if (n > max - k) (*exalloc_status.size_handler)();
    return (n + k);
}

#ifdef  __cplusplus
}
#endif/*__cplusplus*/
