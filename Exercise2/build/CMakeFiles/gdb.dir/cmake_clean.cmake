file(REMOVE_RECURSE
  "app.bin"
  "app.srec"
  "app.lst"
  "CMakeFiles/gdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/gdb.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
