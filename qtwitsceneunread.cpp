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

/*
 * This work is licensed under the Creative Commons Attribution-Noncommercial-Share 
 * Alike 3.0 Unported License. To view a copy of this license, 
 * visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to 
 * Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 * Graphic design by Goce Mitevski 
 */

#include "qtwitsceneunread.h"
#include "qtwit/qtwitstatus.h"

QTwitSceneUnread::QTwitSceneUnread(QObject * parent)
:   QTwitScene(parent)
{
}

void QTwitSceneUnread::nextStatuses()
{
    //doesn't do anything
}


void QTwitSceneUnread::updateStatuses()
{
    QSqlQuery query;
    //bigger but reasonable limit
    QString qr = QString("SELECT id, text, favorited, userId, screenName, profileImageUrl "
                         "FROM status "
                         "WHERE id > %1 AND %2 "
                         "ORDER BY id DESC "
                         "LIMIT 200").arg(lastStatusId()).arg(additionalQuery());
    query.exec(qr);

    QList<QTwitStatus> statuses;

    while (query.next()) {
        QTwitStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setFavorited(query.value(2).toBool());
        st.setUserId(query.value(3).toInt());
        st.setScreenName(query.value(4).toString());
        st.setProfileImageUrl(query.value(5).toString());
        statuses << st;
    }

    if (statuses.count()) {
        qint64 firstStatus = addStatuses(statuses, false);
        setFirstStatusId(firstStatus);

        query.first();

        qint64 lastStatus = query.value(0).toLongLong();
        setLastStatusId(lastStatus);
    }
}