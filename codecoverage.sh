#!/bin/bash

lcov -t "tst_vibuttonctrl" -o tst_vibuttonctrl.info -c -d .
genhtml -o report tst_vibuttonctrl.info
