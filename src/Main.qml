import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kreddit.components

Kirigami.ApplicationWindow {
    id: root

    width: 800
    height: 600

    title: i18nc("@title:window", "Kreddit - Reddit Viewer")

    globalDrawer: Kirigami.GlobalDrawer {
        id: subredditDrawer
        modal: false
        collapsible: true
        collapsed: false

        actions: [
            Kirigami.Action {
                text: i18nc("@globalDrawer:favorites", "Favorite Subreddits")
                icon.name: "application-menu"
                onTriggered: {
                    subredditDrawer.collapsed = !subredditDrawer.collapsed;
                }
            },
            Kirigami.Action {
                text: "r/ProgrammerHumor"
            },
            Kirigami.Action {
                text: "r/itookapicture"
            },
            Kirigami.Action {
                text: "r/xkcd"
            },
            Kirigami.Action {
                text: "r/comics"
            },
            Kirigami.Action {
                text: "r/kde"
            }
        ]
    }

    pageStack.initialPage: Kirigami.ScrollablePage {
        // Kirigami.CardsListView {
        //     id: cardsView
        //     model: postModel
        //     delegate: PostDelegate {}
        // }

        actions: [
            Kirigami.Action {
                id: profileIcon
                icon.name: "user-available-symbolic"
                Kirigami.Action {
                    // icon.source
                    icon.name: "user-available-symbolic"
                    text: "u/userName"
                }
                Kirigami.Action {
                    icon.name: "emblem-system-symblic"
                    text: "Preferences"
                }
                Kirigami.Action {
                    text: i18nc("@globalMenu:about", "About Kreddit")
                    icon.name: "help-about"
                    onTriggered: pageStack.layers.push(aboutPage)
                }
            }
        ]
    }

    Component { // <==== Component that instantiates the Kirigami.AboutPage
        id: aboutPage

        Kirigami.AboutPage {
            aboutData: About
        }
    }
}
