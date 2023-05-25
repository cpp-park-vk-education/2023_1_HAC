#include "../include/ionetwork.h"
#include <string>
#include <fstream>

static std::vector<std::string> tokenize(std::string const &str,
                                         const char delim) {
    std::vector<std::string> out;
    std::stringstream ss(str);
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
    return out;
}

static std::vector<std::string> tokenize_s(std::istream& line_stream,
                                         const char delim) {
    std::vector<std::string> out;
    //std::stringstream ss(str);
    std::string s;
    while (std::getline(line_stream, s, delim)) {
        out.push_back(s);
    }
    return out;
}

void IONetwork::setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void IONetwork::PostRequest(const std::string& url, const std::string& body,
                         std::function<void(const Error& error_state)>
                                 callback) {
    std::cout << "In Network post request" <<std::endl;
    std::cout << url <<std::endl;
    std::cout << body <<std::endl;
    QString url_text = url.c_str();
    QUrl q_url(url_text);
    qDebug() <<q_url <<'\n';
    QNetworkRequest requesti(q_url);
    requesti.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(action_name_.c_str()));
    requesti.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("POST"));
    /*if (body.empty()) {
        requesti.setRawHeader("cookie", cookie.c_str());
        auto reply = network_manager.post(requesti, "");
        std::cout <<"#" <<2 <<"#"<<std::endl;


        connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
               onFinishedPost(reply, user, callback);
    } else {*/
        QByteArray data;
        std::vector<std::string> body_args = tokenize(body, '\t');
        std::cout <<"@@@@@"<<body_args.size()<<"@@@@" <<std::endl;
        std::string user = body_args[0];
        for (size_t i = 0; i < body_args.size(); ++i) {
            data.append((body_args[i] + "/").c_str());

            std::cout <<"&"<< body_args[i] <<"&"<<std::endl;
        }

        std::cout <<"user = " <<user <<"!!" <<std::endl;

        /*if (body.size() == 1 || action_name_ != "AUTHORIZATION" || action_name_ != "REGISTRATION") {
                requesti.setRawHeader("cookie", cookie.c_str());

        } else {
            requesti.setRawHeader("cookie", "");
        }*/
        //QNetworkRequest requesti(q_url);
        std::cout <<"#" <<0 <<"#"<<std::endl;
        //requesti.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(action_name_.c_str()));
        //requesti.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("POST"));
        //requesti.setRawHeader("cookie", "");
        std::cout <<"#" <<1 <<"#"<<std::endl;
        requesti.setHeader(QNetworkRequest::ContentLengthHeader, data.count());
        std::cout <<"#" <<1.5 <<"#"<<std::endl;
        //auto reply = network_manager.post(requesti, data);
        std::cout <<"#" <<2 <<"#"<<std::endl;
        /* Устанавливаем callback который вызовется после получения ответа */
        if (action_name_ == "DELETECOOKIE") {
            requesti.setRawHeader("cookie", cookie.c_str());
            auto reply = network_manager.post(requesti, data);
            connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
                   onFinishedPostExit(reply, user, callback);
            });
        } else if (body.size() == 1 || (action_name_ != "AUTHORIZATION" && action_name_ != "REGISTRATION")) {
            requesti.setRawHeader("cookie", cookie.c_str());
            auto reply = network_manager.post(requesti, data);
            connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
                   onFinishedPost(reply, user, callback);
            });

        } else {
            requesti.setRawHeader("cookie", "");
            auto reply = network_manager.post(requesti, data);
            connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
                   onFirstFinishedPost(reply, user, callback);
            });
        }

    //}
}

