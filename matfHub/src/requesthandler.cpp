#include "../include/requesthandler.h"

#include "../include/exceptions.h"

#include <QDebug>
#include <QEventLoop>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <iostream>
#include <memory>
#include <utility>

RequestHandler::RequestHandler() {}

ReplyAndLoop::~ReplyAndLoop()
{
  delete loop;
  delete reply;
}

std::string RequestHandler::getHtml(const std::string &url) noexcept(false)
{
  MaybeHtml html = getMaybeHtml(url);
  if (html.has_value()) {
    return html.value();
  }
  throw ScheduleNetworkException();
}

MaybeHtml RequestHandler::getMaybeHtml(const std::string &url)
{
  return getMaybeHtml(QString::fromStdString(url));
}

MaybeHtml RequestHandler::getMaybeHtml(const QString &url)
{
  return getMaybeHtml(QUrl(url));
}

MaybeHtml RequestHandler::getMaybeHtml(const QUrl &url)
{
  return RequestHandler::getMaybeHtmlSync(url);
}

MaybeHtml RequestHandler::getMaybeHtmlSync(const QUrl &url)
{
  ReplyAndLoop awaitableReply = getReplyAsync(url);
  QNetworkReply *reply = awaitableReply.reply;
  QEventLoop *loop = awaitableReply.loop;
  loop->exec(); // Blokira thread dok se ne zavrsio I/O
  if (reply->error() != QNetworkReply::NoError) {
    return std::nullopt;
  }
  return reply->readAll().toStdString();
}

ReplyAndLoop RequestHandler::getReplyAsync(const QUrl &url)
{
  QNetworkAccessManager *manager = new QNetworkAccessManager;
  QNetworkRequest request(url);
  QEventLoop *loop = new QEventLoop;
  QObject::connect(manager, &QNetworkAccessManager::finished, loop,
                   &QEventLoop::quit);

  QNetworkReply *reply = manager->get(request);
  return {reply, loop};
}
