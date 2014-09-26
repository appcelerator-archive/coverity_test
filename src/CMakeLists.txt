add_library(TiCore
  BaseProxy.cpp
  BaseProxy.h
  BaseProxy2.cpp
  BaseProxy2.h
  ExampleProxy.cpp
  ExampleProxy.h
  ExampleProxy2.cpp
  ExampleProxy2.h
  TiProxy.cpp
  TiProxy.h
  TiProxy2.cpp
  TiProxy2.h
  TiRuntime.cpp
  TiRuntime.h
  TiValue.cpp
  TiValue.h
  )

target_link_libraries(TiCore ${JavaScriptCore_LIBRARIES})
