/* --------------------------------------------------------------
    Signed integers with unlimited range (version 2.0).
    Base functions in machine-independent form.

    http://www.imach.uran.ru/cbignum

    Copyright 1999-2010 by Raul N.Shakirov, IMach of RAS(UB).
    All Rights Reserved.

    Permission has been granted to copy, distribute and modify
    software in any context without fee, including a commercial
    application, provided that the aforesaid copyright statement
    is present here as well as exhaustive description of changes.

    THE SOFTWARE IS DISTRIBUTED "AS IS". NO WARRANTY OF ANY KIND
    IS EXPRESSED OR IMPLIED. YOU USE AT YOUR OWN RISK. THE AUTHOR
    WILL NOT BE LIABLE FOR DATA LOSS, DAMAGES, LOSS OF PROFITS OR
    ANY OTHER KIND OF LOSS WHILE USING OR MISUSING THIS SOFTWARE.
-------------------------------------------------------------- */
#ifndef _CBIGNUMF_INL
#define _CBIGNUMF_INL

#ifndef _CBIGNUMF_H
#include "Cbignumf.h"
#endif

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#define DISALLOW_EXARRAY_SHORTTYPE
#include "Cbignum.h"
#define BITS    (CHAR_BIT * sizeof (CBNL))

static int count_floor2nBITS()
{
  int n = 2; while (n <= (BITS-1)/2) n <<= 1;
  return n;
}
static int floor2nBITSm1 = count_floor2nBITS() - 1;

#ifdef  _MSC_VER
#pragma auto_inline (off)
#endif

#ifdef  _CBIGNUM_ASM

#if 1
#define _CBIGNUM_OPTIMIZED_COPY
#endif

#endif

inline size_t size_max (size_t n1, size_t n2)
{
  return ((n1 > n2)? n1 : n2);
}

inline size_t size_min (size_t n1, size_t n2)
{
  return ((n1 < n2)? n1 : n2);
}

#ifdef  NCHECKPTR
#ifdef  __cplusplus
extern "C" {
#endif
#endif

cTTY    cBigNumberMessages;
cTTY    cBigNumberProgress;

void    cBigNumberDump (
                const   CBPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  for (; n1; n1--) cBigNumberMessages <<" "<< (const char*) cHexDump (p1 [n1]);
  cBigNumberMessages << "\n";
}

void    cBigNumberInfo (
                const   CBPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  cBigNumberMessages << cHexDump (n1? p1 [n1]: 0);
  if (n1 > 1)
    cBigNumberMessages << "... (" << (CBNL)(n1 * sizeof (CBNL)) << " bytes)\n";
}

void    cBigNumberERange  (
                const   CBPTR(CBNL) p1
        )
{
  cBigNumberMessages << "cBigNumber value out of range: ";
  cBigNumberInfo (p1);
  abort();
}

size_t  cBigNumberWords (
                const   CBPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  if (n1)
  {
    CBNL sign = p1 [n1] >> (BITS-1);
    do
    {
      if (p1 [n1] != sign) break;
    }
    while (--n1 != 0);
  }
  return (n1);
}

int     cLongBits (CBNL num)
{
  int n = 0;
  num ^= (num >> (BITS-1));
  int k = floor2nBITSm1;
  do
  {
    int k0 = (((num >> k) == 0) - 1) & (k + 1); num >>= k0; n += k0;
  }
  while ((k >>= 1) != 0);
  return (n += (num > 0));
}

int     cULongBits (unsigned CBNL num)
{
  int n = (((CBNL)num >= 0) - 1); num &= ~n;
      n &= BITS;
  int k = floor2nBITSm1;
  do
  {
    int k0 = (((num >> k) == 0) - 1) & (k + 1); num >>= k0; n += k0;
  }
  while ((k >>= 1) != 0);
  return (n += (num > 0));
}

CBNL    cBigNumberBits (
                const   CBPTR(CBNL) p1
        )

{
  size_t n = cBigNumberWords(p1);
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  if (n)
  {
    if ((CBNL)n > (CBNL)(CBNL_MAX / BITS))
       cBigNumberERange (p1);
    return ((CBNL)(n - 1) * BITS + cULongBits ((unsigned CBNL)
                                    (p1 [n] ^ (p1 [n1] >> (BITS-1)))));
  }
  return (0);
}

size_t  cBigNumberExWords (
                const   CBPTR(CBNL) p1
)
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  for (size_t n = 0; n < n1; n++)
  {
    if (p1 [n + 1] != 0) return (n);
  }
  return (0);
}

int     cLongExBits (CBNL num)
{
  int n = 0;
  if (num) while ((num & (CBNL)1) == 0) { ++n; num >>= 1; }
  return (n);
}

CBNL    cBigNumberExBits (
                const   CBPTR(CBNL) p1
        )

{
  size_t n = cBigNumberExWords(p1);
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  if (n1)
  {
    if ((CBNL)n > (CBNL)((CBNL_MAX - (BITS - 1)) / BITS))
      cBigNumberERange (p1);
    return ((CBNL)n * BITS + cLongExBits (p1 [n + 1]));
  }
  return (0);
}

int     cLongExactLog2 (CBNL num)
{
  int n = cLongExBits (num);
  if ((num >> n) != 1) n = -1;
  return n;
}

size_t _CBNL_C  cBigNumberIsFit (
                const   CBPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));

  if (n1 > 1) return ((p1 [n1] != (p1 [n1-1] >> (BITS-1))));

  return (n1);
}

size_t _CBNL_C _cBigNumberFit (
                        EXPTR(CBNL) p1
        )
{
  register size_t n1 = (size_t)(*p1);
  p1 += n1;

  if (n1)
  {
    register CBNL pass, lp = *p1;
    do continue;
    while (--n1 != 0 &&
        (pass = lp, lp = p1 [-1], --p1, pass == (lp >> (BITS-1))));
  }
  else { p1 [1] = 0; }

  return (n1 + 1);
}

size_t _CBNL_C _cBigNumberFitTo (
                        EXPTR(CBNL) p1,
                        size_t      n
        )
{
  register size_t n1 = (size_t)(*p1);
  register CBNL pass = p1 [n1] >> (BITS-1);

  do
    p1 [++n1] = pass;
  while (n1 < n);

  return (n1);
}

int    _CBNL_C  cBigNumberComp (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));
  register CBNL lp1 = 0;
  register CBNL lp2 = 0;

  if (n1) lp1 = p1 [n1--];
  if (n2) lp2 = p2 [n2--];

  if (n1 == n2)
  {
    if (lp1 != lp2) goto NotSameSignWord;
  }
  else if (n1 > n2)
  {
    lp2 >>= (BITS-1); ++n2;
    if (lp1 != lp2) goto NotSameSignWord;
    if (n1 > n2)
    {
      do
      {
        lp1 = p1 [n1]; --n1;
        if (lp1 != lp2) goto NotSame;
      }
      while (n1 > n2);
    }
  }
  else
  {
    lp1 >>= (BITS-1); ++n1;
    if (lp1 != lp2) goto NotSameSignWord;
    if (n1 < n2)
    {
      do
      {
        lp2 = p2 [n2]; --n2;
        if (lp1 != lp2) goto NotSame;
      }
      while (n1 < n2);
    }
  }

  if (n1)
  {
    do
    {
      lp1 = p1 [n1]; lp2 = p2 [n1]; --n1;
      if (lp1 != lp2) goto NotSame;
    }
    while (n1);
  }

  return (0);
NotSame:
  return (((unsigned CBNL)lp1 >= (unsigned CBNL)lp2) * 2 - 1);
NotSameSignWord:
  return ((lp1 >= lp2) * 2 - 1);
}

int    _CBNL_C  cBigNumberCompHigh (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));
  assert (n1 >= n2);
  assert (n2 > 0);

  register CBNL lp1 = p1 [n1--];
  register CBNL lp2 = p2 [n2--];
  assert (!n1 || lp1 != (p1 [n1] >> (BITS-1)));
  assert (!n2 || lp2 != (p2 [n2] >> (BITS-1)));

  if (lp1 != lp2) return ((lp1 > lp2) * 2 - 1);

  if (n2)
  do
  {
    lp1 = p1 [n1--]; lp2 = p2 [n2--];
    if (lp1 != lp2)
      return (((unsigned CBNL)lp1 > (unsigned CBNL)lp2) * 2 - 1);
  }
  while (n2);

  if (n1)
  do
  {
    if (p1 [n1--] != 0) return 1;
  }
  while (n1);

  return (0);
}

inline  CBNL    cBigNumberNot0  (
                const CBPTR(CBNL) p1_debug
                )
{
  const CBNL* p1 = CBPTRBASE(p1_debug);
  size_t n1 = (size_t)(*p1);
  return (p1 [n1] | (*p1 >> 1));
}

#ifndef _CBIGNUM_OPTIMIZED_COPY

void   _CBNL_C  cBigNumberCopy (
                const   CBPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*p++ = *CBPTRBASE(p1++));
  if (n1)
  {
    do
      { *p++ = *p1++; }
    while (--n1 != 0);
  }
}

