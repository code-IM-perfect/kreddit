#pragma once

#include <QtNetworkAuth/qoauth2authorizationcodeflow.h>

#include <QtNetwork/qnetworkrequestfactory.h>

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qpointer.h>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qtmetamacros.h>

QT_FORWARD_DECLARE_CLASS(QRestAccessManager)

class RedditModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString requestUrl READ requestUrl WRITE setRequestUrl NOTIFY requestUrlChanged)
    Q_PROPERTY(int authStatus READ authStatus WRITE setAuthStatus NOTIFY authStatusChanged)

public:
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
    void requestUrlChanged();
    void authStatusChanged();

    void updateposts();

    // REQUEST URL
    void setRequestUrl(const QString &url)
    {
        if (url != m_requestUrl) {
            m_requestUrl = url;
            Q_EMIT requestUrlChanged();
        }
    }
    QString requestUrl() const
    {
        return m_requestUrl;
    }

    // AUTH STATUS
    void setAuthStatus(const int status)
    {
        if (status != m_authStatus) {
            m_authStatus = status;
            Q_EMIT authStatusChanged();
        }
    }
    int authStatus() const
    {
        return m_authStatus;
    }

    void getToken();

private:
    QString m_requestUrl = QLatin1String("https://oauth.reddit.com/hot?limit=200&sr_detail=1");
    int m_authStatus;

    qint64 getTokenExpiry() const;

    // Init Network
    QNetworkRequestFactory redditApi;
    QRestAccessManager *network = nullptr;
    QOAuth2AuthorizationCodeFlow oauth2;

    // The Actual List
    QList<QJsonObject> posts;

    bool newExpiry;
};
