#pragma once

#include <QtNetworkAuth/qoauth2authorizationcodeflow.h>

#include <QtNetwork/qnetworkrequestfactory.h>

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qpointer.h>
#include <qcontainerfwd.h>
#include <qnamespace.h>

QT_FORWARD_DECLARE_CLASS(QRestAccessManager)

class RedditModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    // RedditModel(QObject *parent = nullptr);
    RedditModel(QObject *parent = nullptr);

    enum Roles {
        IdRole,
        TitleRole,
        SubRedditRole,
        AuthorRole,
        ScoreRole,
        CreationTimeRole,
        NumCommentsRole,
        PermalinkRole,
        SelfTextRole,
        TypeRole,
        PreviewRole,
        UrlRole,
        SubredditIconRole,
    };

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    // signals:
    void error(const QString &errorString);
    void onGranted();

    void updateposts();

    QString requestUrl = QLatin1String("https://oauth.reddit.com/hot?limit=200&sr_detail=1");

    void getToken(const QString &reqUrl);

private:
    qint64 getTokenExpiry() const;

    // Init Network
    QNetworkRequestFactory redditApi;
    QRestAccessManager *network = nullptr;
    QOAuth2AuthorizationCodeFlow oauth2;

    // The Actual List
    QList<QJsonObject> posts;

    bool newExpiry;
};
