#!/bin/zsh

cd documentation
rm -rf html
doxygen Doxyfile
mkdir -p html/readme_misc
cd html/readme_misc
ln -s ../../../readme_misc/_DSF3371_edit.jpg _DSF3371_edit.jpg
open ../index.html
