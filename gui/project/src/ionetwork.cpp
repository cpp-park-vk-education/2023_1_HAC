#include "../include/ionetwork.h"
#include <string>

void IONetwork::PostRequest(const std::string& url, const std::string& body,
                         std::function<void(const Error& error_state)>
                                 callback) {
    std::cout << "In Network post request" <<std::endl;
    std::cout << url <<std::endl;
    std::cout << body <<std::endl;
    QString url_text = url.c_str();
    QUrl q_url(url_text);
    qDebug() <<q_url <<'\n';
    QByteArray data;
    std::vector<std::string> body_args = tokenize(body, '\t');
    std::string user = body_args[0];
    for (size_t i = 0; i < body_args.size(); ++i) {
        data.append((body_args[i] + "/").c_str());

        std::cout <<"&"<< body_args[i] <<"&"<<std::endl;
    }
    QNetworkRequest requesti(q_url);
    std::cout <<"#" <<0 <<"#"<<std::endl;
    requesti.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(action_name_.c_str()));
    requesti.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("POST"));
    std::cout <<"#" <<1 <<"#"<<std::endl;
    requesti.setHeader(QNetworkRequest::ContentLengthHeader, data.count());
    std::cout <<"#" <<1.5 <<"#"<<std::endl;
    auto reply = network_manager.post(requesti, data);
    std::cout <<"#" <<2 <<"#"<<std::endl;
    /* Устанавливаем callback который вызовется после получения ответа */
    connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
           onFinishedPost(reply, user, callback);
    });
}

void IONetwork::onFinishedPost(QNetworkReply* reply, std::string user,
                 std::function<void(const Error& error_state)> callback) {
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString status = status_code.toString(); // or status_code.toInt();
    std::cout<<"&&"<< status.toStdString() <<"&&";
    Error error_state;
    /*if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        if (data.toStdString() != "error") {//might me other phrase
            error_state.type = "0";
            error_state.message = user;
        } else {
            error_state.type = data.toStdString();
            error_state.message = data.toStdString();
        }

    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }*/

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        error_state.type = "0";
        error_state.message = user;
    } else if (status.toStdString() == "401") {
        error_state.type = "401";
        error_state.message = "Incorrect input!";
    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }

    // Мы сами должны освободить память для reply
    // Однако делать это через delete нельзя
    reply->deleteLater();
    callback(error_state);
}

void IONetwork::GetRequest(const std::string& url, const MainData& body,
                        std::function<void(std::istream& network_output,
                                const Error& error_state)> callback) {
    std::cout << "In Network get request" <<std::endl;
    std::cout << url <<std::endl;
    std::cout << body.stock_name <<std::endl;
    QString url_text = url.c_str();
    QUrl q_url(url_text);
    qDebug() << q_url <<'\n';

    QUrlQuery query;
    query.addQueryItem("name", body.stock_name.c_str());
    query.addQueryItem("graph", body.operation_title.c_str());
    query.addQueryItem("lag", QString::number(body.lag));
    query.addQueryItem("window_size", QString::number(body.window_size));

    q_url.setQuery(query.query());
    QNetworkRequest request( q_url);
    if (body.operation_title == "plot") {
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

void IONetwork::onFinishedGet(QNetworkReply* reply,
                   std::function<void(std::istream& network_output,
                           const Error& error_state)> callback) {
    Error error_state;
    std::string answer;
    std::string res;

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            // Print or catch the status code
    QString status = status_code.toString(); // or status_code.toInt();
    std::cout<<"&&"<< status.toStdString() <<"&&";

    //std::cout << reply->value
    /*if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        //if (data.toStdString() != "error") {//might me other phrase
        //std::vector<int>::iterator it;
        //it = find (error_values.begin(), error_values.end(), data.toStdString());
        if (std::find(error_values.begin(), error_values.end(), data.toStdString()) == error_values.end()) {
            error_state.type = "0";
            error_state.message = "";
            answer = data.toStdString();
            std::cout <<"^" <<answer <<"^"<<std::endl;

        } else {
            error_state.type = data.toStdString();
            error_state.message = data.toStdString();
        }

    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }*/

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        error_state.type = "0";
        error_state.message = "";
        answer = data.toStdString();
        res = parseGetData(answer);
    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }

    /*std::vector<std::string> an = tokenize(answer, ',');
    std::string sos;
    for (size_t i = 0; i < an.size(); ++i) {
        sos+= an[i];
        //std::cout <<"("<<an[i] <<")" <<std::endl;
    }
    std::vector<std::string> bn = tokenize(sos, '\t');
    sos = "";
    for (size_t i = 0; i < bn.size(); ++i) {
        sos+= bn[i];
        //std::cout <<"("<<bn[i] <<")" <<std::endl;
    }

    std::vector<std::string> cn = tokenize(sos, '\n');
    sos = "";
    for (size_t i = 1; i < cn.size() - 1; ++i) {
        sos+= cn[i] + '\n';
        std::cout <<"("<<cn[i] <<")" <<std::endl;
    }
    std::cout <<"?"<<sos <<"?" <<std::endl;*/
    //std::string res = parseGetData(answer);
    std::istringstream iss(res);
    reply->deleteLater();
    callback(iss, error_state);
}

void IONetwork::setConfig(const std::string& action_name) {
    action_name_ = action_name;
}
std::string IONetwork::parseGetData(const std::string answer) {
    std::vector<std::string> an = tokenize(answer, ',');
    std::string sos;
    for (size_t i = 0; i < an.size(); ++i) {
        sos+= an[i];
        //std::cout <<"("<<an[i] <<")" <<std::endl;
    }
    std::vector<std::string> bn = tokenize(sos, '\t');
    sos = "";
    for (size_t i = 0; i < bn.size(); ++i) {
        sos+= bn[i];
        //std::cout <<"("<<bn[i] <<")" <<std::endl;
    }

    std::vector<std::string> cn = tokenize(sos, '\n');
    sos = "";
    for (size_t i = 1; i < cn.size() - 1; ++i) {
        sos+= cn[i] + '\n';
        std::cout <<"("<<cn[i] <<")" <<std::endl;
    }
    std::cout <<"?"<<sos <<"?" <<std::endl;
    return sos;
}
