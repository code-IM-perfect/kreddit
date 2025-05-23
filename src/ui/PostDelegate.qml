import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.AbstractCard {
    id: postCard
    Layout.maximumWidth: Screen.width / 2.2
    Layout.alignment: Qt.AlignHCenter
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
            Controls.Label {
                Layout.alignment: Qt.AlignRight
                text: model.type
            }
        }
        Kirigami.Heading {
            Layout.fillWidth: true
            text: model.title
            level: 2
            wrapMode: Text.WordWrap
        }
        TextEdit {
            Layout.fillWidth: true
            text: "permalink: " + model.permalink + "\n" + "self_text: " + model.self_text + "\n" + "preview_image: " + model.preview_image + "\n" + "url: " + model.url
            wrapMode: Text.WordWrap
            readOnly: true
            selectByMouse: true
        }
        // Controls.Label {
        //     text: "ScreenWidth: " + Screen.width + "\n" + "HeaderWidth: " + postCard.header.width + "\n" + "PostCardWidth: " + postCard.width + "\n"
        // }
        Image {
            id: postPreview
            // TODO: Implement as Loader
            visible: model.type == "image"
            source: model.preview_image
            sourceSize.height: Screen.height / 2.4
            Layout.preferredHeight: sourceSize.height
            Layout.alignment: Qt.AlignHCenter
        }
        Controls.Label {
            property int imageProgress: Math.round(postPreview.progress * 20)
            visible: postPreview.visible && postPreview.progress != Image.Ready
            text: "Loading Image..."
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
    // }
    contentItem: RowLayout {
        Layout.fillWidth: true
        // Layout.alignment: Qt.AlignHCenter
        RowLayout {
            Controls.Button {
                id: upvote
                icon.name: "go-up-symbolic"
                // TODO: Implement Logic
            }
            Controls.Label {
                text: model.score
            }
            Controls.Button {
                id: downvote
                icon.name: "go-down-symbolic"
                // TODO: Implement Logic
            }
        }
        Controls.Button {
            id: commentsButton
            icon.name: "dialog-messages"
            text: model.num_comments + " Comments"
            // TODO: Implement Logic
        }
        Controls.Button {
            id: savePostButton
            icon.name: "bookmarks"
            text: "Save"
            // TODO: Implement Logic
        }
        Kirigami.Action {
            text: "Share"
            icon.name: "emblem-shared-symbolic"

            Kirigami.Action {
                text: "Copy Link to Post"
                // TODO: Implement Logic
            }
            Kirigami.Action {
                text: "Copy Link to " + model.type
                visible: model.type == "image" || model.type == "video" || model.type == "gif"
                // TODO: Implement Logic
            }
            Kirigami.Action {
                text: "Download Image"
                visible: model.type == "image" || model.type == "gif"
                // TODO: Implement Logic
            }
            // TODO: Implement for other media types
        }
    }
}
