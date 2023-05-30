#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

#include "../include/guicontroller.h"
#include <memory>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //background color
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(242, 125, 15));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    btn_predict = ui->btnPredict;
    btn_user_set = ui->btnUserSet;
    //widget_graph = ui->widgetGraph;
    customPlot = ui->widget;
    ui->widget->xAxis->setRange(0,10);
    ui->widget->yAxis->setRange(0, 10);

    ui->btnRad1->setChecked(true);
    if (ui->btnRad1->isChecked()) {
        predict_parm = 8;
    }

    // Инициализируем вертикальную линию
    verticalLine = new QCPCurve(customPlot->xAxis, customPlot->yAxis);

    // Подключаем сигналы событий мыши от полотна графика к слотам для их обработки
    connect(customPlot, &QCustomPlot::mousePress, this, &MainWindow::slotMousePress);
    connect(customPlot, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);


    customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    //customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Подпись координат по Оси X в качестве Даты и Времени
    //customPlot->xAxis->setDateTimeFormat("hh:mm");  // Устанавливаем формат даты и времени

    //QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    //customPlot->xAxis->setTicker(fixedTicker);

    //fixedTicker->setTickStep(QCPAxisTickerFixed::ltDateTime); // tick step shall be 1.0

    //fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    customPlot->xAxis->setTicker(dateTimeTicker);

    //customPlot->xAxis->setRange(QCPAxisTickerDateTime::dateTimeToKey(QDate(2013, 11, 16)), QCPAxisTickerDateTime::dateTimeToKey(QDate(2015, 5, 2)));
    dateTimeTicker->setDateTimeFormat("hh dd. MMM\nyyyy");

    // Настраиваем шрифт по осям координат
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    tracer = new QCPItemTracer(customPlot);

    // Автоматическое масштабирование тиков по Оси X
    //customPlot->xAxis->setAutoTickStep(true); //problem !!!!!!!!!!


    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);

    //customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y




     //mapper = new QSignalMapper(this);

    //connect(this->get_apple_btn(),SIGNAL(clicked(bool)),this, SLOT(start_apple_plot()));
    connect(this->get_predict_btn(),SIGNAL(clicked(bool)),this, SLOT(start_predict()));
    connect(this->get_user_set_btn(),SIGNAL(clicked(bool)),this, SLOT(open_user_set()));
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete btn_apple;
    delete btn_predict;
    delete btn_user_set;
    delete errorMes;
    delete error_message_;
    delete error_type_;
}

/*void MainWindow::start_apple_plot() {
    stock_name = "apple";
    std::cout <<"stock_name" <<std::endl;
    main_handler_ptr->stockSelectHandler("apple");
}*/

void MainWindow::start_plot(const std::string& stock) {
    std::cout << stock <<std::endl;
    stock_name = stock;
    main_handler_ptr->stockSelectHandler(stock);
}

void MainWindow::start_predict() {
    if (ui->btnRad1->isChecked()) {
        predict_parm = 8;
    } else if (ui->btnRad2->isChecked()) {
        predict_parm = 12;
    } else if (ui->btnRad3->isChecked()) {
        predict_parm = 36;
    } else if (ui->btnRad4->isChecked()) {
        predict_parm = 60;
    }
    std::cout <<"stock = " << stock_name <<std::endl;
    main_handler_ptr->predictHandler(stock_name, predict_parm);
}

void MainWindow::open_user_set() {
    std::cout <<"openning"<<std::endl;
    main_handler_ptr->openUserSettings();
}

/*QPushButton* MainWindow::get_apple_btn() {
    return btn_apple;
}*/

QPushButton* MainWindow::get_predict_btn() {
    return btn_predict;
}

QPushButton* MainWindow::get_user_set_btn() {
    return btn_user_set;
}

