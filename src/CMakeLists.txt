add_executable(pim-compiler
  main.cpp
  PimBackend.cpp
  MatrixMultTranslator.cpp
)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(pim-compiler ${llvm_libs})