#endif

void    cBigNumberCompl (
                const   CBPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*p++ = *CBPTRBASE(p1++));
  if (n1)
  {
    do
      { *p++ = ~*p1++; }
    while (--n1 != 0);
  }
  else
  {
    p [-1] = 1; p [0] = ~(CBNL)0;
  }
}

void    cBigNumberCopyShl (
                const   CBPTR(CBNL) p1,
                        size_t      k1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  if (n1 <= 1 && p1 [n1] == 0) k1 = 0;
  p1 += n1; *p = (CBNL)(n1 + k1); p += (n1 + k1);
  if (n1)
  do
    { *p-- = *p1--; }
  while (--n1 != 0);
  if (k1)
  do
    { *p-- = 0; }
  while (--k1 != 0);
}

void    cBigNumberCopyShr (
                const   CBPTR(CBNL) p1,
                        size_t      k1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  if (n1 > k1)
  {
    *p++ = (CBNL)(n1 -= k1);
    p1 += (k1 + 1);
    do
      { *p++ = *p1++; }
    while (--n1 != 0);
  }
  else
  {
    *p = 1; p [1] = p1 [n1] >> (BITS-1);
  }
}

void    cBigNumberCutOut (
                const   CBPTR(CBNL) p1,
                        size_t      k1,
                        size_t      n,
                        EXPTR(CBNL) p
        )
{
  assert ((size_t)(*CBPTRBASE(p1)) >= k1 + n);
  *p++ = (CBNL)(n + 1);
  if (n > 0)
  {
    p1 += (k1 + 1);
    do
      { *p++ = *p1++; }
    while (--n != 0);
  }
  *p = 0;
}

size_t _CBNL_C _cBigNumberNeg (
                const   CBPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  p++;
  size_t n = n1;
  if (n)
  do
  {
    CBNL lp;
    if ((*p = lp = -*p1) != 0)
    {
      if (--n != 0)
      {
        do
          { ++p1; ++p; *p = lp = ~*p1; }
        while (--n != 0);
        return (n1 - (lp == (p [-1] >> (BITS-1))));
      }
      if (lp != CBNL_MIN) return (n1);
      p [1] = 0; return (n1 + 1);
    }
  }
  while ((++p1, ++p, --n) != 0);

  return (n1);
}

size_t  _cBigNumberMInc (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1++);
  size_t n = n1;

  if (n)
  {
    do
    {
      CBNL lp;
      if ((*p1 = lp = *p1 + 1) != 0)
      {
        if (--n != 0)
        {
          if (--n != 0) return (n1);
          return (n1 - (p1 [1] == (lp >> (BITS-1))));
        }
        if (lp != CBNL_MIN) return (n1);
        p1 [1] = 0; return (n1 + 1);
      }
    }
    while ((++p1, --n) != 0);
  }
  else { *p1 = 1; n1 = 1; }

  return (n1);
}

size_t  _cBigNumberMDec (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1++);
  size_t n = n1;

  if (n)
  {
    do
    {
      CBNL lp;
      if ((*p1 = lp = *p1 - 1) != ~(CBNL)0)
      {
        if (--n != 0)
        {
          if (--n != 0) return (n1);
          return (n1 - (p1 [1] == (lp >> (BITS-1))));
        }
        if (lp != CBNL_MAX) return (n1);
        p1 [1] = ~(CBNL)0; return (n1 + 1);
      }
    }
    while ((++p1, --n) != 0);
  }
  else { *p1 = -1; n1 = 1; }

  return (n1);
}

size_t  _cBigNumberMMul2 (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1++);
  size_t n = n1;
  CBNL num = 0;
  if (n)
  do
  {
    CBNL pass = (unsigned CBNL)num >> (BITS-1);
    num = *p1;
    *p1++ = (num << 1) | pass;
  }
  while (--n != 0);

  num >>= (BITS-2);
  n1 += (num != (*p1 = num >> 1));

  return (n1);
}

size_t  _cBigNumberMul2 (
                const   CBPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  p++;
  CBNL num = 0;
  size_t n = n1;

  if (n)
  do
  {
    CBNL pass = (unsigned CBNL)num >> (BITS-1);
    num = *p1++;
    *p++ = (num << 1) | pass;
  }
  while (--n != 0);

  num >>= (BITS-2);
  n1 += (num != (*p = num >> 1));

  return (n1);
}

size_t  _cBigNumberMDiv2 (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1);
  size_t n = n1;
  p1 += n;

  if (n)
  {
    if (--n != 0)
    {
      CBNL num = *p1;
      n1 -= (num == (*p1-- = (num >> 1)));
      do
      {
        CBNL pass = num << (BITS-1);
        num = *p1;
        *p1-- = ((unsigned CBNL)num >> 1) | pass;
      }
      while (--n != 0);
    }
    else
    {
      (*p1) >>= 1;
    }
  }
  return (n1);
}

size_t  _cBigNumberDiv2 (
                const   CBPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n = n1;
  p1 += n;
  p  += n;

  if (n)
  {
    if (--n != 0)
    {
      CBNL num = *p1--;
      n1 -= (num == (*p-- = (num >> 1)));
      do
      {
        CBNL pass = num << (BITS-1);
        num = *p1--;
        *p-- = ((unsigned CBNL)num >> 1) | pass;
      }
      while (--n != 0);
    }
    else
    {
      *p = (*p1) >> 1;
    }
  }
  return (n1);
}

void    cBigNumberTab (
                        EXPTR(CBNL) p
        )
{
  size_t n = cBigNumberFit (p);
  int ns = BITS - 1;
  p [n + 1] = p [n] >> (BITS-1);
  n += 2;
  EXPTR(CBNL) pp = p;
  do
  {
    pp += n;
    cBigNumberMul2 (p, pp);
    p = pp;
  }
  while (--ns > 0);
}

size_t  _cBigNumberXor  (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  size_t n2 = (size_t)(*CBPTRBASE(p2++));
  p++;
  assert (n1 >= n2);
  if (n1 == 0) return 0;

  {
    register CBNL lp2 = 0;

    {
      size_t n;
      if ((n = n2) != 0)
      {
        do { *p++ = *p1++ ^ (lp2 = *p2++); }
        while (--n != 0);
      }
    }

    {
      lp2 >>= (BITS-1);
      size_t n;
      if ((n = n1 - n2) != 0)
      {
        do { *p++ = *p1++ ^ lp2; }
        while (--n != 0);
      }
    }
    --p;
  }

  {
    register CBNL pass;
    register CBNL lp;
    register size_t n = n1;
    if (n)
    {
      lp = *p;
      do continue;
      while (--n != 0 &&
        (pass = lp, lp = p [-1], --p, pass == (lp >> (BITS-1))));
    }
    else { p [1] = 0; }

    return (n + 1);
  }
}

size_t  _cBigNumberAnd  (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  size_t n2 = (size_t)(*CBPTRBASE(p2++));
  p++;
  assert (n1 >= n2);
  if (n1 == 0) return 0;

  {
    register CBNL lp2 = 0;

    {
      size_t n;
      if ((n = n2) != 0)
      {
        do { *p++ = *p1++ & (lp2 = *p2++); }
        while (--n != 0);
      }
    }

    {
      lp2 >>= (BITS-1);
      size_t n;
      if ((n = n1 - n2) != 0)
      {
        do { *p++ = *p1++ & lp2; }
        while (--n != 0);
      }
    }
    --p;
  }

  {
    register CBNL pass;
    register CBNL lp;
    register size_t n = n1;
    if (n)
    {
      lp = *p;
      do continue;
      while (--n != 0 &&
        (pass = lp, lp = p [-1], --p, pass == (lp >> (BITS-1))));
    }
    else { p [1] = 0; }

    return (n + 1);
  }
}

size_t  _cBigNumberOr  (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  size_t n2 = (size_t)(*CBPTRBASE(p2++));
  p++;
  assert (n1 >= n2);
  if (n1 == 0) return 0;

  {
    register CBNL lp2 = 0;

    {
      size_t n;
      if ((n = n2) != 0)
      {
        do { *p++ = *p1++ | (lp2 = *p2++); }
        while (--n != 0);
      }
    }

    {
      lp2 >>= (BITS-1);
      size_t n;
      if ((n = n1 - n2) != 0)
      {
        do { *p++ = *p1++ | lp2; }
        while (--n != 0);
      }
    }
    --p;
  }

  {
    register CBNL pass;
    register CBNL lp;
    register size_t n = n1;
    if (n)
    {
      lp = *p;
      do continue;
      while (--n != 0 &&
        (pass = lp, lp = p [-1], --p, pass == (lp >> (BITS-1))));
    }
    else { p [1] = 0; }

    return (n + 1);
  }
}

