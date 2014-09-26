include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${GTEST_INCLUDE_DIRS}
  )

cxx_test(TiCoreTests . TiCore ${JavaScriptCore_LIBRARIES})
