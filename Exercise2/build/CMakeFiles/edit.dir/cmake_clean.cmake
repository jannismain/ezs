file(REMOVE_RECURSE
  "app.bin"
  "app.srec"
  "app.lst"
  "CMakeFiles/edit"
  "ecos/install/lib/target.ld"
  "ecos/makefile"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/edit.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
