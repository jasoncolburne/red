Copyright (C) 2015, Redbeard Enterprises Ltd.
All rights reserved.

Getting Started:

0. Export target and build environment variables.
$ export RED_TARGET=linux64_x86
$ export RED_BUILD=release

1. Fetch the RedRum build system.
src $ git clone git@github.com:redbeardenterprises/redrum.git

2. Build RedRum as per instructions in readme.

3. Copy redrum package to working directory and unzip.
red $ cp ../redrum/release/redrum-0.1.7-release.zip .
red $ unzip redrum-0.1.7-release.zip

4. Run tests
red $ make test
