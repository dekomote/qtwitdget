/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QThreadPool>
#include <QNetworkReply>
#include "qtweetnetbase.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetlist.h"
#include "qjson/parserrunnable.h"

/*!
    Constructor
 */
QTweetNetBase::QTweetNetBase(QObject *parent) :
    QObject(parent), m_oauthTwitter(0)
{
}

/*!
    Constructor
    \param oauthTwitter OAuth Twitter
    \param parent QObject parent
 */
QTweetNetBase::QTweetNetBase(OAuthTwitter *oauthTwitter, QObject *parent) :
        QObject(parent), m_oauthTwitter(oauthTwitter)
{

}

QTweetNetBase::~QTweetNetBase()
{
}

/*!
    Sets OAuth Twitter authorization
    \param oauthTwitter OAuth Twitter
 */
void QTweetNetBase::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/*!
    Gets OAuth Twitter authorization
    \return OAuth Twitter
 */
OAuthTwitter* QTweetNetBase::oauthTwitter() const
{
    return m_oauthTwitter;
}

QByteArray QTweetNetBase::response() const
{
    return m_response;
}

void QTweetNetBase::setJsonParsingEnabled(bool enable)
{
    m_jsonParsingEnabled = enable;
}

bool QTweetNetBase::isJsonParsingEnabled() const
{
    return m_jsonParsingEnabled;
}

void QTweetNetBase::parseJson(const QByteArray &jsonData)
{
    QJson::ParserRunnable *jsonParser = new QJson::ParserRunnable;
    jsonParser->setData(jsonData);

    connect(jsonParser, SIGNAL(parsingFinished(QVariant,bool,QString)),
            this, SLOT(parsingJsonFinished(QVariant,bool,QString)));

    QThreadPool::globalInstance()->start(jsonParser);
}

void QTweetNetBase::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
         m_response = reply->readAll();
        emit finished(m_response);

        if (isJsonParsingEnabled())
            parseJson(m_response);

        reply->deleteLater();
    }
}

QList<QTweetStatus> QTweetNetBase::variantToStatusList(const QVariant &fromParser)
{
    QList<QTweetStatus> statuses;

    QList<QVariant> listStatus = fromParser.toList();

    foreach (const QVariant& status, listStatus) {
        QVariantMap statusMap = status.toMap();

        QTweetStatus tweetStatus = variantMapToStatus(statusMap);

        statuses.append(tweetStatus);
    }
    return statuses;
}

QTweetStatus QTweetNetBase::variantMapToStatus(const QVariantMap &var)
{
    QTweetStatus status;

    status.setCreatedAt(var["created_at"].toString());
    status.setText(var["text"].toString());
    status.setId(var["id"].toLongLong());
    status.setInReplyToUserId(var["in_reply_to_user_id"].toLongLong());
    status.setInReplyToScreenName(var["in_reply_to_screen_name"].toString());

    QVariantMap userMap = var["user"].toMap();
    QTweetUser user = variantMapToUserInfo(userMap);

    status.setUser(user);
    status.setSource(var["source"].toString());
    status.setInReplyToStatusId(var["in_reply_to_status_id"].toLongLong());

    //check if contains native retweet
    if (var.contains("retweeted_status")) {
        QVariantMap retweetMap = var["retweeted_status"].toMap();

        QTweetStatus rtStatus = variantMapToStatus(retweetMap);

        status.setRetweetedStatus(rtStatus);
    }

    return status;
}

QList<QTweetDMStatus> QTweetNetBase::variantToDirectMessagesList(const QVariant &fromParser)
{
    QList<QTweetDMStatus> directMessages;

    QList<QVariant> listMessages = fromParser.toList();

    foreach (const QVariant& message, listMessages) {
        QTweetDMStatus dmStatus = variantMapToDirectMessage(message.toMap());
        directMessages.append(dmStatus);
    }

    return directMessages;
}

