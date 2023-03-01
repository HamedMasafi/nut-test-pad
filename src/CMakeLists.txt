set(PROJECT_SRC
    field2.cpp
    runtimecheckers/fieldchecker.cpp
    runtimecheckers/tablechecker.cpp
    runtimecheckers/foreignkeychecker.cpp
    core/fieldbase.cpp
    core/tablebase.cpp
    core/sqlserializer.cpp
    phrases/abstractfieldphrase.cpp
    phrases/fieldphrase_qstring.cpp
    phrases/phraselist.cpp
    phrases/fieldphrase_numeric.cpp
    phrases/fieldphrase_bool.cpp
    phrases/conditionalphrase.cpp
    phrases/phrasedatalist.cpp
    phrases/assignmentphrase.cpp
    phrases/fieldphrase.cpp
    phrases/assignmentphraselist.cpp
    phrases/fieldphrase_date.cpp
    phrases/phrasedata.cpp
    database.cpp
    query.cpp
    generators/abstractsqlgenerator.cpp
    generators/sqlitegenerator.cpp
    dataset.cpp
    foreignkey.cpp
    namedtype.cpp
    table.cpp
    phrase.cpp
    models/modelstorage.cpp
    models/fieldmodel.cpp
    models/foreignkeymodel.cpp
    expressions/conditionalexpression.cpp
    expressions/expressiondata.cpp
    abstracttable.cpp
    dataset.h
    runtimecheckers/foreignkeychecker.h
    runtimecheckers/tablechecker.h
    runtimecheckers/fieldchecker.h
    core/tablebase.h
    core/fieldbase.h
    core/sqlserializer.h
    core/globalstorage.h
    core/globalstorage.cpp
    core/changelogtable.h
    core/changelogtable.cpp
    querydata.h
    namedtype.h
    phrases/abstractfieldphrase.h
    phrases/phrasedata.h
    phrases/fieldphrase_numeric.h
    phrases/phraselist.h
    phrases/assignmentphrase.h
    phrases/fieldphrase_qstring.h
    phrases/conditionalphrase.h
    phrases/assignmentphraselist.h
    phrases/fieldphrase_date.h
    phrases/fieldphrase.h
    phrases/phrasedatalist.h
    phrases/fieldphrase_bool.h
    database.h
    global.h
    generators/sqlitegenerator.h
    generators/abstractsqlgenerator.h
    query.h
    foreignkey.h
    abstracttable.h
    phrase.h
    field2.h
    models/modelstorage.h
    models/foreignkeymodel.h
    models/fieldmodel.h
    table.h
    expressions/conditionalexpression.h
    expressions/expressiondata.h
    database/databasebase.h database/databasebase.cpp
    database/databasedata.h database/databasedata.cpp
    database/databasemodel.h database/databasemodel.cpp
    database/databasemigratestorage.h database/databasemigratestorage.cpp
)

add_library(Nut SHARED
    ${PROJECT_SRC}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Nut PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(Nut PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)

target_compile_definitions(Nut PRIVATE NUT_LIBRARY)