size_t  _cBigNumberAdd  (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  size_t n2 = (size_t)(*CBPTRBASE(p2++));
  p++;
  assert (n1 >= n2);
  if (n1 == 0) return 0;

  register unsigned CBNL pass = 0;
  {
    register CBNL lp1 = 0;
    register CBNL lp2 = 0;

    {
      size_t n;
      if ((n = n2) != 0)
      {
        do
        {
          lp1 = *p1++; lp2 = *p2++;
          pass = (~(*p++ = (pass >> (BITS-1)) + lp1 + lp2) & (lp1 ^ lp2))
                                                           | (lp1 & lp2);
        }
        while (--n != 0);
      }
    }

    {
      size_t n;
      if ((n = n1 - n2) != 0)
      {
        register const CBNL *pp = p1;
        if ((lp2 & CBNL_MIN) == 0)
        {
          do
          {
            lp1 = *pp++;
            pass = ~(*p++ = (pass >> (BITS-1)) + lp1) & lp1;
          }
          while (--n != 0);
        }
        else
        {
          do
          {
            lp1 = *pp++;
            pass = ~(*p++ = (pass >> (BITS-1)) + lp1 - 1) | lp1;
          }
          while (--n != 0);
        }
      }
    }
    --p;

    pass = (pass >> (BITS-1)) + ((lp1) >> (BITS-1)) + ((lp2) >> (BITS-1));
  }

  {
    register CBNL lp = *p;
    register size_t n = n1;
    if (n && (CBNL)pass == (lp >> (BITS-1)))
    {
      do continue;
      while (--n != 0 &&
        (pass = lp, lp = p [-1], --p, (CBNL)pass == (lp >> (BITS-1))));
    }
    else { p [1] = pass; }

    return (n + 1);
  }
}

size_t  _cBigNumberSub  (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  size_t n2 = (size_t)(*CBPTRBASE(p2++));
  p++;
  assert (n1 >= n2);
  if (n1 == 0) return 0;

  unsigned CBNL pass = ~(unsigned CBNL)0;
  {
    register CBNL lp1 = 0;
    register CBNL lp2 = ~(CBNL)0;

    {
      size_t n;
      if ((n = n2) != 0)
      {
        do
        {
          lp1 = *p1++; lp2 = ~(*p2++);
          pass = (~(*p++ = (pass >> (BITS-1)) + lp1 + lp2) & (lp1 ^ lp2))
                                                           | (lp1 & lp2);
        }
        while (--n != 0);
      }
    }

    {
      size_t n;
      if ((n = n1 - n2) != 0)
      {
        register const CBNL *pp = p1;
        if ((lp2 & CBNL_MIN) == 0)
        {
          do
          {
            lp1 = *pp++;
            pass = ~(*p++ = (pass >> (BITS-1)) + lp1) & lp1;
          }
          while (--n != 0);
        }
        else
        {
          do
          {
            lp1 = *pp++;
            pass = ~(*p++ = (pass >> (BITS-1)) + lp1 - 1) | lp1;
          }
          while (--n != 0);
        }
      }
    }
    --p;

    pass = (pass >> (BITS-1)) + ((lp1) >> (BITS-1)) + ((lp2) >> (BITS-1));
  }

  {
    register CBNL lp = *p;
    register size_t n = n1;
    if (n && (CBNL)pass == (lp >> (BITS-1)))
    {
      do continue;
      while (--n != 0 &&
        (pass = lp, lp = p [-1], --p, (CBNL)pass == (lp >> (BITS-1))));
    }
    else { p [1] = pass; }

    return (n + 1);
  }
}

size_t  _cBigNumberSubS (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1++));
  size_t n2 = (size_t)(*CBPTRBASE(p2++));
  p++;
  assert (n1 >= n2);
  if (n1 == 0) return 0;

  unsigned CBNL pass = ~(unsigned CBNL)0;
  {
    register CBNL lp1 = ~(CBNL)0;
    register CBNL lp2 = 0;

    {
      size_t n;
      if ((n = n2) != 0)
      {
        do
        {
          lp1 = ~(*p1++); lp2 = *p2++;
          pass = (~(*p++ = (pass >> (BITS-1)) + lp1 + lp2) & (lp1 ^ lp2))
                                                           | (lp1 & lp2);
        }
        while (--n != 0);
      }
    }

    {
      size_t n;
      if ((n = n1 - n2) != 0)
      {
        register const CBNL *pp = p1;
        if ((lp2 & CBNL_MIN) == 0)
        {
          do
          {
            lp1 = ~(*pp++);
            pass = ~(*p++ = (pass >> (BITS-1)) + lp1) & lp1;
          }
          while (--n != 0);
        }
        else
        {
          do
          {
            lp1 = ~(*pp++);
            pass = ~(*p++ = (pass >> (BITS-1)) + lp1 - 1) | lp1;
          }
          while (--n != 0);
        }
      }
    }
    --p;

    pass = (pass >> (BITS-1)) + ((lp1) >> (BITS-1)) + ((lp2) >> (BITS-1));
  }

  {
    register CBNL lp = *p;
    register size_t n = n1;
    if (n && (CBNL)pass == (lp >> (BITS-1)))
    {
      do continue;
      while (--n != 0 &&
        (pass = lp, lp = p [-1], --p, (CBNL)pass == (lp >> (BITS-1))));
    }
    else { p [1] = pass; }

    return (n + 1);
  }
}

#ifndef _CBIGNUM_MT
static  size_t  _cBigNumberSkip = 1;
#else
#define _cBigNumberSkip 1
#endif

size_t _CBNL_C  cBigNumberSkipLow0 (
                const   CBPTR(CBNL) p1
)
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n = 1;

  if (n1 != 0)
  do
  {
    if (p1 [n] != 0) break;
    n++;
  }
  while (--n1 != 0);

#ifndef _CBIGNUM_MT
  _cBigNumberSkip = n;
#endif

  return (n);
}

size_t _CBNL_C  cBigNumberCopySkipLow0 (
                const   CBPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif

  size_t n1 = (size_t)(*CBPTRBASE(p1));

  if (n1)
  {
    CBNL pass, lp = p1 [n1];
    do continue;
    while (--n1 != 0 &&
        (pass = lp, lp = p1 [n1], pass == (lp >> (BITS-1))));
    ++n1; ++p1;

    size_t k1 = 0;
    do
    {
      if (p1 [k1] != 0)
      {
        p1 += k1;
        *p++ = (CBNL)n1;
        do
          { *p++ = *p1++; }
        while (--n1 != 0);
        return (k1);
      }
      k1++;
    }
    while (--n1 != 0);
  }

  *p = 0;
  return (0);
}

#ifndef _CBIGNUM_ASM

void   _CBNL_C  cBigNumberMAddM (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2
        )
{
  const CBPTR(CBNL) pp = p1 + (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));
#ifndef _CBIGNUM_MT
  assert (_cBigNumberSkip > 0);
#endif
  assert ((size_t)(*p1) >= n2 + k2);
  assert (n2 >= _cBigNumberSkip);
  p1 += k2;

  register unsigned CBNL pass = 0;
  {
    const CBPTR(CBNL) pp = p1 + n2;
    p1 += _cBigNumberSkip;
    p2 += _cBigNumberSkip;
    register CBNL lp1;
    register CBNL lp2;

    for (;;)
    {
      lp1 = *p1;        lp2 = *p2;
      pass += lp1;      lp1 ^= lp2;
      pass += lp2;      lp1 = ~lp1;
      *p1 = pass;       pass |= lp1;
      lp1 &= lp2;       pass = ~pass;
      ++p2;             pass |= lp1;
      if (p1 >= pp)     break;
      ++p1;             pass >>= (BITS-1);
    }
    if ((CBNL)(lp2 ^= pass) >= 0) return;
  }

  {
    register CBNL lp1;
    if ((CBNL)pass < 0)
    {
      do
      {
        if (p1 >= pp) break;
        lp1 = *++p1;
      }
      while ((~(*p1 = lp1 + 1) & lp1) < 0);
    }
    else
    {
      do
      {
        if (p1 >= pp) break;
        lp1 = *++p1;
      }
      while ((~(*p1 = lp1 - 1) | lp1) >= 0);
    }
  }
}

void   _CBNL_C  cBigNumberMSubM (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2
        )
{
  const CBPTR(CBNL) pp = p1 + (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));
#ifndef _CBIGNUM_MT
  assert (_cBigNumberSkip > 0);
#endif
  assert ((size_t)(*p1) >= n2 + k2);
  assert (n2 >= _cBigNumberSkip);
  p1 += k2;

  register CBNL pass = 0;
  {
    const CBPTR(CBNL) pp = p1 + n2;
    p1 += _cBigNumberSkip;
    p2 += _cBigNumberSkip;
    register CBNL lp1;
    register CBNL lp2;

    for (;;)
    {
      lp1 = *p1;        lp2 = *p2;
      pass += lp1;      lp1 ^= lp2;
      pass -= lp2;      lp2 = ~lp2;
      *p1 = pass;       pass |= lp1;
      lp1 &= lp2;       pass = ~pass;
      ++p2;             pass |= lp1;
      if (p1 >= pp)     break;
      ++p1;             pass >>= (BITS-1);
                        pass = ~pass;
    }
    if ((CBNL)(lp2 ^= pass) >= 0) return;
  }

  {
    register CBNL lp1;
    if ((CBNL)pass < 0)
    {
      do
      {
        if (p1 >= pp) break;
        lp1 = *++p1;
      }
      while ((~(*p1 = lp1 + 1) & lp1) < 0);
    }
    else
    {
      do
      {
        if (p1 >= pp) break;
        lp1 = *++p1;
      }
      while ((~(*p1 = lp1 - 1) | lp1) >= 0);
    }
  }
}

