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

#ifndef QTWEETNETBASE_H
#define QTWEETNETBASE_H

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <QtDebug>
#include "oauthtwitter.h"
#include "qtweetlib_global.h"

class QTweetStatus;
class QTweetUser;
class QTweetDMStatus;
class QTweetList;

/*!
    Base class for Twitter API classes
 */
class QTWEETLIBSHARED_EXPORT QTweetNetBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OAuthTwitter* oauthTwitter READ oauthTwitter WRITE setOAuthTwitter)
    Q_PROPERTY(bool jsonParsing READ isJsonParsingEnabled WRITE setJsonParsingEnabled)
public:
    QTweetNetBase(QObject *parent = 0);
    QTweetNetBase(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    virtual QTweetNetBase::~QTweetNetBase();

    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    OAuthTwitter* oauthTwitter() const;

    void setJsonParsingEnabled(bool enable);
    bool isJsonParsingEnabled() const;

    QByteArray response() const;

signals:
    /*!
        Emited when fetching is finished
        \param response Contains the response
     */
    void finished(const QByteArray& response);

    /*!
        Emited when there is a network error
     */
    void networkError(const QString& errorString);

protected slots:
    virtual void parsingJsonFinished(const QVariant& json, bool ok, const QString& errorMsg) = 0;
    virtual void reply();

protected:
    QList<QTweetStatus> variantToStatusList(const QVariant& fromParser);
    QList<QTweetDMStatus> variantToDirectMessagesList(const QVariant& fromParser);
    QTweetDMStatus variantMapToDirectMessage(const QVariantMap& var);
    QTweetUser variantMapToUserInfo(const QVariantMap& var);
    QTweetStatus variantMapToStatus(const QVariantMap& var);
    QTweetList variantMapToTweetList(const QVariantMap& var);
    QList<QTweetUser> variantToUserInfoList(const QVariant& fromParser);
    QList<QTweetList> variantToTweetLists(const QVariant& var);

    void parseJson(const QByteArray& jsonData);

    QByteArray m_response;

private:
    OAuthTwitter *m_oauthTwitter;
    bool m_jsonParsingEnabled;
};

#endif // QTWEETNETBASE_H
