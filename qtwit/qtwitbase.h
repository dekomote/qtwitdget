/* Copyright (c) 2009, Antonie Jovanoski
 *	
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWITBASE_H
#define QTWITBASE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "oauth/oauthtwitter.h"

/*! QTwitBase class */
class QTwitBase : public QObject
{
	Q_OBJECT

public:
	/*! Constructor */
	QTwitBase(QObject *parent = 0);
	QTwitBase(QNetworkAccessManager* netManager, OAuthTwitter* oauthTwitter, QObject *parent = 0);

	void setNetworkAccessManager(QNetworkAccessManager* netManager);
	QNetworkAccessManager* networkAccessManager() const;

	void setOAuthTwitter(OAuthTwitter* oauthTwitter);
	OAuthTwitter* oauthTwitter() const;
	
signals:
	/*! emited when finished downloading and processing */
	void finished();
    void networkError(const QString &errorString);

private:
	QNetworkAccessManager *m_netManager;
	OAuthTwitter *m_oauthTwitter;
};

#endif //QTWITBASE_H
