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

#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QByteArray>
#include <QLibrary>
#include <QUrl>
#include <openssl/hmac.h>
#include <openssl/evp.h>

typedef unsigned char * (*FPHMAC)(const EVP_MD *, const void *, int,
		                        const unsigned char *, size_t, unsigned char *,
		                        unsigned int *);

typedef const EVP_MD * (*FPEVPSHA1)(void);

/*! \class OAuth
    \brief Base OAuth class
 */
class OAuth : public QObject
{
	Q_OBJECT

public:
	OAuth(QObject *parent = 0);
	
	enum HttpMethod {GET, POST, PUT, DELETE};

	void parseTokens(const QByteArray& response);
	QByteArray generateAuthorizationHeader(const QUrl& url, HttpMethod method);
	void setOAuthToken(const QByteArray& token);
	void setOAuthTokenSecret(const QByteArray& tokenSecret);
    void clearTokens();
	QByteArray oauthToken() const;
	QByteArray oauthTokenSecret() const;
	
private:
	QByteArray generateSignatureHMACSHA1(const QByteArray& signatureBase);
	QByteArray generateSignatureBase(const QUrl& url, HttpMethod method, const QByteArray& timestamp, const QByteArray& nonce);

	QByteArray m_oauthToken;
	QByteArray m_oauthTokenSecret;

    //dynamic linking and function pointers
    QLibrary m_libraryOpenssl;
    FPHMAC HMAC_fp;
    FPEVPSHA1 EVP_sha1_fp;
};

#endif //OAUTH_H
