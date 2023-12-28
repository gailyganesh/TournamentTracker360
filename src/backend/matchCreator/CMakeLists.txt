project(MatchCreator)

add_library(${PROJECT_NAME}
			include/MatchCreator.h
			MatchCreator.cpp
			)
target_include_directories(${PROJECT_NAME}
                        PUBLIC
				include)
				
set_target_properties(${PROJECT_NAME} 
			PROPERTIES
                        FOLDER "backend/matchCreator")