size_t _CBNL_C _cBigNumberMSubD (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2
        )
{
  size_t n2 = (size_t)(*CBPTRBASE(p2));
#ifndef _CBIGNUM_MT
  assert (_cBigNumberSkip > 0);
#endif
  assert ((p1 [(size_t)(*p1)] ^ p2 [n2]) >= 0);
  assert ((size_t)(*p1) >= n2 + k2);
  assert (n2 >= _cBigNumberSkip);
  p1 += k2;

  register CBNL pass = 0;
  {
    const CBPTR(CBNL) pp = p1 + n2;
    p1 += _cBigNumberSkip;
    p2 += _cBigNumberSkip;
    register CBNL lp1;
    register CBNL lp2;

    for (;;)
    {
      lp1 = *p1;        lp2 = *p2;
      pass += lp1;      lp1 ^= lp2;
      pass -= lp2;      lp2 = ~lp2;
      *p1 = pass;       lp2 &= lp1;
      if (p1 >= pp)     break;
      lp2 = ~lp2;       pass |= lp1;
      ++p1;             pass &= lp2;
      ++p2;             pass >>= (BITS-1);
    }
  }

  {
    register size_t n1 = n2 + k2;

    for (;;)
    {
      register CBNL lp1;
      register CBNL lp2;
      if (--n1 == 0)    break;
      lp2 = p1 [-1];    --p1;
      lp1 = pass;       pass = lp2;
      lp2 >>= (BITS-1);
      if (lp1 != lp2)   break;
    }
    return (n1 + 1);
  }
}

size_t _CBNL_C _cBigNumberMMul2M (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1);
#ifndef _CBIGNUM_MT
  assert (_cBigNumberSkip > 0);
#endif
  assert (n1 >= _cBigNumberSkip);

  const CBPTR(CBNL) pp = p1 + n1;
  p1 += _cBigNumberSkip;
  CBNL num = 0;
  do
  {
    CBNL pass = (unsigned CBNL)num >> (BITS-1);
    num = *p1;
    *p1++ = (num << 1) | pass;
  }
  while (p1 <= pp);

  num >>= (BITS-2);
  n1 += (num != (*p1 = num >> 1));

  return (n1);
}

size_t _CBNL_C _cBigNumberMDiv2D (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1);
#ifndef _CBIGNUM_MT
  assert (_cBigNumberSkip > 0);
#endif
  assert (n1 >= _cBigNumberSkip);
  assert ((p1 [_cBigNumberSkip] & (CBNL)1)==0);

  const CBPTR(CBNL) pp = p1 + _cBigNumberSkip;
  p1 += n1;
  CBNL num = *p1;
  n1 -= (n1 > 1) & (num == (*p1-- = num >> 1));
  while (p1 >= pp)
  {
    CBNL pass = num << (BITS-1);
    num = *p1;
    *p1-- = ((unsigned CBNL)num >> 1) | pass;
  }

  return (n1);
}

#endif

void    cBigNumberMShrM (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1);
  assert (n1 > 1);
  *p1 = (CBNL)--n1;
  do
    { p1 [1] = p1 [2]; p1++; }
  while ((CBNL)--n1 > 0);
}

void    cBigNumberMShlD (
                        EXPTR(CBNL) p1
        )
{
  size_t n1 = (size_t)(*p1);
  assert (n1 > 0);
  *p1 = (CBNL)(n1 + 1);
  do
    { p1 [n1 + 1] = p1 [n1]; }
  while ((CBNL)--n1 > 0);
  p1 [1] = 0;
}

void    cBigNumberMAddShl (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2
        )
{
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigNumberFitTo (p1, n2 + k2 + 1);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
  if (n2 > 0) cBigNumberMAddM (p1, p2, k2);

  cBigNumberFit (p1);
}

void    cBigNumberMSubShl (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2
        )
{
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigNumberFitTo (p1, n2 + k2 + 1);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
  if (n2 > 0) cBigNumberMSubM (p1, p2, k2);

  cBigNumberFit (p1);
}

#ifndef _CBIGNUM_HARDWARE_MUL

void    cBigNumberMAddMulShl (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  assert (p2 != p1);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  if (n1 >= cBigNumberSkipLow0 (p1) && n2 != 0)
  {
    cBigNumberFitTo (p, n1 + n2 + k + 1);
    --n2;
    size_t k2 = 0;
    for (; k2 != n2; k2++)
    {
      CBNL num = p2 [k2 + 1];
      if (num == 0) continue;
      CBNL mask = 1;
      for (;; cBigNumberMMul2M (p1))
      {
        if (num & mask) cBigNumberMAddM (p, p1, k + k2);
        if ((mask <<= 1) == 0) break;
      }
      cBigNumberMMul2M (p1);
      cBigNumberMShrM (p1);
    }
    {
      CBNL num = p2 [k2 + 1];
      CBNL mask = (num != 0);
      if (num > 0)
      do
        mask <<= 1;
      while ((num <<= 1) > 0);
      for (;; cBigNumberMMul2M (p1))
      {
        if (num & mask) cBigNumberMAddM (p, p1, k + k2);
        if ((mask <<= 1) == 0) break;
      }
    }
  }

  cBigNumberFit (p);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
}

void    cBigNumberMSubMulShl (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  assert (p2 != p1);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  if (n1 >= cBigNumberSkipLow0 (p1) && n2 != 0)
  {
    cBigNumberFitTo (p, n1 + n2 + k + 1);
    --n2;
    size_t k2 = 0;
    for (; k2 != n2; k2++)
    {
      CBNL num = p2 [k2 + 1];
      if (num == 0) continue;
      CBNL mask = 1;
      for (;; cBigNumberMMul2M (p1))
      {
        if (num & mask) cBigNumberMSubM (p, p1, k + k2);
        if ((mask <<= 1) == 0) break;
      }
      cBigNumberMMul2M (p1);
      cBigNumberMShrM (p1);
    }
    {
      CBNL num = p2 [k2 + 1];
      CBNL mask = (num != 0);
      if (num > 0)
      do
        mask <<= 1;
      while ((num <<= 1) > 0);
      for (;; cBigNumberMMul2M (p1))
      {
        if (num & mask) cBigNumberMSubM (p, p1, k + k2);
        if ((mask <<= 1) == 0) break;
      }
    }
  }

  cBigNumberFit (p);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
}

#endif

void    cBigNumberMAddMulShlTab (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  if (n1 >= cBigNumberSkipLow0 (p1) && n2 != 0)
  {
    cBigNumberFitTo (p, n1 + n2 + k + 1);
    n1 += 2;
#ifndef _CBIGNUM_TERNARY_MUL
    for (size_t k2 = 0; k2 != n2; k2++)
    {
      CBNL num = p2 [k2 + 1];
      CBNL mask = 1;
      const CBPTR(CBNL) pp1 = p1;
      for (;; pp1 += n1)
      {
        if (num & mask) cBigNumberMAddM (p, pp1, k + k2);
        if ((mask <<= 1) == 0) break;
      }
    }
#else
    CBNL num2 = p2 [n2], num1 = num2;
    num2 <<= 1;
    CBNL mode = 0;
    if ((num1 & num2) < 0)
    {
      cBigNumberMAddM (p, p1, k + n2);
      num1 = ~num1; num2 = ~num2;
      mode = ~(CBNL)0;
    }
    p1 += n1 * BITS; p1 -= n1;
    for (size_t k2 = n2; k2-- != 0;)
    {
      const CBPTR(CBNL) pp1 = p1;
      for (size_t kb = BITS;; pp1 -= n1)
      {
        CBNL num = num1;
        num1 = num2;
        if (kb != 2) num2 <<= 1;
        else num2 = (k2? p2 [k2]: 0L) ^ mode;
        if ((num | (num1 & num2) ) < 0)
        {
           if (mode == 0) cBigNumberMAddM (p, pp1, k + k2);
           else           cBigNumberMSubM (p, pp1, k + k2);
           if (num >= 0) { num1 = ~num1; num2 = ~num2; mode = ~mode; }
        }
        if (--kb == 0) break;
      }
    }
#endif
  }

  cBigNumberFit (p);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
}

