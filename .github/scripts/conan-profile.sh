#!/bin/bash

set -e

conan profile detect -f

std=20
version=11
  

profile="$(conan profile path default)"

mv "$profile" "${profile}.bak"
sed -e 's/^\(compiler\.cppstd=\).\{1,\}$/\1'"$std/" \
    -e 's/^\(compiler\.version=\).\{1,\}$/\1'"$version/" \
    "${profile}.bak" > "$profile"
rm "${profile}.bak"
