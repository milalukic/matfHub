#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <utility>
#include <memory>
#include <optional>
#include <string>
#include "exceptions.h"

using MaybeHtml = std::optional<std::string>;

struct ReplyAndLoop {
    QNetworkReply* reply;
    QEventLoop* loop;

    ~ReplyAndLoop();
};

class RequestHandler{
public:
    RequestHandler();
    std::string getHtml(const std::string& url) noexcept(false);
    MaybeHtml getMaybeHtml(const std::string& url);
    MaybeHtml getMaybeHtml(const QString& url);
    MaybeHtml getMaybeHtml(const QUrl& url);
    MaybeHtml getMaybeHtmlSync(const QUrl& url);
    ReplyAndLoop getReplyAsync(const QUrl& url);
};

#endif // REQUESTHANDLER_H
