#ifndef TEST_VK_PROJECT_IONETWORK_H
#define TEST_VK_PROJECT_IONETWORK_H

#include <QtNetwork/QNetworkAccessManager>
#include <QUrlQuery>

#include <string>
#include <iostream>
#include <sstream>
#include <QNetworkReply>
#include <QUrlQuery>
//#include <functional>

#include "../include/ionetwork_interface.h"

static std::vector<std::string> tokenize(std::string const &str,
                                  const char delim) {
    std::vector<std::string> out;
    std::stringstream ss(str); //строка как поток
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
    return out;
}

class IONetwork : public IONetworkInterface, QObject {
public:
    void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(const Error& error_state)>
                                     callback) override {
        std::cout << "In Network post request" <<std::endl;
        std::cout << url <<std::endl;
        std::cout << body <<std::endl;
        QString url_text = url.c_str();
        QUrl q_url(url_text);
        qDebug() <<q_url <<'\n';
        //QUrlQuery query;
        QByteArray data;
        std::vector<std::string> body_args = tokenize(body, '\t');
        //QByteArray data;
        std::string user = body_args[0];
        for (size_t i = 0; i < body_args.size(); ++i) {
            if (i % 2 == 0) {
                data.append((body_args[i] + "/").c_str());
            } else {
                data.append((body_args[i] + "//").c_str());
            }
            std::cout <<"&"<< body_args[i] <<"&"<<std::endl;
        }
        /*data.append("username/");
        data.append("test_user//");
        data.append("password/");
        data.append("test_pass");*/

        QNetworkRequest requesti(q_url);
        std::cout <<"#" <<0 <<"#"<<std::endl;
        requesti.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
        std::cout <<"#" <<1 <<"#"<<std::endl;
        requesti.setHeader(QNetworkRequest::ContentLengthHeader, data.count());
        std::cout <<"#" <<1.5 <<"#"<<std::endl;
        //network_manager.post(requesti, data);
        auto reply = network_manager.post(requesti, data);
        std::cout <<"#" <<2 <<"#"<<std::endl;
        /* Устанавливаем callback который вызовется после получения ответа */
        connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
               onFinished(reply, user, callback);
           });
    }

    void onFinished(QNetworkReply* reply, std::string user,
                     std::function<void(const Error& error_state)> callback) {
        Error error_state;
        if ( reply->error() == QNetworkReply::NoError ) {
            QString data = QString::fromUtf8( reply->readAll() );
            std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
            if (data.toStdString() != "error") {//might me other phrase
                error_state.type = "0";
                error_state.message = user;
            } else {
                error_state.type = data.toStdString();
                error_state.message = data.toStdString();
            }

        } else {
            //ui->lbStatus->setText( reply->errorString() );
            std::cout <<"@" <<"server error" <<"@"<<std::endl;
            //SEND error with parameters server error
            error_state.type = "Server Error!";
            error_state.message = "Server did not response.";
        }

        // Мы сами должны освободить память для reply
        // Однако делать это через delete нельзя
        reply->deleteLater();
        callback(error_state);
    }
    void GetRequest(const std::string& url, const MainData& body,
                            std::function<void(std::istream& network_output,
                                    const Error& error_state)> callback)
                                    override {
        std::cout << "In Network get request" <<std::endl;
        std::cout << url <<std::endl;
        std::cout << body.stock_name <<std::endl;
        QString url_text = url.c_str();
        QUrl q_url(url_text);
        qDebug() <<q_url <<'\n';

        QUrlQuery query;
        query.addQueryItem("name", body.stock_name.c_str());
        query.addQueryItem("graph", body.operation_title.c_str());
        query.addQueryItem("lag", QString::number(body.lag));
        query.addQueryItem("window_size", QString::number(body.window_size));

        q_url.setQuery(query.query());
        QNetworkRequest request( q_url);
        //requesti.setRawHeader("ngrok-skip-browser-warning", "69420");
        //m_manager.get( QNetworkRequest( QUrl( urlText ) ) );
        //std::string status = body.operation_title;
        if (body.operation_title == "plot") {
        //request.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("TIMESERIES_REQUEST"));
            request.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("PLOT"));
        } else {
            request.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("PREDICT"));
        }
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("GET"));
        auto reply = network_manager.get(request);
        connect(reply, &QNetworkReply::finished, [this, reply, callback]() {
               onFinishedGet(reply, callback);
           });
    }

    void onFinishedGet(QNetworkReply* reply,
                       std::function<void(std::istream& network_output,
                               const Error& error_state)> callback) {
        Error error_state;
        std::string answer;
        if ( reply->error() == QNetworkReply::NoError ) {
            QString data = QString::fromUtf8( reply->readAll() );
            std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
            if (data.toStdString() != "error") {//might me other phrase
                error_state.type = "0";
                error_state.message = "";
                //answer = status + "\n" + data.toStdString();
                answer = data.toStdString();
                std::cout <<"^" <<answer <<"^"<<std::endl;
                //std::istringstream iss(data.toStdString());
                //std::istream& server_answer(iss);

                //std::istringstream iss(data.toStdString());
                //std::istream& input(iss);

            } else {
                error_state.type = data.toStdString();
                error_state.message = data.toStdString();
            }

        } else {
            //ui->lbStatus->setText( reply->errorString() );
            std::cout <<"@" <<"server error" <<"@"<<std::endl;
            //SEND error with parameters server error
            error_state.type = "Server Error!";
            error_state.message = "Server did not response.";
        }

        std::istringstream iss(answer);
        //std::istream& server_answer(iss);

        // Мы сами должны освободить память для reply
        // Однако делать это через delete нельзя
        reply->deleteLater();
        //std::stringstream out(answer);
        callback(iss, error_state);
    }

    void setConfig(const std::string& host) override {}
private:
    QNetworkAccessManager network_manager;
    std::string host_;
};

#endif //TEST_VK_PROJECT_IONETWORK_H
