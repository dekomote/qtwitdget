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

#ifndef TWEETLISTMODELUNREAD_H
#define TWEETLISTMODELUNREAD_H

#include <QtSql>
#include "qtwit/qtwitstatus.h"
#include "tweetlistmodel.h"

/*!
    Specialization of TweetListModel for handling list of unread tweets.
    List contains only unread tweets.
 */
class TweetListModelUnread : public TweetListModel
{
    Q_OBJECT
public:
    TweetListModelUnread(QObject *parent = 0);
    void update();
    void nextPage();
    qint64 nextUnread() const;
    bool markRead(qint64 id);
    void markAllRead();
};

#endif // TWEETLISTMODELUNREAD_H
