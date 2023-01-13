enable_testing()

add_executable(BasicTest
    basic.cpp
    basic.h
    sampletable.h
    sampletable.cpp
    parenttable.h
    parenttable.cpp
    db.h
    db.cpp
)

add_test(NAME BasicTest COMMAND BasicTest)
target_include_directories(BasicTest PRIVATE ../../src)

target_link_libraries(BasicTest
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    Nut
)
