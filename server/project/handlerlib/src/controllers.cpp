#include "controllers.h"

namespace controllers {

// class PredictController
PredictController::PredictController
    (const ptrToDBController db_controller, const ptrToModelController model_controller) :
            db_controller_(db_controller), model_controller_(model_controller) {}

Json::Value PredictController::makePredict(const Json::Value& request) {
    // получает джейсон 
    // получает из него название акций 
    // отправляет название ДБ контроллеру

    // проверка ответа 
        // сформировать джейсон с ошибкой
        // выйти из функции

    // создаём таймсериес
    // auto time_series = makeTimeSeries(json_plot_data, window_size);
    // ловим исключение
        // сформировать джейсон с ошибкой
        // выйти из функции

    // передаём управление
    // получаем предикт в джейсоне

    // проверяем предикт и добавляем дату 
        // сформировать джейсон с ошибкой
        // выйти из функции

    // отдаём джейсон
    // выходим из функции

}

TimeSeriesPredicts PredictController::makeTimeSeries(const Json::Value& samples_data, size_t window_size) {
    // идём по джейсону
    // формируем кортежи с размером window_size
    // записываем их в вектор
    // возращаем таймсериес
}

// class ModelController
ModelController::ModelController(const ptrToAPIModel api_model) 
    : api_model_(api_model) {}

Json::Value ModelController::callModelApi(const TimeSeriesPredicts& samples_data) {
    // создаёт реквест для апи модели
    // отправляет реквест апи
    // принимает респонс
    // парсит респонс
    // формирует хороший или плохой джейсон
}

IHTTPRequest_ ModelController::makeHttpForModel(const TimeSeriesPredicts& samples_data) {
    // создание реквеста по тайм серии
}

Json::Value ModelController::parseModelResponse(const IHTTPResponse_ request) {
    // парсинг респоса от модели
}

// class ShowPlotController
ShowPlotController::ShowPlotController(const ptrToDBController db_controller) 
    : db_controller_(db_controller) {}

Json::Value ShowPlotController::createPlotData(const Json::Value& request) {
    // получает из джейсона название акции
    // запрашивает у ДБ контроллера 
    // проверяет ответ
        // отдаёт плохой джейсон
    // отдаёт хороший джейсон
}

// class RegisterController
RegisterController::RegisterController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value RegisterController::registration(const Json::Value& request) {
    // получает данные регистрации
    // проверяет
        // отдаёт отказ
    
    // хешируем пароль, солим пароль

    // формируем джейсон для ДБ контроллера
    // отправляем на запись
    // проверка
        // фалс
    // тру
}

hash_ RegisterController::hashPassword(const std::string& password) {
    // хеширование
}

// class AuthorizeController
AuthorizeController::AuthorizeController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value AuthorizeController::authorization(const Json::Value& request) {
    // получаем джейсон
    // хешируем пароль
    // отправляем бд
    // проверяем ответ
        // отдаём отказ
    // отдаём тру
} 

hash_ AuthorizeController::hashPassword(const std::string& password) {
    // хеширование
}

// class UpdateDataController
UpdateDataController::UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock)
    : db_controller_(db_controller), api_stock_(api_stock) {}

bool UpdateDataController::updateData(const handlers::ProtocolAPI& protocol) {
    // создаём из структуры стрингу
    // отдаём на вход апи стрингу

    // парсим хттп в json 
    // отдаём json ДБ контроллеру на запись

    // возращаем статус состояния

}

Json::Value UpdateDataController::parseHTTPToJson(IHTTPResponse_ response) {
    // из поданнной на вход структуры делаем стрингу по протоколу 

}

std::string UpdateDataController::parseToApiRequest(const handlers::ProtocolAPI& protocol) {
    // из поданнной на вход структуры делаем стрингу по протоколу 

}

} // namespace controllers 
