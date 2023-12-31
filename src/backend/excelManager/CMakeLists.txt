cmake_minimum_required(VERSION 3.11)

project(ExcelManager)

add_library(${PROJECT_NAME}
			include/ExcelManager.h
			ExcelManager.cpp
			)
target_include_directories(${PROJECT_NAME}
                        PUBLIC
				include)

include(FetchContent)

FetchContent_Declare(
    QXlsx
    GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
    GIT_TAG        3222f30
    SOURCE_SUBDIR  QXlsx
)
FetchContent_MakeAvailable(QXlsx)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC
                        MatchCreator
                        QXlsx::QXlsx
                        )
				
set_target_properties(${PROJECT_NAME} 
			PROPERTIES
                        FOLDER "backend/excelManager")
