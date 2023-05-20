install(
    TARGETS threads_exe
    RUNTIME COMPONENT threads_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
