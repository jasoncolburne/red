# Copyright (C) 2015, Redbeard Enterprises Ltd.
#
# Not to be used in any form without express written consent.
# All rights reserved.
#
# $Id: makefile 22 2010-04-28 19:25:22Z jason $

RUMPATH = redrum

# if we don't have redrum built yet we can't include this
ifneq ("","$(wildcard $(RUMPATH)/target.mk)")
  include $(RUMPATH)/target.mk
endif

REVISION="`git symbolic-ref HEAD 2> /dev/null | cut -b 12-`-`git log --pretty=format:\"%h\" -1 | sed s/\"//g`"

VERSION_MAJOR = $(word 1,$(subst _, ,$(shell cat version)))
VERSION_MINOR = $(word 2,$(subst _, ,$(shell cat version)))
VERSION_POINT = $(word 3,$(subst _, ,$(shell cat version)))

VERSION_STRING =$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_POINT)

build:
	@$(MAKE) -w -C src
	@$(MAKE) -w -C lib

test: build
	@$(MAKE) -w -C src test

prepare:
#	@make -w -C pkg prepare

dist:
	: creating release package...
	@cp -f changelog.txt changelogtmp.txt
	$(SED_IN_PLACE) "s/???/$(REVISION)/g" changelogtmp.txt
	@ruby $(RUMPATH)/scripts/package.rb release_manifest.yaml $(VERSION_STRING) $(RED_BUILD) $(RED_TARGET)
	@rm -f changelogtmp.txt

%:
	@$(MAKE) -w -C src $@
	@$(MAKE) -w -C lib $@
