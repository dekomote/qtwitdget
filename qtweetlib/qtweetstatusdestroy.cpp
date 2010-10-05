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

#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetstatusdestroy.h"
#include "qtweetstatus.h"

QTweetStatusDestroy::QTweetStatusDestroy(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetStatusDestroy::QTweetStatusDestroy(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Destroys tweet with id
    \param id Tweet ID
    \param respType Response type
    \remarks Async
 */
void QTweetStatusDestroy::destroy(qint64 id,
                                  bool trimUser,
                                  bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url("http://api.twitter.com/1/statuses/destroy.json");

    url.addQueryItem("id", QString::number(id));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetStatusDestroy::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetStatus status = variantMapToStatus(json.toMap());

        emit deletedStatus(status);
    } else {
        qDebug() << "QTweetStatusDestroy parse error: " << errorMsg;
    }
}

void QTweetStatusDestroy::error()
{
    // ### TODO:
}
