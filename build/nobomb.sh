#!/bin/bash
# nobomb.sh src_dir

for sf in ipdb.h ipdb.cpp ipdb_desc.h main.cpp; do
	vi $1"/"${sf} << EOF
:set nobomb
:wq
EOF
done