void    cBigNumberMSubMulShlTab (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  if (n1 >= cBigNumberSkipLow0 (p1) && n2 != 0)
  {
    cBigNumberFitTo (p, n1 + n2 + k + 1);
    n1 += 2;
#ifndef _CBIGNUM_TERNARY_MUL
    for (size_t k2 = 0; k2 != n2; k2++)
    {
      CBNL num = p2 [k2 + 1];
      CBNL mask = 1;
      const CBPTR(CBNL) pp1 = p1;
      for (;; pp1 += n1)
      {
        if (num & mask) cBigNumberMSubM (p, pp1, k + k2);
        if ((mask <<= 1) == 0) break;
      }
    }
#else
    CBNL num2 = p2 [n2], num1 = num2;
    num2 <<= 1;
    CBNL mode = 0;
    if ((num1 & num2) < 0)
    {
      cBigNumberMSubM (p, p1, k + n2);
      num1 = ~num1; num2 = ~num2;
      mode = ~(CBNL)0;
    }
    p1 += n1 * BITS; p1 -= n1;
    for (size_t k2 = n2; k2-- != 0;)
    {
      const CBPTR(CBNL) pp1 = p1;
      for (size_t kb = BITS;; pp1 -= n1)
      {
        CBNL num = num1;
        num1 = num2;
        if (kb != 2) num2 <<= 1;
        else num2 = (k2? p2 [k2]: 0L) ^ mode;
        if ((num | (num1 & num2) ) < 0)
        {
           if (mode == 0) cBigNumberMSubM (p, pp1, k + k2);
           else           cBigNumberMAddM (p, pp1, k + k2);
           if (num >= 0) { num1 = ~num1; num2 = ~num2; mode = ~mode; }
        }
        if (--kb == 0) break;
      }
    }
#endif
  }

  cBigNumberFit (p);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
}

#ifndef _CBIGNUM_HARDWARE_MUL

void    cBigNumberMAddMulShlKar (
                        EXPTR(CBNL) p1,
                        EXPTR(CBNL) p2,
                        size_t      k,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*p2);

#ifdef  _CBIGNUM_KARATSUBA_MUL
  if (n1 > _CBNL_KARATSUBA_MIN && n2 > _CBNL_KARATSUBA_MIN)
  {

    while (n1 > n2 * 2 - n2 / 4)
    {
      CBNL lp = p1 [n1 -= n2];
      p1 [n1] = (CBNL) n2;
      cBigNumberMAddMulShlKar (p1 + n1, p2, k + n1, p);
      p1 [n1] = lp;
      p1 [n1 + 1] = 0; n1 += (lp < 0);
      p1 [0] = (CBNL) n1;
    }

    size_t n = (n1 >= n2? n1: n2 - 1) / 2;

    cBigTemp c12; c12.checkexpand (n * 2 + 5);
    EXPTR(CBNL) p12 = EXPTRTYPE(c12);

    cBigTemp c1; c1.checkexpand (n + 2);
    cBigTemp c2; c2.checkexpand (n + 4);
    cBigNumberCopyShr (p2, n, EXPTRTYPE(c2));

    if (n1 > n)
    {
      cBigNumberCopyShr (p1, n, EXPTRTYPE(c1));
      CBNL lp = p1 [n];
      p1 [n] = (CBNL)(n1 - n);
      p12 [0] = 0;
      cBigNumberMAddMulShlKar (p1 + n, EXPTRTYPE(c2), 0, p12);
      cBigNumberMAddShl (p, p12, k + n * 2);
      cBigNumberMSubShl (p, p12, k + n);
      p1 [n] = lp;
      p1 [n + 1] = 0;
      p1 [0] = (CBNL)(n + (lp < 0));
      cBigNumberMAddShl (EXPTRTYPE(c1), p1, 0);
    }
    else
      cBigNumberCopy (p1, EXPTRTYPE(c1));

    assert (n2 > n);
    {
      size_t nc2 = c2.length() + 1;
      c2.item (0) = (CBNL) nc2;
      ((EXPTR(CBNL)) EXPTRTYPE(c2)) [nc2] = 0;

      CBNL lp = p2 [n + 1];
      p2 [n + 1] = 0;
      p2 [0] = (CBNL)(n + 1);
      cBigNumberMAddShl (EXPTRTYPE(c2), p2, 0);
      p2 [n + 1] = lp;

      p2 [0] = (CBNL) n;
      p12 [0] = 0;
      cBigNumberMAddMulShlKar (p1, p2, 0, p12);
      p2 [0] = (CBNL) n2;

      c2.item (0) = (CBNL)(c2.length() - (c2.hiword() == 0));
      cBigNumberCopy (CBPTRTYPE(c1), p1);
      cBigNumberMAddMulShlKar (p1, EXPTRTYPE(c2), k + n, p);

      cBigNumberMSubShl (p, p12, k + n);
      cBigNumberMAddShl (p, p12, k);
    }

    return;
  }
#endif

#ifdef  _CBIGNUM_BLOCK_MUL
  if (n1 > _CBNL_TAB_HIGH)
  {
    --n1;
    do
    {
      n1 -= _CBNL_TAB_OPT;
      CBNL lp = p1 [n1];
      p1 [n1] = _CBNL_TAB_OPT + 1;
#ifdef  _CBIGNUM_SHIFTTAB_MUL
      if (n2 >= _CBNL_TAB_MIN) {
        cBigNumberTab (p1 + n1);
        cBigNumberMAddMulShlTab (p1 + n1, p2, k + n1, p);
      } else
#endif
        cBigNumberMAddMulShl (p1 + n1, p2, k + n1, p);
      p1 [n1 + 1] = 0;
      p1 [n1] = lp;
    }
    while (n1 >= _CBNL_TAB_HIGH);
    p1 [0] = (CBNL)(n1 + 1);
  }
#endif

#ifdef  _CBIGNUM_SHIFTTAB_MUL
#ifdef  _CBIGNUM_BLOCK_MUL
  if (n2 >= _CBNL_TAB_MIN) {
#else
  if (n2 >= _CBNL_TAB_MIN && n1 <= _CBNL_TAB_HIGH) {
#endif
    cBigNumberTab (p1);
    cBigNumberMAddMulShlTab (p1, p2, k, p);
  } else
#endif
    cBigNumberMAddMulShl (p1, p2, k, p);
}

void    cBigNumberMSubMulShlKar (
                        EXPTR(CBNL) p1,
                        EXPTR(CBNL) p2,
                        size_t      k,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*p2);

#ifdef  _CBIGNUM_KARATSUBA_MUL
  if (n1 > _CBNL_KARATSUBA_MIN && n2 > _CBNL_KARATSUBA_MIN)
  {

    while (n1 > n2 * 2 - n2 / 4)
    {
      CBNL lp = p1 [n1 -= n2];
      p1 [n1] = (CBNL) n2;
      cBigNumberMSubMulShlKar (p1 + n1, p2, k + n1, p);
      p1 [n1] = lp;
      p1 [n1 + 1] = 0; n1 += (lp < 0);
      p1 [0] = (CBNL) n1;
    }

    size_t n = (n1 >= n2? n1: n2 - 1) / 2;

    cBigTemp c12; c12.checkexpand (n * 2 + 5);
    EXPTR(CBNL) p12 = EXPTRTYPE(c12);

    cBigTemp c1; c1.checkexpand (n + 2);
    cBigTemp c2; c2.checkexpand (n + 4);
    cBigNumberCopyShr (p2, n, EXPTRTYPE(c2));

    if (n1 > n)
    {
      cBigNumberCopyShr (p1, n, EXPTRTYPE(c1));
      CBNL lp = p1 [n];
      p1 [n] = (CBNL)(n1 - n);
      p12 [0] = 0;
      cBigNumberMAddMulShlKar (p1 + n, EXPTRTYPE(c2), 0, p12);
      cBigNumberMSubShl (p, p12, k + n * 2);
      cBigNumberMAddShl (p, p12, k + n);
      p1 [n] = lp;
      p1 [n + 1] = 0;
      p1 [0] = (CBNL)(n + (lp < 0));
      cBigNumberMAddShl (EXPTRTYPE(c1), p1, 0);
    }
    else
      cBigNumberCopy (p1, EXPTRTYPE(c1));

    assert (n2 > n);
    {
      size_t nc2 = c2.length() + 1;
      c2.item (0) = (CBNL) nc2;
      ((EXPTR(CBNL)) EXPTRTYPE(c2)) [nc2] = 0;

      CBNL lp = p2 [n + 1];
      p2 [n + 1] = 0;
      p2 [0] = (CBNL)(n + 1);
      cBigNumberMAddShl (EXPTRTYPE(c2), p2, 0);
      p2 [n + 1] = lp;

      p2 [0] = (CBNL) n;
      p12 [0] = 0;
      cBigNumberMAddMulShlKar (p1, p2, 0, p12);
      p2 [0] = (CBNL) n2;

      c2.item (0) = (CBNL)(c2.length() - (c2.hiword() == 0));
      cBigNumberCopy (CBPTRTYPE(c1), p1);
      cBigNumberMSubMulShlKar (p1, EXPTRTYPE(c2), k + n, p);

      cBigNumberMAddShl (p, p12, k + n);
      cBigNumberMSubShl (p, p12, k);
    }

    return;
  }
