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
 */

#include <QtDebug>
#include <QNetworkReply>
#include "qtwitfriends.h"
#include "xml/xmlreaderextusers.h"

QTwitFriends::QTwitFriends(QObject *parent)
	:	QTwitBase(parent)
{
}

QTwitFriends::QTwitFriends(QNetworkAccessManager* netManager, OAuthTwitter* oauthTwitter, QObject *parent)
:	QTwitBase(netManager, oauthTwitter, parent)
{
}

QList<QTwitExtUserInfo> QTwitFriends::getFriends() const
{
	return m_friends;
}

void QTwitFriends::updateFriends(int id, int userId, const QString& screenName, int page)
{
	Q_ASSERT(networkAccessManager() != 0);

	m_friends.clear();

    QUrl url("http://api.twitter.com/1/statuses/friends.xml");

	if(id != 0){
		QString strId = QString("%1").arg(id);
		url.addQueryItem("id", strId);
	}

	if(userId != 0){
		QString strUserId = QString("%1").arg(userId);
		url.addQueryItem("user_id", strUserId);
	}

	if(!screenName.isEmpty()){
		url.addQueryItem("screen_name", screenName);
	}

	if (page != 0){
		QString strPage = QString("%1").arg(page);
		url.addQueryItem("page", strPage);
	}

	QNetworkRequest req(url);
	QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
	req.setRawHeader("Authorization", oauthHeader);
	QNetworkReply *reply = networkAccessManager()->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(reply()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitFriends::reply()
{
	QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
	if(netReply){
		XmlReaderExtUsers xrextusers;
		xrextusers.readUsers(netReply);
		m_friends = xrextusers.users();
	}

	emit finished();
}


void QTwitFriends::error()
{
	qDebug() << "QTwitFriends error";
}
