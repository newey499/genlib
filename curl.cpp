#include "curl.h"

Curl::Curl(QWidget *parent) :
    QObject(parent)
{
}


void Curl::get(QUrl url)
{
	QNetworkRequest request = QNetworkRequest(url);
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(getFinished(QNetworkReply*)));

	manager->get(request);
}


void Curl::getFinished(QNetworkReply* reply)
{
	QString res = reply->readAll();

	emit getResult(res);

	reply->deleteLater();
	reply = 0;
}


void Curl::post(QUrl url)
{
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(postFinished(QNetworkReply*)));

	QByteArray postArgs = url.encodedQuery();
	url.queryItems().clear();
	QNetworkRequest request(url);

	// Content-type: "text/xml"
	// "application/x-www-form-urlencoded"
	request.setHeader(QNetworkRequest::ContentTypeHeader,
										"application/x-www-form-urlencoded");

	manager->post(request, postArgs);
}


void Curl::postFinished(QNetworkReply* reply)
{
	QString res = reply->readAll();

	emit postResult(res);

	reply->deleteLater();
	reply = 0;
}


void Curl::put(QUrl url)
{
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(putFinished(QNetworkReply*)));

	QByteArray putArgs = url.encodedQuery();
	url.queryItems().clear();
	QNetworkRequest request(url);

	// Content-type: "text/xml"
	// "application/x-www-form-urlencoded"
	request.setHeader(QNetworkRequest::ContentTypeHeader,
										"application/x-www-form-urlencoded");

	manager->put(request, putArgs);
}


void Curl::putFinished(QNetworkReply* reply)
{
	QString res = reply->readAll();

	emit putResult(res);

	reply->deleteLater();
	reply = 0;
}


void Curl::del(QUrl url)
{
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(delFinished(QNetworkReply*)));

	QByteArray getArgs = url.encodedQuery();
	url.queryItems().clear();
	QNetworkRequest request(url);

	// Content-type: "text/xml"
	// "application/x-www-form-urlencoded"
	request.setHeader(QNetworkRequest::ContentTypeHeader,
										"application/x-www-form-urlencoded");

	manager->deleteResource(request);
}


void Curl::delFinished(QNetworkReply* reply)
{
	QString res = reply->readAll();

	emit delResult(res);

	reply->deleteLater();
	reply = 0;
}