#endif

#ifdef  _CBIGNUM_BLOCK_MUL
  if (n1 > _CBNL_TAB_HIGH)
  {
    --n1;
    do
    {
      n1 -= _CBNL_TAB_OPT;
      CBNL lp = p1 [n1];
      p1 [n1] = _CBNL_TAB_OPT + 1;
#ifdef  _CBIGNUM_SHIFTTAB_MUL
      if (n2 >= _CBNL_TAB_MIN) {
        cBigNumberTab (p1 + n1);
        cBigNumberMSubMulShlTab (p1 + n1, p2, k + n1, p);
      } else
#endif
        cBigNumberMSubMulShl (p1 + n1, p2, k + n1, p);
      p1 [n1 + 1] = 0;
      p1 [n1] = lp;
    }
    while (n1 >= _CBNL_TAB_HIGH);
    p1 [0] = (CBNL)(n1 + 1);
  }
#endif

#ifdef  _CBIGNUM_SHIFTTAB_MUL
#ifdef  _CBIGNUM_BLOCK_MUL
  if (n2 >= _CBNL_TAB_MIN) {
#else
  if (n2 >= _CBNL_TAB_MIN && n1 <= _CBNL_TAB_HIGH) {
#endif
    cBigNumberTab (p1);
    cBigNumberMSubMulShlTab (p1, p2, k, p);
  } else
#endif
    cBigNumberMSubMulShl (p1, p2, k, p);
}

void    cBigNumberMAddMul (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigTemp cBigBuf1;
  cBigTemp cBigBuf2;
  cBigBuf1.checkexpand ((n2 < _CBNL_TAB_MIN)? (n1 + 2):
                        (n1 < _CBNL_TAB_HIGH)? ((n1 + 3) * BITS + 1):
                          (n1 + 2 + (_CBNL_TAB_HIGH + 2) * BITS + 1));
  cBigBuf2.checkexpand (n2 + 1);
  EXPTR(CBNL) pp1 = EXPTRTYPE(cBigBuf1);
  EXPTR(CBNL) pp2 = EXPTRTYPE(cBigBuf2);

  size_t k = cBigNumberCopySkipLow0 (p1, pp1);

  if (CBPTRBASE(p2)[n2] >= 0)
  {
    cBigNumberCopy (p2, pp2);
    cBigNumberMAddMulShlKar (pp1, pp2, k, p);
  }
  else
  {
    cBigNumberNeg (p2, pp2);
    *pp2 = (CBNL)n2;
    cBigNumberMSubMulShlKar (pp1, pp2, k, p);
  }
}

void    cBigNumberMSubMul (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigTemp cBigBuf1;
  cBigTemp cBigBuf2;
  cBigBuf1.checkexpand ((n2 < _CBNL_TAB_MIN)? (n1 + 2):
                        (n1 < _CBNL_TAB_HIGH)? ((n1 + 3) * BITS + 1):
                          (n1 + 2 + (_CBNL_TAB_HIGH + 2) * BITS + 1));
  cBigBuf2.checkexpand (n2 + 1);
  EXPTR(CBNL) pp1 = EXPTRTYPE(cBigBuf1);
  EXPTR(CBNL) pp2 = EXPTRTYPE(cBigBuf2);

  size_t k = cBigNumberCopySkipLow0 (p1, pp1);

  if (CBPTRBASE(p2)[n2] >= 0)
  {
    cBigNumberCopy (p2, pp2);
    cBigNumberMSubMulShlKar (pp1, pp2, k, p);
  }
  else
  {
    cBigNumberNeg (p2, pp2);
    *pp2 = (CBNL)n2;
    cBigNumberMAddMulShlKar (pp1, pp2, k, p);
  }
}

void    cBigNumberMul (
                const   CBPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  size_t n1 = (size_t)(*CBPTRBASE(p1));
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigTemp cBigBuf1;
  cBigTemp cBigBuf2;
  cBigBuf1.checkexpand ((n2 < _CBNL_TAB_MIN)? (n1 + 2):
                        (n1 < _CBNL_TAB_HIGH)? ((n1 + 3) * BITS + 1):
                          (n1 + 2 + (_CBNL_TAB_HIGH + 2) * BITS + 1));
  cBigBuf2.checkexpand (n2 + 1);
  EXPTR(CBNL) pp1 = EXPTRTYPE(cBigBuf1);
  EXPTR(CBNL) pp2 = EXPTRTYPE(cBigBuf2);

  size_t k = cBigNumberCopySkipLow0 (p1, pp1);

  if (CBPTRBASE(p2)[n2] >= 0)
  {
    cBigNumberCopy (p2, pp2);
    p [0] = 0;
    cBigNumberMAddMulShlKar (pp1, pp2, k, p);
  }
  else
  {
    cBigNumberNeg (p2, pp2);
    *pp2 = (CBNL)n2;
    p [0] = 0;
    cBigNumberMSubMulShlKar (pp1, pp2, k, p);
  }
}

void   _CBNL_C  cLongMul (
                        CBNL    l1,
                        CBNL    l2,
                        CBNL    p [2]
        )
{
  CBNL c1 [5]; c1 [0] = 1; c1 [1] = l1;
  CBNL c2 [3]; c2 [0] = 1; c2 [1] = l2;
  CBNL c  [5]; c [0] = c [1] = c [2] = 0;
  EXPTR(CBNL) pp1 = EXPTRTO(CBNL,c1,sizeof(c1)/sizeof(*c1)-1);
  EXPTR(CBNL) pp2 = EXPTRTO(CBNL,c2,sizeof(c2)/sizeof(*c2)-1);
  EXPTR(CBNL) pp  = EXPTRTO(CBNL,c, sizeof(c) /sizeof(*c) -1);

  if (l2 >= 0)
    cBigNumberMAddMulShl (pp1, pp2, 0, pp);
  else {
    c2 [1] = -l2;
    cBigNumberMSubMulShl (pp1, pp2, 0, pp);
  }

  p [0] = pp [1];
  p [1] = pp [2];
}

void   _CBNL_C  cULongMul (
                unsigned CBNL   l1,
                unsigned CBNL   l2,
                unsigned CBNL   p [2]
        )
{
  CBNL c1 [5]; c1 [0] = 2; c1 [1] = l1; c1 [2] = 0;
  CBNL c2 [3]; c2 [0] = 1; c2 [1] = l2;
  CBNL c  [6]; c [0] = c [1] = c [2] = 0;
  EXPTR(CBNL) pp1 = EXPTRTO(CBNL,c1,sizeof(c1)/sizeof(*c1)-1);
  EXPTR(CBNL) pp2 = EXPTRTO(CBNL,c2,sizeof(c2)/sizeof(*c2)-1);
  EXPTR(CBNL) pp  = EXPTRTO(CBNL,c, sizeof(c) /sizeof(*c) -1);

  cBigNumberMAddMulShl (pp1, pp2, 0, pp);

  p [0] = pp [1];
  p [1] = pp [2];
}

#endif

size_t  cBigNumberMaskDiv0;
void    cBigNumberDiv0()
{
  cBigNumberMaskDiv0 /= cBigNumberMaskDiv0;
}

void    cBigNumberMModDivShlTab (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2,
                        EXPTR(CBNL) p
        )
{
  assert (cBigNumberIsFit (p1));
  assert (p1 != p);
  assert (p2 != p);
  assert (p2 != p1);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));
  size_t n = n2 + k2;
  if (n > n1) n = n1; n = n1 - n + 1;

  *p = (CBNL)(n + 1); p += n; p [1] = 0;

  if (n2 >= cBigNumberSkipLow0 (p2))
  {
    assert ((p1 [n1] ^ CBPTRBASE(p2)[n2]) >= 0);
    CBNL lt = (CBPTRBASE(p2)[n2] < 0) * 2 - 1;
    n2 += 2;
    do
    {
      --n;
      p2 += n2 * BITS;
      unsigned CBNL mask = (unsigned CBNL)
                        (((CBNL)1) << (BITS-1));
      *p = 0;
      do
      {
        p2 -= n2;
        CBNL diff = (CBNL)(n + k2) - *p1 + *p2;
        if (diff <= 0 && (diff < 0 ||
            cBigNumberCompHigh (p1, p2) != lt))
        {
          cBigNumberMSubD (p1, p2, n + k2);
          (*p) += mask;
        }
      }
      while ((mask >>= 1) != 0);
      --p;
    }
    while (n);
    assert ((p1 [(size_t)(*p1)] ^ lt) < 0 ||
            (p1 [(size_t)(*p1)] + *p1) == 1);
    assert (cBigNumberIsFit (p1));
  }
  else
  {
    cBigNumberDiv0();
    do { *p-- = 0; } while (--n);
  }

  cBigNumberFit (p);
  assert (p [(size_t)(*p)] >= 0);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
}

