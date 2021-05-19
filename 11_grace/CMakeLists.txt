add_executable(graceclient graceclient.c)
target_link_libraries(graceclient NetworkUtility)

add_executable(graceserver graceserver.c)
target_link_libraries(graceserver NetworkUtility)

