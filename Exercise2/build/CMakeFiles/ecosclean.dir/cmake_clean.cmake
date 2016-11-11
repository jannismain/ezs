file(REMOVE_RECURSE
  "app.bin"
  "app.srec"
  "app.lst"
  "CMakeFiles/ecosclean"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/ecosclean.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
