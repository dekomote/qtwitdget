/* Copyright (c) 2010, Antonie Jovanoski
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

#ifndef QTWITNEWDIRECTMESSAGE_H
#define QTWITNEWDIRECTMESSAGE_H

#include "qtwitbase.h"

class QTwitNewDirectMessage : public QTwitBase
{
    Q_OBJECT
public:
    QTwitNewDirectMessage(QObject *parent = 0);
    QTwitNewDirectMessage(QNetworkAccessManager* netManager, OAuthTwitter* oauthTwitter, QObject *parent = 0);
    void sendMessage(qint64 userId, const QString& text);
    void sendMessage(const QString& screenName, const QString& text);

signals:


private slots:
    void reply();
    void error();
};

#endif // QTWITNEWDIRECTMESSAGE_H
