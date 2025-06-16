add_library(LibMailPlugin
    EmailAddress.h
    Identity.h
    Lazy.h
    MailSettings.cpp
    MailSettings.h
    Rule.h
    Signature.h
)

find_package(glaze)
find_package(Qt6 REQUIRED COMPONENTS Core)

target_link_libraries(LibMailPlugin PUBLIC glaze::glaze Qt6::Core)
