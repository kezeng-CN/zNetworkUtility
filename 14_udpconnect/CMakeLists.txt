add_executable(udpconnectclient udpconnectclient.c)
target_link_libraries(udpconnectclient NetworkUtility)

add_executable(udpconnectserver udpconnectserver.c)
target_link_libraries(udpconnectserver NetworkUtility)

add_executable(udpconnectclient02 udpconnectclient02.c)
target_link_libraries(udpconnectclient02 NetworkUtility)