void    cBigNumberMModDiv (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigTemp cBigBuf2;
#ifdef  _CBIGNUM_SHIFTTAB_DIV
  CBNL nt = (CBNL)((n2 - n1 + _CBNL_TAB_MIN - 1) & (n2 - _CBNL_TAB_MAX));
  cBigBuf2.checkexpand ((size_t)((nt < 0)? ((n2 + 3) * BITS + 1): (n2 + 3)));
#else
  cBigBuf2.checkexpand ((size_t)(n2 + 3));
#endif
  EXPTR(CBNL) pp2 = EXPTRTYPE(cBigBuf2);

  size_t k2 = cBigNumberCopySkipLow0 (p2, pp2);

  CBNL signquot = p1 [n1] ^ CBPTRBASE(p2)[n2];
  if (signquot < 0) cBigNumberNeg (pp2, pp2);

#ifdef  _CBIGNUM_SHIFTTAB_DIV
  if (nt < 0)
  {
    cBigNumberTab (pp2);
    cBigNumberMModDivShlTab (p1, pp2, k2, p);
    if (signquot < 0) cBigNumberNeg (p, p);
    return;
  }
#endif

  n2 = (size_t)(*pp2);
  if (n2 == 0)
  {
    cBigNumberDiv0();
    p [0] = 1; p [1] = 0;
    return;
  }
  assert (cBigNumberIsFit (p1));
  assert (cBigNumberIsFit (pp2));

  size_t n = n2 + k2;

  if (n2 > 1 || n != n1)
  {
    if (n > n1)
    {
      p [0] = 1; p [1] = 0;
      return;
    }
    n = n1 - n + 1;

    *p = (CBNL)(n + 1); p += n; p [1] = 0;

    assert ((p1 [n1] ^ pp2 [n2]) >= 0);
    CBNL lt = (pp2 [n2] < 0) * 2 - 1;
    do
    {
      --n;
      *p = 0;
      cBigNumberMShlD (pp2);
      unsigned CBNL mask = ((unsigned CBNL)1)
                            << (BITS-1);
      do
      {
        CBNL diff = (CBNL)cBigNumberMDiv2D (pp2)
                  + (CBNL)(n + k2) - *p1;
        if (diff <= 0 && (diff < 0 ||
            cBigNumberCompHigh (p1, pp2) != lt))
        {
          cBigNumberMSubD (p1, pp2, n + k2);
          (*p) += mask;
        }
      }
      while ((mask >>= 1) != 0);
      --p;
    }
    while (n);
    assert ((p1 [(size_t)(*p1)] ^ lt) < 0 ||
            (p1 [(size_t)(*p1)] + *p1) == 1);
    assert (cBigNumberIsFit (p1));
    cBigNumberFit (p);
  }
  else
  {
    CBNL p1n = p1 [n1];
    CBNL p2n = pp2 [n2];
    assert ((p1n ^ p2n) >= 0);
    assert (p2n != 0);

#ifndef _CBIGNUM_HARDWARE_DIV
    CBNL pn = 0;
    int nb = cLongBits (p1n) - cLongBits (p2n);
    if (nb >= 0)
    {
      unsigned CBNL mask = ((unsigned CBNL)1)
                            << nb;
      p2n <<= nb;

      if (p2n > 0)
      {
        for (;; p2n >>= 1)
        {
#ifdef  _CBIGNUM_REDUCE_JUMPS
          CBNL m = ((p1n < p2n) - 1);
          p1n -= p2n & m;
          pn += mask & m;
#else
          if (p1n >= p2n)
          {
            p1n -= p2n;
            pn += mask;
          }
#endif
          if ((mask >>= 1) == 0) break;
        }
      }
      else
      {
        for (;; p2n >>= 1)
        {
#ifdef  _CBIGNUM_REDUCE_JUMPS
          CBNL m = ((p1n > p2n) - 1);
          p1n -= p2n & m;
          pn += mask & m;
#else
          if (p1n <= p2n)
          {
            p1n -= p2n;
            pn += mask;
          }
#endif
          if ((mask >>= 1) == 0) break;
        }
        if (p1n == 0)
        {
          for (size_t n = n1; --n > 0;)
          {
            if (p1 [n] != 0) { pn--; p1n += p2n; break; }
          }
        }
      }
    }
#else
    CBNL pn;
    if (p1n != CBNL_MIN || p2n != -1)
    {
      pn = p1n / p2n;
      p1n -= pn * p2n;
    }
    else
    {
      pn = p1n;
      p1n = 0;
    }
    if (p2n < 0 && p1n == 0)
    {
      for (size_t n = n1; --n > 0;)
      {
        if (p1 [n] != 0) { pn--; p1n += p2n; break; }
      }
    }
#endif

    p [0] = 1; p [1] = pn;
    if (pn < 0) { p [0] = 2; p [2] = 0; }
    p1 [n1] = p1n;
    cBigNumberFit (p1);
    assert ((p1 [(size_t)(*p1)] ^ pp2 [n2]) >= 0
         || (p1 [(size_t)(*p1)] + *p1) == 1);
    assert (cBigNumberIsFit (p));
  }

  assert (p [(size_t)(*p)] >= 0);
  if (signquot < 0) cBigNumberNeg (p, p);
}

void    cBigNumberMModShlTab (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        size_t      k2
        )
{
  assert (cBigNumberIsFit (p1));
  assert (p1 != p2);
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));
  size_t n = n2 + k2;
  if (n > n1) n = n1; n = n1 - n + 1;

  if (n2 >= cBigNumberSkipLow0 (p2))
  {
    assert ((p1 [n1] ^ CBPTRBASE(p2)[n2]) >= 0);
    CBNL lt = (CBPTRBASE(p2)[n2] < 0) * 2 - 1;
    n2 += 2;
    do
    {
      --n;
      p2 += n2 * BITS;
      unsigned CBNL mask = (unsigned CBNL)
                        (((CBNL)1) << (BITS-1));
      do
      {
        p2 -= n2;
        CBNL diff = (CBNL)(n + k2) - *p1 + *p2;
        if (diff <= 0 && (diff < 0 ||
            cBigNumberCompHigh (p1, p2) != lt))
        {
          cBigNumberMSubD (p1, p2, n + k2);
        }
      }
      while ((mask >>= 1) != 0);
    }
    while (n);
    assert ((p1 [(size_t)(*p1)] ^ lt) < 0 ||
            (p1 [(size_t)(*p1)] + *p1) == 1);
    assert (cBigNumberIsFit (p1));
  }
  else
  {
    cBigNumberDiv0();
  }

#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif
}

