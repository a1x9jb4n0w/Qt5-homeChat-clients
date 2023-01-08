#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <QMainWindow>
#include <QTimer>
#include <QTextCursor>

class Timer
{
private:

    using clock_type = std::chrono::steady_clock;
    using second_type = std::chrono::duration< double, std::ratio<1,60> >;

    std::chrono::time_point<clock_type> m_beg;

public:

    Timer() : m_beg { clock_type::now() } { }

    void reset() { m_beg = clock_type::now(); }

    double elapsed() const
    { return std::chrono::duration_cast<second_type>( clock_type::now() - m_beg ).count(); }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace asio; using namespace asio::ip;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void on_textEdit_textChanged();

    void mfnc();

    void backf();

    void on_pushQuit_clicked();

    void on_pushCnct_clicked();

private:

    Ui::MainWindow *ui;

    bool isback {false}, isconnected {false};
    double looptime {0};
    uint chcnt {0};

    QTimer *t0, *t1;
    Timer timer;
    QTextCursor csr;

    tcp::endpoint endpnt { address::from_string( "127.0.0.1" ), 31313 };
    io_context* iOc {nullptr};
    tcp::socket* socket0 {nullptr}; //{ iOc };
    error_code ec;


};
#endif // MAINWINDOW_H


