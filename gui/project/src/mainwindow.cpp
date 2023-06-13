#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <string>

#include "../include/guicontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //цвет фона
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    ui->lineEdit->setReadOnly(true);

    btn_predict = ui->btnPredict;
    btn_user_set = ui->btnUserSet;

    custom_plot = ui->widget;
    ui->widget->xAxis->setRange(0,10);
    ui->widget->yAxis->setRange(0, 10);

    ui->btnRad1->setChecked(true);
    if (ui->btnRad1->isChecked()) {
        predict_parm = 8;
    }
    // Инициализируем вертикальную линию
    verticalLine = new QCPCurve(custom_plot->xAxis, custom_plot->yAxis);

    // Подключаем сигналы событий мыши от полотна графика к слотам для их обработки
    connect(custom_plot, &QCustomPlot::mousePress, this, &MainWindow::slotMousePress);
    connect(custom_plot, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);

    custom_plot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    custom_plot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    custom_plot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    custom_plot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем
    // удаление/приближение только по горизонтальной оси

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    custom_plot->xAxis->setTicker(dateTimeTicker);
    dateTimeTicker->setDateTimeFormat("hh dd. MMM\nyyyy");
    // Настраиваем шрифт по осям координат
    custom_plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    custom_plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    tracer = new QCPItemTracer(custom_plot);
    tracer->setPen(QPen(Qt::white));

    QColor axis_color(146,131,155);
    custom_plot->setBackground(QColor(38,35,50));
    custom_plot->yAxis->setLabelColor(axis_color);
    custom_plot->xAxis->setTickLabelColor(axis_color);
    custom_plot->xAxis->setBasePen(QPen(axis_color));
    custom_plot->xAxis->setLabelColor(axis_color);
    custom_plot->xAxis->setTickPen(QPen(axis_color));
    custom_plot->xAxis->setSubTickPen(QPen(axis_color));


    custom_plot->yAxis->setLabelColor(axis_color);
    custom_plot->yAxis->setTickLabelColor(axis_color);
    custom_plot->yAxis->setBasePen(QPen(axis_color));
    custom_plot->yAxis->setLabelColor(axis_color);
    custom_plot->yAxis->setTickPen(QPen(axis_color));
    custom_plot->yAxis->setSubTickPen(QPen(axis_color));

    ui->lineEdit->setStyleSheet("background-color:rgb(200,201,182)");
    ui->lineEdit_2->setStyleSheet("background-color:rgb(200,201,182)");
    ui->lineEdit->setAlignment(Qt::AlignCenter);
    ui->lineEdit_2->setAlignment(Qt::AlignCenter);

    connect(this->getPredictBtn(),SIGNAL(clicked(bool)),this, SLOT
    (startPredict()));
    connect(this->getUserSetBtn(),SIGNAL(clicked(bool)),this, SLOT
    (openUserSet()));

    min_el = 0, max_el = 0, current_x = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete btn_predict;
    delete btn_user_set;
    delete error_mes_;
    delete error_message_;
    delete error_type_;
    for (int i = 0; i < actions.size(); ++i) {
        delete actions[i];
    }
    delete custom_plot;
    delete verticalLine;
    delete tracer;
}

void MainWindow::startPlot(const std::string& stock) {
    stock_name = stock;
    main_handler_ptr->stockSelectHandler(stock);
}

void MainWindow::startPredict() {
    if (ui->btnRad1->isChecked()) {
        predict_parm = 8;
    } else if (ui->btnRad2->isChecked()) {
        predict_parm = 12;
    } else if (ui->btnRad3->isChecked()) {
        predict_parm = 36;
    } else if (ui->btnRad4->isChecked()) {
        predict_parm = 60;
    }
    main_handler_ptr->predictHandler(stock_name, predict_parm);
}

void MainWindow::openUserSet() {
    main_handler_ptr->openUserSettings();
}

QPushButton* MainWindow::getPredictBtn() {
    return btn_predict;
}

QPushButton* MainWindow::getUserSetBtn() {
    return btn_user_set;
}

