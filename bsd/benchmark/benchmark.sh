#!/bin/bash
# This script computes BSD and CSD for some given owfns and measures the performance of the bsd tool.
PARAMETERS="'directory with bsd checkout' 'input directory' 'output directory' 'bound1' ['bound2'] ..."

# parameter 1: directory with bsd checkout
if [ ! $1 ]; then
   echo "$0: Usage: $0 $PARAMETERS"
   exit 1
fi

if [ ! -d $1 ]; then
   echo "$0: Directory doesn't exist, exiting..."
   exit 1
fi

# parameter 2: directory with owfn files
if [ ! $2 ]; then
   echo "$0: Usage: $0 $PARAMETERS"
   exit 2
fi

if [ ! -d $2 ]; then
   echo "$0: Directory doesn't exist, exiting..."
   exit 2
fi

# parameter 3: directory for output
if [ ! $3 ]; then
   echo "$0: Usage: $0 $PARAMETERS"
   exit 3
fi

if [ ! -d $3 ]; then
   echo "$0: Directory doesn't exist, exiting..."
   exit 3
fi

# parameter 4+: bounds
if [ ! $4 ]; then
   echo "$0: Usage: $0 $PARAMETERS"
   exit 4
fi

for var in "${@:4}"; do
   for i in `find $2 -name "*.owfn" | sort`; do
      file=`basename $i | sed -e 's/\.owfn//'`

      # computes BSD for given owfn
      $1/src/bsd ${i} --bound=${var} --BSD --output=$3/BSD_${var}_${file}.dot --config=$1/src/bsd.conf
      
      # computes CSD for given owfn
      $1/src/bsd ${i} --bound=${var} --CSD --output=$3/CSD_${var}_${file}.dot --config=$1/src/bsd.conf

      # computes mp for previously computed CSD
      $1/src/bsd $3/CSD_${var}_${file}.dot --bound=${var} --mp --output=$3/mp_${var}_${file}.owfn --config=$1/src/bsd.conf

      # computes max for previously computed CSD
      $1/src/bsd $3/CSD_${var}_${file}.dot --bound=${var} --max --output=$3/max_${var}_${file}.owfn --config=$1/src/bsd.conf
   done
done
