

add_executable(main 
                main.cpp 
                le.cpp
                languajeAnalisis/parseHtml.cpp
                languajeAnalisis/parseCss.cpp
                utils.cpp
                )

target_link_libraries(main lipsLexer lipsParser)