void MainWindow::drawPlot() {
    ui->widget->clearGraphs();
    /*QVector<double> x, x_new;
    double max_element =*std::max_element(y.begin(), y.end());
    double max_element_new =*std::max_element(y_new.begin(), y_new.end());
    double min_element = *std::min_element(y.begin(), y.end());
    double min_element_new =*std::min_element(y_new.begin(), y_new.end());
    max_element_new = std::max(max_element, max_element_new);
    min_element_new = std::min(min_element, min_element_new);
    ui->widget->xAxis->setRange(0, y.size()+2);
    ui->widget->yAxis->setRange(min_element - 3, max_element+2);
    for (size_t i = 0; i < y.size(); ++i) {
        x.push_back(i);
    }

    ui->widget->xAxis->setLabel("lag");
    ui->widget->yAxis->setLabel("price, $");

    ui->widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::red));
    ui->widget->graph(0)->addData(x, y);
    ui->widget->graph(0)->setBrush(QBrush(QColor(235, 19, 19, 70)));
    ui->widget->replot();


    if (y_new.size() != 0) {
        ui->widget->xAxis->setRange(0, y.size() +y_new.size()+2);
        ui->widget->yAxis->setRange(min_element_new - 3, max_element_new+2);
        //ui->widget->graph(0)->setPen(QPen(Qt::green));
        //ui->widget->graph(0)->setBrush(QBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256, 70)));
        x_new.push_back(y.size()-1);
        y_new.push_front(y[y.size()-1]);
        std::cout <<"**"<< y_new.size() <<"**"<< std::endl;
        for (size_t i = 1; i < y_new.size(); ++i) {
            //std::cout <<
            x_new.push_back(y.size() + i);
            std::cout << x_new[i] << ' ';
        }

        for (size_t i = 0; i < y_new.size(); ++i) {
            //std::cout <<
            std::cout << y_new[i] << ' ';
        }

        //ui->widget->graph(0)->setPen(QPen(Qt::green));

        //ui->widget->addGraph(); // red line
        //ui->widget->graph(1)->setPen(QPen((Qt::green)));

        //ui->widget->update();
        ui->widget->addGraph();
        ui->widget->graph(1)->setPen(QPen(Qt::green));
        ui->widget->graph(1)->addData(x_new, y_new);
        ui->widget->graph(1)->setBrush(QBrush(QColor(162, 242, 136, 70)));
        ui->widget->replot();



        */
        // Инициализируем график и привязываем его к Осям
        //graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
        //customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
        //graphic->setName("Доход, Р");       // Устанавливаем
        //graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
        //graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
        //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков


        /*if (tracer) {
            tracer = nullptr;
        }*/


        y_new.push_back(180);
        y_new.push_back(170);
        y_new.push_back(175);
        y_new.push_back(177);
        y_new.push_back(179);

        double now = QDateTime::currentDateTime().toTime_t();
        double cur = 0;
        QVector<double> x, x_new;
        QVector<double> xx, xx_new;

        int predict_x_start = y.size() * 0.7;

        double max_element =*std::max_element(y.begin(), y.end());
        double max_element_new =*std::max_element(y_new.begin(), y_new.end());
        double min_element = *std::min_element(y.begin(), y.end());
        double min_element_new =*std::min_element(y_new.begin(), y_new.end());
        max_element_new = std::max(max_element, max_element_new);



        std::reverse(y.begin(), y.end());
        /*if (y_new.size() != 0) {
            min_element = std::min(min_element, min_element_new);
        }*/
        min_element_new = std::min(min_element, min_element_new);
        std::cout <<"min element"  << min_element_new <<std::endl;


        std::string strin = x_dates[x_dates.size() - 1].substr(11, 2) +" " + x_dates[x_dates.size() - 1].substr(8, 2)
                + "-" + x_dates[x_dates.size() - 1].substr(5, 2) + "-" + x_dates[x_dates.size() - 1].substr(0, 4);
        QDateTime b = QDateTime::fromString(strin.c_str(),"hh dd-MM-yyyy");

        //std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
        now = b.toTime_t();

        strin = x_dates[0].substr(11, 2) +" " + x_dates[0].substr(8, 2)
                + "-" + x_dates[0].substr(5, 2) + "-" + x_dates[0].substr(0, 4);
        b = QDateTime::fromString(strin.c_str(),"hh dd-MM-yyyy");

        //std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
        cur = b.toTime_t();
        curenti = cur;

        QVector<double> x_line(2) , y_line(2);
        x_line[0] = now;
        y_line[0] = min_element;
        y_line[1] = max_element;
        x_line[1] = now;

        mi = min_element;
        ma = max_element;

    //customPlot->addPlottable(verticalLine);   // Добавляем линию на полотно
    verticalLine->setName("Vertical");      // Устанавливаем ей наименование
    verticalLine->setData(x_line, y_line);            // И устанавливаем координаты
    verticalLine->setVisible(false);

        ui->widget->xAxis->setRange(now, cur + 4 * 3600);

        int mini = min_element - 3;
        ui->widget->yAxis->setRange(mini, max_element+2);
        //ui->widget->yAxis->setRange(min_element - 3, max_element+2);
        for (size_t i = 0; i < y.size(); ++i) {
            x.push_back(i);
            //xx.push_back(now + 3600 * i);
            //QDateTime b = QDateTime::fromString(x_dates[i].c_str(),"hh dd. MMM\nyyyy");
            //std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
            //double curent = b.toTime_t();
            //xx.push_back(curent);

           // xx.push_back()
        }

        /*for (size_t i = 0; i < x_dates.size(); ++i) {
            //x.push_back(i);
            //xx.push_back(now + 3600 * i);
            //std::string str = x_dates[i].substr(0, 13);
            std::string str = x_dates[i].substr(11, 2) +" " + x_dates[i].substr(8, 2) + "-"
                    + x_dates[i].substr(5, 2) + "-" + x_dates[i].substr(0, 4);
            //std::cout <<x_dates[i]<<std::endl;
            //std::cout <<test<<std::endl;
            //std::string stri = "09 25-05-2023";
            QDateTime b = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");

            std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
            double curent = b.toTime_t();
            cur = curent;
            xx.push_back(curent);

           // xx.push_back()
        }*/

        for (int i = x_dates.size() - 1; i >= 0 ; i--) {
            //x.push_back(i);
            //xx.push_back(now + 3600 * i);
            //std::string str = x_dates[i].substr(0, 13);
            std::string str = x_dates[i].substr(11, 2) +" " + x_dates[i].substr(8, 2) + "-"
                    + x_dates[i].substr(5, 2) + "-" + x_dates[i].substr(0, 4);
            //std::cout <<x_dates[i]<<std::endl;
            //std::cout <<test<<std::endl;
            //std::string stri = "09 25-05-2023";
            QDateTime b = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");

            std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
            double curent = b.toTime_t();
            cur = curent;
            xx.push_back(curent);

           // xx.push_back()
        }


        ui->widget->xAxis->setLabel("lag");
        //ui->widget->yAxis->setLabel("price, $");
        ui->widget->yAxis->setLabel("VAVASTO, Романы");

        ui->widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        ui->widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(Qt::red));
        ui->widget->graph(0)->addData(xx, y);
        ui->widget->graph(0)->setBrush(QBrush(QColor(235, 19, 19, 70)));

        //tracer = new QCPItemTracer(customPlot);
        tracer->setGraph(customPlot->graph(0));   // Трассировщик будет работать с графиком


        ui->widget->replot();





        if (y_new.size() != 0) {

            /*strin = x_new_dates[x_dates.size() - 1].substr(11, 2) +" " + x_new_dates[x_dates.size() - 1].substr(8, 2)
                            + "-" + x_new_dates[x_dates.size() - 1].substr(5, 2) + "-" + x_new_dates[x_dates.size() - 1].substr(0, 4);
            QDateTime b = QDateTime::fromString(strin.c_str(),"hh dd-MM-yyyy");

                    //std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
            double now_new = b.toTime_t();*/

            strin = x_dates[0].substr(11, 2) +" " + x_dates[0].substr(8, 2)
                            + "-" + x_dates[0].substr(5, 2) + "-" + x_dates[0].substr(0, 4);
            b = QDateTime::fromString(strin.c_str(),"hh dd-MM-yyyy");

            //std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
            cur = b.toTime_t();

            /*strin = x_new_dates[0].substr(11, 2) +" " + x_new_dates[0].substr(8, 2)
                            + "-" + x_new_dates[0].substr(5, 2) + "-" + x_new_dates[0].substr(0, 4);
            b = QDateTime::fromString(strin.c_str(),"hh dd-MM-yyyy");

            //std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
            cur = b.toTime_t();*/

            ui->widget->xAxis->setRange(now, now + 3600 *(cur + y_new.size() + 2));
            //ui->widget->xAxis->setRange(now, cur + 3600 * 2);
            mini = min_element_new - 3;
            std::cout<<"##" <<mini<<"##"<<std::endl;

            ui->widget->yAxis->setRange(mini, max_element_new+2);

            //ui->widget->yAxis->setRange(min_element_new - 3, max_element_new+2);
            //ui->widget->graph(0)->setPen(QPen(Qt::green));
            //ui->widget->graph(0)->setBrush(QBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256, 70)));
            x_new.push_front(cur);
            xx_new.push_front(cur);
            y_new.push_front(y[y.size()-1]);
            std::cout <<"**"<< y_new.size() <<"**"<< std::endl;
            for (size_t i = 1; i < y_new.size(); ++i) {
                //std::cout <<
                x_new.push_back(cur + (i) * 3600);
                std::cout << x_new[i] << ' ';
                std::string str = x_new_dates[i].substr(11, 2) +" " + x_new_dates[i].substr(8, 2) + "-"
                        + x_new_dates[i].substr(5, 2) + "-" + x_new_dates[i].substr(0, 4);
                //std::cout <<x_dates[i]<<std::endl;
                //std::cout <<test<<std::endl;
                //std::string stri = "09 25-05-2023";
                QDateTime b = QDateTime::fromString(str.c_str(),"hh dd-MM-yyyy");

                std::cout<<"!" << b.toString("hh dd. MMM\nyyyy").toStdString() <<"!" <<std::endl;
                double curent = b.toTime_t();
                //cur = curent;
                xx_new.push_back(curent);
            }

            for (size_t i = 0; i < y_new.size(); ++i) {
                //std::cout <<
                std::cout << y_new[i] << ' ';
            }

            //ui->widget->graph(0)->setPen(QPen(Qt::green));

            //ui->widget->addGraph(); // red line
            //ui->widget->graph(1)->setPen(QPen((Qt::green)));

            //ui->widget->update();
            ui->widget->addGraph();
            ui->widget->graph(1)->setPen(QPen(Qt::green));
            ui->widget->graph(1)->addData(x_new, y_new);
            //ui->widget->graph(1)->addData(xx_new, y_new);
            ui->widget->graph(1)->setBrush(QBrush(QColor(162, 242, 136, 70)));
            ui->widget->replot();

        //graphic->setData(time, income); // Устанавливаем данные
        customPlot->rescaleAxes();      // Масштабируем график по данным
        ui->widget->yAxis->setRange(mini, max_element_new+2);
        customPlot->replot();           // Отрисовываем график






    }

    /*ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
    ui->widget->graph(0)->setPen(QPen(Qt::red));*/

    /*QLinearGradient lGrad(QPointF(0,0), QPointF(x_new[0],y_new[0]));
    lGrad.setColorAt(0, QColor(Qt::red));
    lGrad.setColorAt(1, QColor(Qt::green));
    QBrush lBrush(lGrad);
    QPen myPen;
    myPen.setBrush(lBrush);

    ui->widget->graph(0)->setPen(myPen);*/
    //ui->widget->graph(0)->setPen(QPen(Qt::red));


    //ui->widget->graph(0)->setPen(QPen(Qt::green));

    y_new.clear();
    y_new.squeeze();
    y.clear();
    y.squeeze();
    std::cout << "Graphic is drawn" << std::endl;
    /*if (tracer) {
        delete tracer;
    }*/
}

