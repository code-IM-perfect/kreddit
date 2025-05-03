#include "redditmodel.h"

#include <QtNetworkAuth/qoauthhttpserverreplyhandler.h>

#include <QtGui/qdesktopservices.h>

#include <QtNetwork/qrestaccessmanager.h>
#include <QtNetwork/qrestreply.h>

#include <QtCore/qjsonarray.h>
#include <QtCore/qjsondocument.h>
#include <qassert.h>
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qset.h>
#include <qsettings.h>
#include <qstringliteral.h>
#include <qtmetamacros.h>

using namespace Qt::StringLiterals;

static constexpr auto clientId = "nGnecoqXrZOzaxkqlO0Vtw"_L1;
static constexpr auto authorizationUrl = "https://www.reddit.com/api/v1/authorize"_L1;
static constexpr auto accessTokenUrl = "https://www.reddit.com/api/v1/access_token"_L1;

QHash<int, QByteArray> RedditModel::roleNames() const
{
    return {
        {TitleRole, "title"},
        {SubRedditRole, "subreddit"},
        {AuthorRole, "author"},
        {ScoreRole, "score"},
        {CreationTimeRole, "timestamp"},
        {NumCommentsRole, "num_comments"},
        {PermalinkRole, "permalink"},
        {SelfTextRole, "self_text"},
        {TypeRole, "type"},
        {PreviewRole, "preview_image"},
        {UrlRole, "url"},
        {SubredditIconRole, "subreddit_icon"},
    };
}

void RedditModel::requestUrlChanged()
{
    return;
}

void RedditModel::authStatusChanged()
{
    return;
}

qint64 RedditModel::getTokenExpiry() const
{
    int expiresIn;
    QVariantMap tokenInfo = oauth2.extraTokens();
    if (tokenInfo.contains("expires_in"_L1)) {
        expiresIn = tokenInfo["expires_in"_L1].toInt();
    } else {
        expiresIn = 3600;
        qDebug() << "Did not find 'expires_in' in response, setting it to 3600 (according to reddit api documentation)";
    }
    return (QDateTime::currentSecsSinceEpoch() + expiresIn);
}

void RedditModel::getToken()
{
    // TODO: Save and Fetch from QSettings
    QSettings settings;

    QString accessToken = settings.value("accessToken").toString();
    QString refreshToken = settings.value("refreshToken").toString();
    qint64 expiryTime = settings.value("expiryTime").toLongLong();

    if (!accessToken.isEmpty() && QDateTime::currentSecsSinceEpoch() < expiryTime) {
        qDebug() << "Access Token is still valid, continuing without it";
        oauth2.setToken(accessToken);

        // Emit granted() to trigger onGranted()
        Q_EMIT oauth2.granted();
        return;
    }

    if (!refreshToken.isEmpty()) {
        qDebug() << "Access token expired but refresh token found, starting token refresh.";
        newExpiry = true;
        oauth2.setRefreshToken(refreshToken);
        oauth2.refreshTokens();
        return;
    }

    qDebug() << "Could not get stored keys, starting full auth process from the begining";
    newExpiry = true;
    oauth2.grant();
}

RedditModel::RedditModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    QNetworkAccessManager *qnam = new QNetworkAccessManager(this);
    network = new QRestAccessManager(qnam, qnam);

    // set request url
    redditApi.setBaseUrl(QUrl(requestUrl()));

    auto replyHandler = new QOAuthHttpServerReplyHandler(QHostAddress::Any, 1337, this);
    oauth2.setReplyHandler(replyHandler);
    oauth2.setAuthorizationUrl(QUrl(authorizationUrl));
    oauth2.setTokenUrl(QUrl(accessTokenUrl));
    oauth2.setClientIdentifier(clientId);
    const QSet<QByteArray> scope = {QByteArray("identity"), QByteArray("read")};
    oauth2.setRequestedScopeTokens(scope);

    oauth2.setModifyParametersFunction([](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
        // qDebug() << "trying to modify parameters";
        if (!parameters) {
            qDebug() << "modifying parameters failed: could not access parameters";
            return; // Return if pointer is invalid
        }

        if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
            parameters->insert("duration"_L1, "permanent"_L1); // Add duration parameter
            qDebug() << "Modified parameters for permanent access";
        }
    });

    QObject::connect(&oauth2, &QAbstractOAuth::granted, this, &RedditModel::onGranted);
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, this, &QDesktopServices::openUrl);

    getToken();
}

