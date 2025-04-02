add_executable(kreddit)

ecm_add_qml_module(kreddit
    URI
    org.kde.kreddit
)

target_sources(kreddit
    PRIVATE
    main.cpp
    api/redditmodel.cpp api/redditmodel.h
)

ecm_target_qml_sources(kreddit
    SOURCES
    Main.qml
)

target_link_libraries(kreddit
    PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
    kreddit-ui

    Qt6::Network
    Qt6::NetworkAuth
)

install(TARGETS kreddit ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(ui)
