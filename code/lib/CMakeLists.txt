
set(SAS_LIBRARY_SRCS
  file_loader_data_creator.cpp
  solver.cpp
  
)

set(SAS_LIBRARY_HDRS
  basic_types.h
  data_creator.h
  file_loader_data_creator.h
  solver.h
)

add_library(sas ${SAS_LIBRARY_HDRS} ${SAS_LIBRARY_SRCS})

target_link_libraries(sas ${ALL_THIRD_PARTY_LIBRARIES})