void    cBigNumberMMod (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2
        )
{
  size_t n1 = (size_t)(*p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  cBigTemp cBigBuf2;
#ifdef  _CBIGNUM_SHIFTTAB_DIV
  CBNL nt = (CBNL)((n2 - n1 + _CBNL_TAB_MIN - 1) & (n2 - _CBNL_TAB_MAX));
  cBigBuf2.checkexpand ((size_t)((nt < 0)? ((n2 + 3) * BITS + 1): (n2 + 3)));
#else
  cBigBuf2.checkexpand ((size_t)(n2 + 3));
#endif
  EXPTR(CBNL) pp2 = EXPTRTYPE(cBigBuf2);

  size_t k2 = cBigNumberCopySkipLow0 (p2, pp2);

  if ((p1 [n1] ^ CBPTRBASE(p2)[n2]) < 0) cBigNumberNeg (pp2, pp2);

#ifdef  _CBIGNUM_SHIFTTAB_DIV
  if (nt < 0)
  {
    cBigNumberTab (pp2);
    cBigNumberMModShlTab (p1, pp2, k2);
    return;
  }
#endif

  n2 = (size_t)(*pp2);
  if (n2 == 0)
  {
    cBigNumberDiv0();
    return;
  }
  assert (cBigNumberIsFit (p1));
  assert (cBigNumberIsFit (pp2));

  size_t n = n2 + k2;

  if (n2 > 1 || n != n1)
  {
    if (n > n1) return;
    n = n1 - n + 1;

    assert ((p1 [n1] ^ pp2 [n2]) >= 0);
    CBNL lt = (pp2 [n2] < 0) * 2 - 1;
    do
    {
      --n;
      cBigNumberMShlD (pp2);
      unsigned CBNL mask = ((unsigned CBNL)1)
                            << (BITS-1);
      do
      {
        CBNL diff = (CBNL)cBigNumberMDiv2D (pp2)
                  + (CBNL)(n + k2) - *p1;
        if (diff <= 0 && (diff < 0 ||
            cBigNumberCompHigh (p1, pp2) != lt))
        {
          cBigNumberMSubD (p1, pp2, n + k2);
        }
      }
      while ((mask >>= 1) != 0);
    }
    while (n);
    assert ((p1 [(size_t)(*p1)] ^ lt) < 0 ||
            (p1 [(size_t)(*p1)] + *p1) == 1);
    assert (cBigNumberIsFit (p1));
  }
  else
  {
    CBNL p1n = p1 [n1];
    CBNL p2n = pp2 [n2];
    assert ((p1n ^ p2n) >= 0);
    assert (p2n != 0);

#ifndef _CBIGNUM_HARDWARE_DIV
    int nb = cLongBits (p1n) - cLongBits (p2n);
    if (nb >= 0)
    {
      unsigned CBNL mask = ((unsigned CBNL)1)
                            << nb;
      p2n <<= nb;

      if (p2n > 0)
      {
        for (;; p2n >>= 1)
        {
#ifdef  _CBIGNUM_REDUCE_JUMPS
          CBNL m = ((p1n < p2n) - 1);
          p1n -= p2n & m;
#else
          if (p1n >= p2n)
          {
            p1n -= p2n;
          }
#endif
          if ((mask >>= 1) == 0) break;
        }
      }
      else
      {
        for (;; p2n >>= 1)
        {
#ifdef  _CBIGNUM_REDUCE_JUMPS
          CBNL m = ((p1n > p2n) - 1);
          p1n -= p2n & m;
#else
          if (p1n <= p2n)
          {
            p1n -= p2n;
          }
#endif
          if ((mask >>= 1) == 0) break;
        }
        if (p1n == 0)
        {
          for (size_t n = n1; --n > 0;)
          {
            if (p1 [n] != 0) { p1n += p2n; break; }
          }
        }
      }
    }
#else
    if (p1n != CBNL_MIN || p2n != -1)
    {
      p1n = p1n % p2n;
    }
    else
    {
      p1n = 0;
    }
    if (p2n < 0 && p1n == 0)
    {
      for (size_t n = n1; --n > 0;)
      {
        if (p1 [n] != 0) { p1n += p2n; break; }
      }
    }
#endif

    p1 [n1] = p1n;
    cBigNumberFit (p1);
    assert ((p1 [(size_t)(*p1)] ^ pp2 [n2]) >= 0
         || (p1 [(size_t)(*p1)] + *p1) == 1);
  }
}

void    cBigNumberPow (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  assert (p2 != p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  p [0] = 1; p [1] = 1;

  if (CBPTRBASE(p2)[n2] < 0)
  {
    cBigNumberFit (p1);
    cBigNumberMModDiv (p, p1, p1);
    if (p1 [1] + (p2 [1] & 1) < 0) p1 [1] = 1;
    p [1] = p1 [1];
  }
  else if (n2 != 0)
  {
    p2++;
    for (;;)
    {
      CBNL num = *p2++;
      CBNL mask = 1;
      if (--n2 == 0)
      {
        if (num == 0) break;
        do
          mask <<= 1;
        while ((num <<= 1) > 0);
      }
      for (;;)
      {
        if (num & mask)
          cBigNumberMul (p1, p, p);
        if ((mask <<= 1) == 0) break;
        cBigNumberMul (p1, p1, p1);
      }
      if (n2 == 0) break;
      cBigNumberMul (p1, p1, p1);
    }
  }
}

void    cBigNumberPowMod (
                        EXPTR(CBNL) p1,
                const   CBPTR(CBNL) p2,
                const   CBPTR(CBNL) mod,
                        EXPTR(CBNL) p
        )
{
  assert (p1 != p);
  assert (p2 != p);
  assert (p2 != p1);
  size_t n2 = (size_t)(*CBPTRBASE(p2));

  p [0] = 1; p [1] = 1;

  cBigNumberFit (p1);
  if (CBPTRBASE(p2)[n2] < 0)
  {
    cBigNumberMModDiv (p, p1, p1);
    if (p1 [1] + (p2 [1] & 1) < 0) p1 [1] = 1;
    p [1] = p1 [1]; cBigNumberMMod (p, mod);
    return;
  }

  cBigTemp cBigBuf3;
  cBigBuf3.checkexpand (exmuladd (BITS, (size_t)(*CBPTRBASE(mod)),
                                  BITS * 3 + 1));
  EXPTR(CBNL) pmod = EXPTRTYPE(cBigBuf3);

  size_t kmod = cBigNumberCopySkipLow0 (mod, pmod);

  {
    size_t nmod = (size_t)(*pmod);
    if (nmod == 0) n2 = 0;
    if (pmod [nmod] < 0) cBigNumberNeg (pmod, pmod);
  }
  cBigNumberTab (pmod);

  cBigNumberMModShlTab (p, pmod, kmod);

  if (n2 != 0 && p [1] != 0)
  {
    p2++;

    CBNL sign = p1 [(size_t)(*p1)];
    if (sign < 0)
    {
      cBigNumberNeg (p1, p1);
      sign = (*p2 << (BITS-1));
    }
    cBigNumberMModShlTab (p1, pmod, kmod);

    for (;;)
    {
      CBNL num = *p2++;
      CBNL mask = 1;
      if (--n2 == 0)
      {
        if (num == 0) break;
        do
          mask <<= 1;
        while ((num <<= 1) > 0);
      }
      for (;;)
      {
        if (num & mask)
        {
          if (*p1 > *p) cBigNumberMul (p1, p, p);
          else          cBigNumberMul (p, p1, p);
          cBigNumberMModShlTab (p, pmod, kmod);
        }
        if ((mask <<= 1) == 0) break;
        cBigNumberMul (p1, p1, p1);
        cBigNumberMModShlTab (p1, pmod, kmod);
      }
      if (n2 == 0) break;
      cBigNumberMul (p1, p1, p1);
      cBigNumberMModShlTab (p1, pmod, kmod);
    }
    if (sign < 0) cBigNumberNeg (p, p);
  }
}

void    cBigNumberMRmSqrt (
                        EXPTR(CBNL) p1,
                        EXPTR(CBNL) p
        )
{
  assert (cBigNumberIsFit (p1));
  assert (p1 != p);
  size_t n1 = (size_t)(*p1);
  assert (p1 [n1] >= 0);

  cBigTemp cBigBuf2;
  cBigBuf2.checkexpand (n1 + 1);
  EXPTR(CBNL) pp2 = EXPTRTYPE(cBigBuf2);
  *pp2 = 0;
  cBigNumberFitTo (pp2, n1);
#ifndef _CBIGNUM_MT
  _cBigNumberSkip = 1;
#endif

  CBNL nbit = (cBigNumberBits (p1) + 1) & ~1;
  if ((nbit -= 2) >= 0)
  {

    {
      size_t nw = (size_t)(nbit / BITS);
      CBNL lw = (((CBNL)1) << (nbit % BITS));
      pp2 [nw + 1] += lw;
      assert (pp2 [(size_t)(*pp2)] >= 0);
      p1  [nw + 1] -= lw;
      assert (p1  [(size_t)(*p1)]  >= 0);
    }

    while ((nbit -= 2) >= 0)
    {
      size_t nw = (size_t)(nbit / BITS);
      CBNL lw = (((CBNL)1) << (nbit % BITS));
      pp2 [nw + 1] += lw;
      if (cBigNumberComp (p1, pp2) < 0)
      {
        pp2 [nw] = (CBNL)(n1 - nw);
        pp2 [nw + 1] -= lw;
        cBigNumberMDiv2D (pp2 + nw);
      }
      else
      {
        pp2 [nw] = (CBNL)(n1 - nw);
        cBigNumberMSubD (p1, pp2 + nw, nw);
        assert (p1 [(size_t)(*p1)] >= 0);
        assert (cBigNumberIsFit (p1));
        pp2 [nw + 1] -= lw;
        cBigNumberMDiv2D (pp2 + nw);
        pp2 [nw + 1] += lw;
      }
      n1 = (size_t)(pp2 [nw] + nw);
      pp2 [nw] = 0;
      pp2 [0] = (CBNL)n1;
      assert (pp2 [(size_t)(*pp2)] >= 0);
      assert (cBigNumberIsFit (pp2));
    }
  }

  cBigNumberFit (pp2);
  cBigNumberCopy (pp2, p);

  assert (cBigNumberComp ((cBigNumberMMul2M (pp2), pp2), p1) >= 0);
}

void    cBigNumberRandom (
                unsigned long (*pfnRand)(),
                unsigned CBNL lBits,
                        EXPTR(CBNL) p
        )
{
  CBNL   l1 = lBits % BITS;
  size_t n1 = (size_t)(lBits / BITS + 1);

  if (l1 != 0) l1 = cLongRandom (*pfnRand) &
                    ((unsigned CBNL)~0L >> (BITS-(size_t)l1));
  p [0] = (CBNL)n1;
  p [n1] = l1;
  while (--n1 != 0) p [n1] = cLongRandom (*pfnRand);
  cBigNumberFit (p);
  assert (p [(size_t)(*p)] >= 0);
}

#if UCBNL_MAX > ULONG_MAX

#define LBITS   (CHAR_BIT * sizeof (long))

unsigned CBNL _CBNL_C cLongRandom (
                unsigned long (*pfnRand)()
)
{
  unsigned CBNL temp = (*pfnRand)();
  int sh = LBITS; do temp |= (((CBNL)(*pfnRand)()) << LBITS); while ((sh += LBITS) < BITS);
  return (temp);
}

#endif

#ifdef  NCHECKPTR
#ifdef  __cplusplus
}
#endif
#endif

#endif