void MainWindow::drawPlot() {
    ui->widget->clearGraphs();
    ui->lineEdit_2->setText(stock_name.c_str());
    double now = QDateTime::currentDateTime().toTime_t();
    double cur = 0;
    QVector<double> x, x_new;

    int predict_x_start = x_dates.size() * 0.99;
    double max_element =*std::max_element(y.begin(), y.end());
    double max_element_new =*std::max_element(y_new.begin(), y_new.end());
    double min_element = *std::min_element(y.begin(), y.end());
    double min_element_new =*std::min_element(y_new.begin(), y_new.end());
    max_element_new = std::max(max_element, max_element_new);
    std::reverse(y.begin(), y.end());
    min_element_new = std::min(min_element, min_element_new);
    std::string str = x_dates[x_dates.size() - 1].substr(11, 2) +" " +
            x_dates[x_dates.size() - 1].substr(8, 2)
            + "-" + x_dates[x_dates.size() - 1].substr(5, 2) + "-" +
            x_dates[x_dates.size() - 1].substr(0, 4);
    QDateTime date = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");
    now = date.toTime_t();
    str = x_dates[0].substr(11, 2) +" " + x_dates[0].substr(8, 2)
            + "-" + x_dates[0].substr(5, 2) + "-" +
            x_dates[0].substr(0, 4);
    date = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");

    cur = date.toTime_t();
    current_x = cur;

    QVector<double> x_line(2) , y_line(2);
    x_line[0] = now;
    y_line[0] = min_element;
    y_line[1] = max_element;
    x_line[1] = now;

    min_el = min_element;
    max_el = max_element;
    verticalLine->setData(x_line, y_line); // Устанавливаем координаты
    verticalLine->setVisible(false);
    ui->widget->xAxis->setRange(now, cur + 4 * 3600);
    ui->widget->yAxis->setRange(min_element - 3, max_element + 2);

    for (int i = x_dates.size() - 1; i >= 0 ; i--) {

        std::string str = x_dates[i].substr(11, 2) +" " + x_dates[i].substr(8, 2) + "-"
                    + x_dates[i].substr(5, 2) + "-" + x_dates[i].substr(0, 4);
        QDateTime b = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");
        double curent = b.toTime_t();
        x.push_back(curent);
    }
    ui->widget->xAxis->setLabel("Дата и время");
    ui->widget->yAxis->setLabel("Цена, $");

    ui->widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QColor(179,68,108, 255));
    ui->widget->graph(0)->addData(x, y);
    ui->widget->graph(0)->setBrush(QBrush(QColor(179,68,108, 128)));


    tracer->setGraph(custom_plot->graph(0));   // Трассировщик будет работать с графиком
    ui->widget->replot();

    if (y_new.size() != 0) {

        str = x_dates[x_dates.size() - 1 - predict_x_start].substr(11, 2) +" "
                + x_dates[x_dates.size() - 1 - predict_x_start].substr(8, 2)
                + "-" + x_dates[x_dates.size() - 1 - predict_x_start].substr(5, 2)
                + "-" + x_dates[x_dates.size() - 1 - predict_x_start].substr(0, 4);
        date = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");

        double now_new = date.toTime_t();
        str = x_dates[0].substr(11, 2) +" " + x_dates[0].substr(8, 2)
                + "-" + x_dates[0].substr(5, 2) + "-" + x_dates[0].substr(0, 4);
        date = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");

        double start = date.toTime_t();

        str = x_new_dates[x_new_dates.size() - 1].substr(11, 2) +" " + x_new_dates[x_new_dates.size() - 1].substr(8, 2)
                + "-" + x_new_dates[x_new_dates.size() - 1].substr(5, 2) + "-" + x_new_dates[x_new_dates.size() - 1].substr(0, 4);
        date = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");
        double cur_new = date.toTime_t();

        ui->widget->xAxis->setRange(now_new, cur_new + 3600 * 2);
        ui->widget->yAxis->setRange(min_element_new - 3, max_element_new + 2);
        x_new.insert(x_new.begin(), start);
        y_new.insert(y_new.begin(), y[y.size()-1]);
        for (size_t i = 0; i < x_new_dates.size(); ++i) {
            std::string str = x_new_dates[i].substr(11, 2) +" " + x_new_dates[i].substr(8, 2) + "-"
                        + x_new_dates[i].substr(5, 2) + "-" + x_new_dates[i].substr(0, 4);

            QDateTime b = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");
            double curent = b.toTime_t();
            x_new.push_back(curent);
        }

        min_el = min_element_new;
        max_el = max_element_new;

        ui->widget->addGraph();
        ui->widget->graph(1)->setPen(QPen(QColor(34, 139, 34, 255)));
        ui->widget->graph(1)->addData(x_new, y_new);
        ui->widget->graph(1)->setBrush(QColor(34, 139, 34, 70));
        ui->widget->replot();

        custom_plot->rescaleAxes();
        ui->widget->yAxis->setRange(min_element_new - 3, max_element_new+2);
        ui->widget->xAxis->setRange(now_new, cur_new + 3600 * 2);
        custom_plot->replot();
    }
    y_new.clear();
    y_new.squeeze();
    y.clear();
    y.squeeze();
}