void MainWindow::slotMousePress(QMouseEvent *event)
{
    // Определяем координату X на графике, где был произведён клик мышью
    double coordX = customPlot->xAxis->pixelToCoord(event->pos().x());
    if (coordX > curenti) {
        tracer->setGraph(customPlot->graph(1));
    } else {
        tracer->setGraph(customPlot->graph(0));
    }

    // Подготавливаем координаты по оси X для переноса вертикальной линии
    QVector<double> x_line(2), y_line(2);
    x_line[0] = coordX;
    //y[0] = -50;
    x_line[1] = coordX;
    //y[1] = 50;
    y_line[0] = mi;
    //x_line[1] = max_element;
    y_line[1] = ma;


    // Устанавливаем новые координаты
    verticalLine->setData(x_line, y_line);

    // По координате X клика мыши определим ближайшие координаты для трассировщика
    tracer->setGraphKey(coordX);

    // Выводим координаты точки графика, где установился трассировщик, в lineEdit
    ui->lineEdit->setText("Цена: " + QString::number(tracer->position->value()) + " $");
    customPlot->replot(); // Перерисовываем содержимое полотна графика
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
    errorMes = new QErrorMessage(this);
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
}

void MainWindow::createErrorMessage(const Error& error_message) {
    error_type_ = new QString(error_message.type.c_str());
    error_message_ = new QString(error_message.message.c_str());
    std::cout << "created error message"<<std::endl;
}

void MainWindow::start_actions(const std::vector<std::string>& action_strs) {
    for (size_t i = 0; i < action_strs.size(); ++i) {
        QPushButton *button = new QPushButton(this);            // Создаём кнопку
        button->setText(action_strs[i].c_str());  // Устанавливаем в неё текст
        //counter++;                                              // Инкрементируем счётчик

        std::cout <<"(" <<i<<")"<<std::endl;

        actions.push_back(button);
        ui->verticalLayout->addWidget(actions[i]);
        //ui->verticalLayout->addWidget(button);                  // Помещаем кнопку в vertical layout

        // подключаем сигнал клика кнопки к мапперу



        connect(button, &QPushButton::clicked, [this, button](){
            //ui->label->setText(button->text());
            std::cout <<"@" << button->text().toStdString() <<std::endl;
            start_plot(button->text().toStdString());
        });

        //mapper->removeMappings(actions[i]);
    }
    //start_apple_plot();
    /*Error error;
    error.message = "09";
    error.type = "aaa";
    createErrorMessage(error);
    showErrorMessage();*/
}

void MainWindow::get_actions_data() {
    main_handler_ptr->getActionsDataHandler();
}