void IONetwork::onFinishedPost(QNetworkReply* reply, std::string user,
                 std::function<void(const Error& error_state)> callback) {
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString status = status_code.toString();
    std::cout<<"&&"<< status.toStdString() <<"&&"<<std::endl;
    //std::cout <<"user = " <<user <<"!!" <<std::endl;

    Error error_state;

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        error_state.type = "0";
        error_state.message = user;
    } else if (status.toStdString() == "401") {
        error_state.type = "401";
        error_state.message = "Incorrect input!";
    } else if (status.toStdString() == "403") {
        //have to clean cookie!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
       //window_manager_ptr->openAuthorizationWindow();
        std::ofstream of;
        cookie = "";
        of.open("../../project/auth_parm.txt");
        if (!of.is_open()) {
            std::cout <<"not open of_403" <<std::endl;
        } else {
            std::cout <<"open of_403" <<std::endl;
            of.close();
        }
        reply->deleteLater();
       return window_manager_ptr->openAuthorizationWindow();;
    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }

    reply->deleteLater();
    callback(error_state);
}

void IONetwork::onFinishedPostExit(QNetworkReply* reply, std::string user,
                 std::function<void(const Error& error_state)> callback) {
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString status = status_code.toString();
    std::cout<<"&&"<< status.toStdString() <<"&&"<<std::endl;
    std::ofstream of;
    cookie = "";
    of.open("../../project/auth_parm.txt");
    if (!of.is_open()) {
        std::cout <<"not open of_403" <<std::endl;
    } else {
        std::cout <<"open of_403" <<std::endl;
        of.close();
    }
    //std::cout <<"user = " <<user <<"!!" <<std::endl;
    reply->deleteLater();
    return window_manager_ptr->openAuthorizationWindow();
}

void IONetwork::onFirstFinishedPost(QNetworkReply* reply, std::string user,
                 std::function<void(const Error& error_state)> callback) {
    std::cout<< "first";
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString status = status_code.toString();
    std::cout<<"&&"<< status.toStdString() <<"&&";

    QByteArray header_line = "cookie";
    //QVariant cookie_var = reply->header(QNetworkRequest::rawHeader(header_line));
    QByteArray headerValue = reply->rawHeader(header_line);
    std::cout << "cookie values: ";
    QString qstr= QString(headerValue);
    /*for (int i = 0; i < headerValue.size(); ++i) {
    std::cout << headerValue[i] <<' ';
    }*/
    cookie = qstr.toStdString();
    std::cout <<"$$" << cookie <<"$$" <<std::endl;

    Error error_state;

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        error_state.type = "0";
        error_state.message = user;
        std::ofstream of;
        cookie = qstr.toStdString();
        of.open("../../project/auth_parm.txt");
        if (!of.is_open()) {
            std::cout <<"not open of" <<std::endl;
        } else {
            of << user << '\t' << cookie;
            //of << user << '\t' << "567";
            of.close();
        }
    } else if (status.toStdString() == "401") {
        error_state.type = "401";
        error_state.message = "Incorrect input!";
    } else if (status.toStdString() == "403") {
         //have to clean cookie!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        cookie = "";
        std::ofstream of;
        of.open("../../project/auth_parm.txt");
        if (!of.is_open()) {
            std::cout <<"not open of_403" <<std::endl;
        } else {
            std::cout <<"open of_403" <<std::endl;
            of.close();
        }
        //window_manager_ptr->openAuthorizationWindow();
        reply->deleteLater();
        return window_manager_ptr->openAuthorizationWindow();
    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }

    reply->deleteLater();
    callback(error_state);
}

