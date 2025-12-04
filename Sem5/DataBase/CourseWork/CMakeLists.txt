cmake_minimum_required(VERSION 3.16)
project(WholesaleFirm VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск пакетов
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(PostgreSQL REQUIRED)

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/DatabaseManager.cpp
    src/ConfigManager.cpp
    src/AuthManager.cpp
    src/LoginDialog.cpp
    src/GoodsDialog.cpp
    src/SalesDialog.cpp
    src/WarehouseDialog.cpp
    src/ReportGenerator.cpp
)

# Заголовочные файлы
set(HEADERS
    src/MainWindow.h
    src/DatabaseManager.h
    src/ConfigManager.h
    src/AuthManager.h
    src/LoginDialog.h
    src/GoodsDialog.h
    src/SalesDialog.h
    src/WarehouseDialog.h
    src/ReportGenerator.h
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Подключение библиотек Qt
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Sql
    ${PostgreSQL_LIBRARIES}
)

# Настройка пути к заголовкам
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Добавляем определение для работы с PostgreSQL в Qt
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    QT_SQL_LIB
    QT_CORE_LIB
    QT_WIDGETS_LIB
)

# Копирование конфигурационного файла в директорию сборки
configure_file(config.ini ${CMAKE_CURRENT_BINARY_DIR}/config.ini COPYONLY)

# Установка
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES config.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})
