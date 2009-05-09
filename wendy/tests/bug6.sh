#!/usr/bin/env bash

if test "$FIONA" = "not found"; then
  # The exit 77 is the magic value which tells Automake's `make check'
  # that this test was skipped.
  exit 77
fi

wendy bug6.owfn --og --verbose &>result.log
fiona -tequivalence bug6.og bug6.fiona.og >>result.log 2>&1

