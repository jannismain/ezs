file(REMOVE_RECURSE
  "app.bin"
  "app.srec"
  "app.lst"
  "CMakeFiles/ecos"
  "ecos/makefile"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/ecos.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
