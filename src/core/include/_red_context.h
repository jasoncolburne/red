/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_context.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___RED_CONTEXT_H__
#define ___RED_CONTEXT_H__


struct _RedContext {
  redfuncMalloc rfMalloc;
  redfuncFree   rfFree;
  redfuncMemcpy rfMemcpy;
  redfuncMemset rfMemset;
  redfuncMemcmp rfMemcmp;

  void* data;
};


#endif /* __coreGlobalContext_h__ */
