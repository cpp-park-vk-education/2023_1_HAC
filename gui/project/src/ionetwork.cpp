#include "../include/ionetwork.h"
#include <string>
#include <fstream>

static const std::string PATH_TO_AUTH_PARMS_FILE = "../../project/auth_parm"
                                                   ".txt";

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
    QString url_text = url.c_str();
    QUrl q_url(url_text);
    QNetworkRequest requesti(q_url);
    requesti.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(action_name_.c_str()));
    requesti.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("POST"));
        QByteArray data;
        std::vector<std::string> body_args = tokenize(body, '\t');
        std::string user = body_args[0];
        for (size_t i = 0; i < body_args.size(); ++i) {
            data.append((body_args[i] + "/").c_str());

        }
        requesti.setHeader(QNetworkRequest::ContentLengthHeader, data.count());
        if (action_name_ == "DELETECOOKIE") {
            requesti.setRawHeader("cookie", cookie.c_str());
            auto reply = network_manager.post(requesti, data);
            connect(reply, &QNetworkReply::finished,
                    [this, reply, user, callback]() {
                onFinishedPostExit(reply, user, callback);
            });
        } else if (body.size() == 1 ||
        (action_name_ != "AUTHORIZATION" && action_name_ != "REGISTRATION")) {
            requesti.setRawHeader("cookie", cookie.c_str());
            auto reply = network_manager.post(requesti, data);
            connect(reply, &QNetworkReply::finished, [this, reply, user, callback]() {
                   onFinishedPost(reply, user, callback);
            });

        } else {
            requesti.setRawHeader("cookie", "");
            auto reply = network_manager.post(requesti, data);
            connect(reply, &QNetworkReply::finished,
                    [this, reply, user, callback]() {
                   onFirstFinishedPost(reply, user, callback);
            });
        }
}

void IONetwork::onFinishedPost(QNetworkReply* reply, std::string user,
                 std::function<void(const Error& error_state)> callback) {
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString status = status_code.toString();
    Error error_state;
    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        error_state.type = "0";
        error_state.message = user;
    } else if (status.toStdString() == "401") {
        error_state.type = "401";
        error_state.message = "Incorrect input!";
    } else if (status.toStdString() == "403") {
        std::ofstream of;
        cookie = "";
        of.open(PATH_TO_AUTH_PARMS_FILE);
        if (!of.is_open()) {
            error_state.type = "Something went wrong!";
            error_state.message = "Please contact developers!";
            qDebug() << "Not open auth_file. PROBLEM!!!";
        } else {
            of.close();
        }
        reply->deleteLater();
       return window_manager_ptr->openAuthorizationWindow();;
    } else {
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
    std::ofstream of;
    cookie = "";
    of.open(PATH_TO_AUTH_PARMS_FILE);
    if (!of.is_open()) {
        qDebug() << "Not open auth_file. PROBLEM!!!";
    } else {
        of.close();
    }
    reply->deleteLater();
    return window_manager_ptr->openAuthorizationWindow();
}

void IONetwork::onFirstFinishedPost(QNetworkReply* reply, std::string user,
                 std::function<void(const Error& error_state)> callback) {
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString status = status_code.toString();
    QByteArray header_line = "cookie";
    QByteArray headerValue = reply->rawHeader(header_line);

    QString qstr= QString(headerValue);
    cookie = qstr.toStdString();

    Error error_state;

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        error_state.type = "0";
        error_state.message = user;
        std::ofstream of;
        cookie = qstr.toStdString();
        of.open(PATH_TO_AUTH_PARMS_FILE);
        if (!of.is_open()) {
            error_state.type = "Something went wrong!";
            error_state.message = "Please contact developers!";
            qDebug() << "Not open auth_file. PROBLEM!!!";
        } else {
            of << user << '\t' << cookie;
            of.close();
        }
    } else if (status.toStdString() == "401") {
        error_state.type = "401";
        error_state.message = "Incorrect input!";
    } else if (status.toStdString() == "403") {
        cookie = "";
        std::ofstream of;
        of.open(PATH_TO_AUTH_PARMS_FILE);
        if (!of.is_open()) {
            error_state.type = "Something went wrong!";
            error_state.message = "Please contact developers!";
            qDebug() << "Not open auth_file. PROBLEM!!!";
        } else {
            of.close();
        }
        reply->deleteLater();
        return window_manager_ptr->openAuthorizationWindow();
    } else {
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }
    reply->deleteLater();
    callback(error_state);
}

void IONetwork::GetRequest(const std::string& url, std::istream& body,
                        std::function<void(std::istream& network_output,
                                const Error& error_state)> callback) {
    QString url_text = url.c_str();
    QUrl q_url(url_text);

    QUrlQuery query;
    std::vector<std::string> an = tokenize_s(body, '\n');
    if (an.size() == 1) {
        query.addQueryItem("0", "0");
    } else {
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
    QString status = status_code.toString();

    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8(reply->readAll());
        error_state.type = "0";
        error_state.message = "";
        answer = data.toStdString();
        res = parseGetData(answer);
    } else if (status.toStdString() == "403") {
        std::ofstream of;
        cookie = "";
        of.open(PATH_TO_AUTH_PARMS_FILE);
        if (!of.is_open()) {
            error_state.type = "Something went wrong!";
            error_state.message = "Please contact developers!";
            qDebug() << "Not open auth_file. PROBLEM!!!";;
        } else {
            of.close();
        }
        reply->deleteLater();
        return window_manager_ptr->openAuthorizationWindow();;
    } else {
        error_state.type = "Server Error!";
        error_state.message = "Server did not response.";
    }
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
    std::vector<std::string> tokenized_answer = tokenize(answer, ',');
    std::string concatenated_str;
    for (size_t i = 0; i < tokenized_answer.size(); ++i) {
        concatenated_str += tokenized_answer[i];
    }

    tokenized_answer = tokenize(concatenated_str, '\t');
    concatenated_str = "";
    for (size_t i = 0; i < tokenized_answer.size(); ++i) {
        concatenated_str += tokenized_answer[i];
    }

    tokenized_answer  = tokenize(concatenated_str, '\n');
    concatenated_str = "";
    for (size_t i = 1; i < tokenized_answer.size() - 1; ++i) {
        if (tokenized_answer[i] == "]" or tokenized_answer[i] == "[")
            continue;
        concatenated_str += tokenized_answer[i] + '\n';
    }
    return concatenated_str;
}
