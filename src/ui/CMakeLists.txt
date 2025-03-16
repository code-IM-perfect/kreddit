add_library(kreddit-ui)

ecm_add_qml_module(kreddit-ui
    URI "org.kde.kreddit.ui"
    GENERATE_PLUGIN_SOURCE
)

# ecm_target_qml_sources(kreddit-ui
#     SOURCES
#     AddDialog.qml
#     KountdownDelegate.qml
# )

ecm_finalize_qml_module(kreddit-ui)

install(TARGETS kreddit-ui ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
