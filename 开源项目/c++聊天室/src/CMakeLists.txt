INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/erro)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/clientlogin)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/serverlogin)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/mysql_connection)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ADD_EXECUTABLE(client client.c)
ADD_EXECUTABLE(server server.c)

TARGET_LINK_LIBRARIES(client clientlogin)
TARGET_LINK_LIBRARIES(client mysql_connection)
TARGET_LINK_LIBRARIES(client erro)           #链接的顺序不能随意改变!!!
TARGET_LINK_LIBRARIES(client -lpthread ) #添加pthread的链接库
TARGET_LINK_LIBRARIES(client  -lmysqlclient) #添加mysql的链接库

TARGET_LINK_LIBRARIES(server serverlogin)
TARGET_LINK_LIBRARIES(server erro)
TARGET_LINK_LIBRARIES(server mysql_connection)
TARGET_LINK_LIBRARIES(server -lpthread) #添加pthread的链接库
TARGET_LINK_LIBRARIES(server -lmysqlclient) #添加mysql的链接库