void MainWindow::slotMousePress(QMouseEvent *event)
{
    // Определяем координату x на графике, где был произведён клик мышью
    double coord_x = custom_plot->xAxis->pixelToCoord(event->pos().x());
    if (coord_x > current_x) {
        tracer->setGraph(custom_plot->graph(1));
    } else {
        tracer->setGraph(custom_plot->graph(0));
    }

    // Подготавливаем координаты по оси X для переноса вертикальной линии
    QVector<double> x_line(2), y_line(2);
    x_line[0] = coord_x;
    x_line[1] = coord_x;
    y_line[0] = min_el;
    y_line[1] = max_el;

    // Устанавливаем новые координаты
    verticalLine->setData(x_line, y_line);

    // По координате X клика мыши определим ближайшие координаты для трассировщика
    tracer->setGraphKey(coord_x);

    // Выводим координаты точки графика, где установился трассировщик, в lineEdit
    ui->lineEdit->setText("Цена: " + QString::number(tracer->position->value()) + " $");
    custom_plot->replot(); // Перерисовываем содержимое полотна графика
}

void MainWindow::slotMouseMove(QMouseEvent *event)
{
    /* Если при передвижении мыши, ей кнопки нажаты,
     * то вызываем отработку координат мыши
     * через слот клика
     * */
    if(QApplication::mouseButtons()) slotMousePress(event);
}

void MainWindow::setMainWindowHandler(ptr_to_main_handler handler_main_ptr) {
main_handler_ptr = handler_main_ptr;
}

void MainWindow::createPlot(const std::vector<double>& y_plot_data,
                            const std::vector<double>& y_predict_data,
                            const std::vector<std::string>& x_plot,
                            const std::vector<std::string>& x_predict) {
    y = QVector<double>(y_plot_data.begin(), y_plot_data.end());
    y_new = QVector<double>(y_predict_data.begin(), y_predict_data.end());
    x_dates = QVector<std::string>(x_plot.begin(), x_plot.end());
    x_new_dates = QVector<std::string>(x_predict.begin(), x_predict.end());
}

void MainWindow::showErrorMessage() {
    error_mes_ = new QErrorMessage(this);
    error_mes_->setWindowTitle("Error!");
    error_mes_->showMessage(*error_type_ + "! " + *error_message_);
}

void MainWindow::createErrorMessage(const Error& error_message) {
    error_type_ = new QString(error_message.type.c_str());
    error_message_ = new QString(error_message.message.c_str());
}

void MainWindow::startActions(const std::vector<std::string>& action_strs) {
    for (size_t i = 0; i < action_strs.size(); ++i) {
        QPushButton *button = new QPushButton(this); // Создаём кнопку
        button->setText(action_strs[i].c_str()); // Устанавливаем в неё текст

        button->setStyleSheet("QPushButton { background-color:  rgb(104,97,112);"
                              " color: rgb(255, 255, 255); } "
                              "QPushButton:pressed { background-color:  "
                              "rgb(70, 68, 81);}");
        actions.push_back(button);
        ui->verticalLayout->addWidget(actions[i]);

        connect(button, &QPushButton::clicked, [this, button](){
            startPlot(button->text().toStdString());
        });
    }
}

void MainWindow::getActionsData() {
    main_handler_ptr->getActionsDataHandler();
}