void RedditModel::onGranted()
{
    QSettings settings;
    settings.setValue("accessToken", oauth2.token());
    settings.setValue("refreshToken", oauth2.refreshToken());

    if (oauth2.refreshToken().isEmpty()) {
        qDebug() << "Access granted but refresh token was not recieved";
    }

    if (newExpiry) {
        settings.setValue("expiryTime", RedditModel::getTokenExpiry());
    }

    settings.sync();

    redditApi.setBearerToken(oauth2.token().toLatin1());
    updateposts();
}

int RedditModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return posts.size();
}

int RedditModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return posts.size() ? 1 : 0;
}

QVariant RedditModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid())
        return QVariant();

    const auto childrenObject = posts.at(index.row());
    Q_ASSERT(childrenObject.value("data"_L1).isObject());
    const auto dataObject = childrenObject.value("data"_L1).toObject();

    switch (role) {
    case IdRole:
        return dataObject.value("id"_L1).toString();
    case TitleRole:
        // qDebug() << dataObject;
        return dataObject.value("title"_L1).toString();
    case SubRedditRole:
        return dataObject.value("subreddit_name_prefixed"_L1).toString();
    case AuthorRole:
        return dataObject.value("author"_L1).toString();
    case ScoreRole:
        return dataObject.value("score"_L1).toInt();
    case CreationTimeRole:
        return dataObject.value("created"_L1).toInt();
    case NumCommentsRole:
        return dataObject.value("num_comments"_L1).toInt();
    case PermalinkRole:
        return dataObject.value("permalink"_L1).toString();
    case SelfTextRole:
        return dataObject.value("selftext"_L1).toString();
    case TypeRole: {
        const auto domain = dataObject.value("domain"_L1).toString();
        const auto url = dataObject.value("url"_L1).toString();
        if (!QString::compare(domain, "i.redd.it"_L1)) {
            if (dataObject.value("url"_L1).toString().endsWith(".gif"_L1)) {
                return "gif"_L1;
            }
            return "image"_L1;
        } else if (!QString::compare(domain, "v.redd.it"_L1)) {
            return "video"_L1;
        } else if (url.startsWith("https://www.reddit.com/gallery"_L1)) {
            return "image_gallery"_L1;
        } else if (domain.startsWith("self."_L1)) {
            return "internal"_L1;
        }
        return "external"_L1;
    }
    case PreviewRole: {
        Q_ASSERT(dataObject.value("preview"_L1).isObject());
        const auto previewObject = dataObject.value("preview"_L1).toObject();
        Q_ASSERT(previewObject.value("images"_L1).isArray());
        const auto imagesArray = previewObject.value("images"_L1).toArray();
        // TODO: Implement Getting fitting resolution Logic
        Q_ASSERT(imagesArray.first().isObject());
        const auto firstImage = imagesArray.first().toObject();
        return firstImage.value("source"_L1).toObject().value("url"_L1).toString();
    }
    case SubredditIconRole: {
        Q_ASSERT(dataObject.value("sr_detail"_L1).isObject());
        const auto srDetails = dataObject.value("sr_detail"_L1).toObject();
        return srDetails.value("icon_img"_L1).toString();
    }
    case UrlRole: {
        // TODO: Handle video and gallery pages
        if (dataObject.value("url"_L1).toString().isEmpty()) {
            return dataObject.value("url_overridden_by_dest"_L1).toString();
        }
        return dataObject.value("url"_L1).toString();
    }
    default:
        // Fallback to Id
        return dataObject.value("id"_L1).toString();
    }

    return QVariant();
}

void RedditModel::updateposts()
{
    network->get(redditApi.createRequest(), this, [this](QRestReply &reply) {
        if (!reply.isSuccess()) {
            // Q_EMIT error(reply.errorString());
            qDebug() << "Request to Reddit API failed";

            qDebug() << "Retrying...";
            oauth2.grant();
            return;
        }
        const auto document = reply.readJson();
        Q_ASSERT(document && document->isObject());
        const auto rootObject = document->object();
        Q_ASSERT(rootObject.value("kind"_L1).toString() == "Listing"_L1);
        const auto dataValue = rootObject.value("data"_L1);
        Q_ASSERT(dataValue.isObject());
        const auto dataObject = dataValue.toObject();
        const auto childrenValue = dataObject.value("children"_L1);
        Q_ASSERT(childrenValue.isArray());
        const auto childrenArray = childrenValue.toArray();

        if (childrenArray.isEmpty())
            return;

        beginInsertRows(QModelIndex(), posts.size(), childrenArray.size() + posts.size() - 1);
        for (const auto childValue : std::as_const(childrenArray)) {
            Q_ASSERT(childValue.isObject());
            posts.append(childValue.toObject());
        }
        endInsertRows();
    });
}
