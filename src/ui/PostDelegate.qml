import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.AbstractCard {
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
        }
        Kirigami.Heading {
            text: model.title
            level: 2
            wrapMode: Text.WordWrap
        }
    }
}
