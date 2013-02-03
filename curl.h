/**********************************************************************
This class has to be run inside an event loop because the QNetwork API
is asynchronous

***********************************************************************/
#ifndef CURL_H
#define CURL_H

#include <QDialog>
#include <QDomDocument>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>


class Curl : public QObject
{
    Q_OBJECT

public:
    explicit Curl(QWidget *parent = 0);

    void get(QUrl url);
		void post(QUrl url);
		void put(QUrl url);
		void del(QUrl url);

signals:
    void getResult(QString reply);
		void postResult(QString reply);
		void putResult(QString reply);
		void delResult(QString reply);

public slots:
    void getFinished(QNetworkReply* reply);
		void postFinished(QNetworkReply* reply);
		void putFinished(QNetworkReply* reply);
		void delFinished(QNetworkReply* reply);

protected:

private:

};

#endif // CURL_H
