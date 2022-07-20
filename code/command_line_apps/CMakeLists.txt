set(_command_line_apps
  sas_app
)

foreach(_app ${_command_line_apps})
  add_executable(${_app} ${_app}.cpp)
  target_link_libraries(${_app} ${ALL_THIRD_PARTY_LIBRARIES} sas)
endforeach()
