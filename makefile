# Copyright (C) 2010, Redbeard Enterprises Ltd.
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

SVNPKGREPO:=https://colburne.kicks-ass.org:987/repos/packages
REDRUMVER :=0.1.5-r23

VERSION_MAJOR = $(word 1,$(subst _, ,$(shell cat version)))
VERSION_MINOR = $(word 2,$(subst _, ,$(shell cat version)))
VERSION_POINT = $(word 3,$(subst _, ,$(shell cat version)))
REVISION := $(subst :, ,$(shell svnversion -nc .))
REVISION := $(word $(words $(REVISION)),$(REVISION))
VERSION_STRING =$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_POINT)-r$(REVISION)

build:
	@$(MAKE) -w -C src
	@$(MAKE) -w -C lib

test: build
	@$(MAKE) -w -C src test

prepare:
	: fetching redrum...
	@svn export $(SVNPKGREPO)/redrum/redrum-$(REDRUMVER).zip
	@unzip redrum-$(REDRUMVER).zip
	@rm -rf redrum-$(REDRUMVER).zip
#	@make -w -C pkg prepare

dist:
	: creating release package...
	@cp -f changelog.txt changelogtmp.txt
	@$(SED_IN_PLACE) "s/???/$(REVISION)/g" changelogtmp.txt
	@ruby $(RUMPATH)/scripts/package.rb release_manifest.yaml $(VERSION_STRING) $(RED_TARGET) $(RED_BUILD)
	@rm -f changelogtmp.txt

%:
	@$(MAKE) -w -C src $@
	@$(MAKE) -w -C lib $@
