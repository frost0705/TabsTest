/*	exarray.cpp

	������������ ������� � ��������� �
	�������������� ��������� �������
	(����������� �������).
	������� ������������� ������.

	(C) �.�.�������, ���� ��� PAH, 1998 - 2000
	All Rights Reserved.
*/
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <new.h>

//	������� exmrealloc ��������, ��������,
//	����������� � ����������� ����� ������,
//	����������� ������.
//	��� ������������ ��������� ����� �������
//	���������� ������� abort().
//	��� �������� ������ ���������� �������,
//	������������� �� set_new_handler ��
//	new.h, � ���� �� ��� - �� abort().

void	exmrealloc (void **p, unsigned size,
	 		      unsigned oldsize)
{
  void *pp = *p;
  if (size)		// ��������� ������.
  {
    if (size > (~0u)-9)	// ����� ������ realloc().
      abort();
    while ((pp = realloc (*p, size)) == NULL)
    {			// ����� new_handler().
      typedef void (*pvfunc) (void);
      pvfunc pvf = set_new_handler (NULL);
      set_new_handler (pvf);
      if (pvf) (*pvf)(); else abort();
    }
    if (size > oldsize)
      memset           	// ���������.
       ((char*)pp + oldsize, 0, size - oldsize);
  }
  else			// �c���������� ������.
  { 
    if (pp) { free (pp); pp = NULL; }
  }
  *p = pp;
}

//	������� excalcsize ��������� ������ �����
//	������ � ������, ������� ������ ���� ��
//	������ ����������, ��� ���� ���������
//	������ SIZE_MOD ���������� �� 2 � 1.5.
//	��� ������������ �������� ~0u.
//	��� ���������� ������������ ������������
//	������ ����������� ������ ��������� ������
//	SIZE_SYS, ����������� �������� realloc �
//	Borland C++ 3.1, 4.5.
//	��� ����������� ������ ���� L1 ������c����
//	Pentuim SIZE_MOD �������� ��� 64**n +/- 16.

#define SIZE_MOD (112)
#define SIZE_SYS (sizeof(int) * 2)

unsigned excalcsize (unsigned size)
{
  unsigned n = SIZE_MOD, k = 0;
  for (;; k = ~k, (k? (n <<= 1): (n += (n >> 1))))
  {
    n -= SIZE_SYS; if (n  >= size) break;
    n += SIZE_SYS; if ((int)n < 0) {n =~0u; break;}
  }
  return (n);
}

//	������ - ���������, ����������
//	������� ��������� � ������� ��������.

class	{ void* e; unsigned len; } __EXNULL;
