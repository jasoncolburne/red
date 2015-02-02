# Copyright (C) 2010, Redbeard Enterprises Ltd.
#
# Not to be used in any form without express written consent.
# All rights reserved.
#
# $Id: makefile 18 2010-04-17 05:57:03Z jason $

RUMPATH := ../redrum

ifeq ("", "$(wildcard $(RUMPATH)/target.mk)")
  $(error Please run 'make prepare' from top level to retrieve the redrum build system)
endif

STATICLIBS := red

red := src/core src/data src/file src/stream src/concurrency

include $(RUMPATH)/target.mk
include $(RUMPATH)/lib.mk