QTweetDMStatus QTweetNetBase::variantMapToDirectMessage(const QVariantMap &var)
{
    QTweetDMStatus directMessage;

    directMessage.setCreatedAt(var["created_at"].toString());
    directMessage.setSenderScreenName(var["sender_screen_name"].toString());

    QVariantMap senderMap = var["sender"].toMap();
    QTweetUser sender = variantMapToUserInfo(senderMap);

    directMessage.setSender(sender);

    directMessage.setText(var["text"].toString());
    directMessage.setRecipientScreenName(var["recipient_screen_name"].toString());
    directMessage.setId(var["id"].toLongLong());

    QVariantMap recipientMap = var["recipient"].toMap();
    QTweetUser recipient = variantMapToUserInfo(recipientMap);

    directMessage.setRecipient(recipient);

    directMessage.setRecipientId(var["recipient_id"].toLongLong());
    directMessage.setSenderId(var["sender_id"].toLongLong());

    return directMessage;
}

QTweetUser QTweetNetBase::variantMapToUserInfo(const QVariantMap &var)
{
    QTweetUser userInfo;

    userInfo.setId(var["id"].toLongLong());

    //don't fill rest of it, when user info is trimmed
    if (var.contains("name")) {

        userInfo.setName(var["name"].toString());
        userInfo.setLocation(var["location"].toString());
        userInfo.setprofileImageUrl(var["profile_image_url"].toString());
        userInfo.setCreatedAt(var["created_at"].toString());
        userInfo.setFavouritesCount(var["favourites_count"].toInt());
        userInfo.setUrl(var["url"].toString());
        userInfo.setUtcOffset(var["utc_offset"].toInt());
        userInfo.setProtected(var["protected"].toBool());
        userInfo.setFollowersCount(var["followers_count"].toInt());
        userInfo.setVerified(var["verified"].toBool());
        userInfo.setGeoEnabled(var["geo_enabled"].toBool());
        userInfo.setDescription(var["description"].toString());
        userInfo.setTimezone(var["time_zone"].toString());
        userInfo.setFriendsCount(var["friends_count"].toInt());
        userInfo.setStatusesCount(var["statuses_count"].toInt());
        userInfo.setScreenName(var["screen_name"].toString());
        userInfo.setContributorsEnabled(var["contributors_enabled"].toBool());
        userInfo.setListedCount(var["listed_count"].toInt());
        userInfo.setLang(var["lang"].toString());

        //check if contains status
        if (var.contains("status")) {
            QVariantMap statusMap = var["status"].toMap();

            QTweetStatus status = variantMapToStatus(statusMap);

            userInfo.setStatus(status);
        }
    }

    return userInfo;
}

QList<QTweetUser> QTweetNetBase::variantToUserInfoList(const QVariant &fromParser)
{
    QList<QTweetUser> users;

    QList<QVariant> listUsers = fromParser.toList();

    foreach (const QVariant& user, listUsers) {
        QTweetUser userInfo = variantMapToUserInfo(user.toMap());

        users.append(userInfo);
    }

    return users;
}

QTweetList QTweetNetBase::variantMapToTweetList(const QVariantMap &var)
{
    QTweetList list;

    list.setMode(var["mode"].toString());
    list.setDescription(var["description"].toString());
    list.setFollowing(var["following"].toBool());
    list.setMemberCount(var["member_count"].toInt());
    list.setFullName(var["full_name"].toString());
    list.setSubscriberCount(var["subscriber_count"].toInt());
    list.setSlug(var["slug"].toString());
    list.setName(var["name"].toString());
    list.setId(var["id"].toLongLong());
    list.setUri(var["uri"].toString());

    if (var.contains("user")) {
        QVariantMap userMap = var["user"].toMap();

        QTweetUser user = variantMapToUserInfo(userMap);

        list.setUser(user);
    }
    return list;
}

QList<QTweetList> QTweetNetBase::variantToTweetLists(const QVariant &var)
{
    QList<QTweetList> lists;

    QList<QVariant> varLists = var.toList();

    foreach (const QVariant& varlist, varLists) {
        QTweetList tweetlist = variantMapToTweetList(varlist.toMap());

        lists.append(tweetlist);
    }

    return lists;
}
