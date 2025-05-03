import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.AbstractCard {
    id: postCard
    header: ColumnLayout {
        RowLayout {
            Kirigami.Icon {
                id: subredditIcon
                source: model.subreddit_icon
            }
            ColumnLayout {
                Controls.Label {
                    text: model.subreddit
                }
                Controls.Label {
                    text: model.author
                }
            }
            Controls.Label {
                text: new Date(model.timestamp * 1000).toLocaleString(Qt.locale(), Locale.ShortFormat)
            }
        }
        Kirigami.Heading {
            Layout.fillWidth: true
            text: model.title
            level: 2
            wrapMode: Text.WordWrap
        }
        Controls.Label {
            Layout.fillWidth: true
            text: "score: " + model.score + "\n" + "timestamp: " + model.timestamp + "\n" + "num_comments: " + model.num_comments + "\n" + "permalink: " + model.permalink + "\n" + "self_text: " + model.self_text + "\n" + "type: " + model.type + "\n" + "preview_image: " + model.preview_image + "\n" + "url: " + model.url + "\n" + "subreddit_icon: " + model.subreddit_icon
            wrapMode: Text.WordWrap
        }
        Controls.Label {
            text: "ScreenWidth: " + Screen.width + "\n" + "ParentWidth: " + parent.width + "\n" + "Width: " + width + "\n"
        }
    }
    // contentItem: Rectangle {
    //     id: postMediaContainer
    //     Layout.preferredHeight: Screen.height / 2.7
    //     Layout.fillWidth: true
    //     color: "red"
    //     // Image {
    //     //     id: postImage
    //     //     visible: model.type == "image"
    //     //     source: model.url
    //     // }
    //     // AnimatedImage {
    //     //     id: animatedImage
    //     //     visible: model.type == "gif"
    //     // }
    //     // Controls.Label {
    //     //     visible: model.type != "image" && model.type != "gif"
    //     //     text: "Sorry kreddit does not support " + model.type + " yet"
    //     // }
    //     ColumnLayout {
    //         anchors.fill: parent
    //         Controls.Label {
    //             text: "score: " + model.score + "\n" + "timestamp: " + model.timestamp + "\n" + "num_comments: " + model.num_comments + "\n" + "permalink: " + model.permalink + "\n" + "self_text: " + model.self_text + "\n" + "type: " + model.type + "\n" + "preview_image: " + model.preview_image + "\n" + "url: " + model.url + "\n" + "subreddit_icon: " + model.subreddit_icon
    //         }
    //     }
    // }
    contentItem: RowLayout {
        // Layout.fillHeight: true
        RowLayout {
            Controls.Button {
                id: upvote
                icon.name: "go-up-symbolic"
            }
            Controls.Label {
                text: model.score
            }
            Controls.Button {
                id: downvote
                icon.name: "go-down-symbolic"
            }
        }
        Controls.Button {
            id: commentsButton
            icon.name: "dialog-messages"
            text: model.num_comments + " Comments"
        }
        Controls.Button {
            id: savePostButton
            icon.name: "bookmarks"
            text: "Add to Saves"
        }
    }
}