void IONetwork::GetRequest(const std::string& url, std::istream& body,
                        std::function<void(std::istream& network_output,
                                const Error& error_state)> callback) {
    std::cout << "In Network get request" <<std::endl;
    std::cout << url <<std::endl;
    //std::cout << body.stock_name <<std::endl;
    QString url_text = url.c_str();
    QUrl q_url(url_text);
    qDebug() << q_url <<'\n';

    /*std::stringstream info;
    if (body.operation_title == "plot") {
        info << "PLOT\n";
        info <<body.operation_title << '\n';
        info <<"name\n" << body.stock_name << '\n';
        info << "lag\n" << body.lag <<'\n';
    } else {
        info << "PREDICT\n";
        info <<body.operation_title << '\n';
        info <<"name\n" << body.stock_name << '\n';
        info << "lenpredict\n" << body.window_size <<'\n';
    }

    std::string r = info.str();
    std::istringstream input(r);*/


    //std::stringstream main_body;
    QUrlQuery query;
    std::vector<std::string> an = tokenize_s(body, '\n');
    if (an.size() == 1) {
        query.addQueryItem("0", "0");
    } else if (an.size() % 2 == 1) {
        std::cout<<"ploho" <<std::endl;
    } else {
        //QUrlQuery query;
      //  query.addQueryItem("name", body.stock_name.c_str());
        // query.addQueryItem("graph", body.operation_title.c_str());
        // query.addQueryItem("lag", QString::number(body.lag));
     //   query.addQueryItem("window_size", QString::number(body.window_size));

       // q_url.setQuery(query.query());
       // QNetworkRequest request( q_url);
       //q_url.setQuery(query.query());
      // QNetworkRequest request( q_url);
        /*if (body.operation_title == "plot") {
            query.addQueryItem("name", body.stock_name.c_str());
            query.addQueryItem("lag", QString::number(body.lag));
            q_url.setQuery(query.query());
            QNetworkRequest request( q_url);

            request.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("PLOT"));
            request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("GET"));
            auto reply = network_manager.get(request);
            connect(reply, &QNetworkReply::finished, [this, reply, callback]() {
                    onFinishedGet(reply, callback);
                });
        } else {
            query.addQueryItem("name", body.stock_name.c_str());
            query.addQueryItem("lenpredict", QString::number(body.window_size));
            q_url.setQuery(query.query());
            QNetworkRequest request( q_url);

            request.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("PREDICT"));
            request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("GET"));
            auto reply = network_manager.get(request);
            connect(reply, &QNetworkReply::finished, [this, reply, callback]() {
                onFinishedGet(reply, callback);
           });
        }*/
        for (size_t i = 2; i < an.size(); i+=2) {
            query.addQueryItem(an[i].c_str(), an[i+1].c_str());
        }
    }
    q_url.setQuery(query.query());
    QNetworkRequest request( q_url);
    request.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(an[0].c_str()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("GET"));
    request.setRawHeader("cookie", cookie.c_str());
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
    QString status = status_code.toString(); // or status_code.toInt();
    std::cout<<"&&"<< status.toStdString() <<"&&"<<std::endl;

    //check lines
    std::cout <<"bad cookie"<<std::endl;
    //window_manager_ptr->openAuthorizationWindow();
    //return window_manager_ptr->openAuthorizationWindow();;

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        std::cout <<"@" <<data.toStdString() <<"@"<<std::endl;
        error_state.type = "0";
        error_state.message = "";
        answer = data.toStdString();
        res = parseGetData(answer);
    } else if (status.toStdString() == "403") {
        //window_manager_ptr->openAuthorizationWindow();
        //have to clean cookie!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::ofstream of;
        cookie = "";
        of.open("../../project/auth_parm.txt");
        if (!of.is_open()) {
            std::cout <<"not open of_403" <<std::endl;
        } else {
            std::cout <<"open of_403" <<std::endl;
            of.close();
        }
        reply->deleteLater();
        return window_manager_ptr->openAuthorizationWindow();;
    } else {
        std::cout <<"@" <<"server error" <<"@"<<std::endl;
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }
    std::cout <<res <<std::endl;
    std::istringstream iss(res);
    reply->deleteLater();
    callback(iss, error_state);
}

void IONetwork::setConfig(const std::string& action_name) {
    action_name_ = action_name;
}

void IONetwork::setCookie(const std::string& cookie_data) {
    cookie = cookie_data;
}
std::string IONetwork::parseGetData(const std::string answer) {
    std::vector<std::string> an = tokenize(answer, ',');
    std::string sos;
    for (size_t i = 0; i < an.size(); ++i) {
        sos+= an[i];
    }
    std::vector<std::string> bn = tokenize(sos, '\t');
    sos = "";
    for (size_t i = 0; i < bn.size(); ++i) {
        sos+= bn[i];
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
