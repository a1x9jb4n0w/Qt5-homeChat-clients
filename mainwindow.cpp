#include "mainwindow.h"
#include "ui_mainwindow.h"

char readbuf[128]{"AAAAA"};
struct r_handler
{
    r_handler* th;
    Ui::MainWindow* uu;
    tcp::socket* cur_sct;

    void operator()( const error_code& error, std::size_t tbytes )
    {
        if ( !error ) { readbuf[tbytes] = 0;

            std::stringstream ss; ss << readbuf;
            std::string s0 = ss.str();
            uu->textBrowser->append( s0.data() );
            std::cout << readbuf << std::endl;
            cur_sct->async_read_some( asio::buffer( readbuf, 128 ), *th );

        } else { std::cout << error.message() << " !!! " << std::endl;  }

    }

} r_h0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); ui->textEdit->setPlainText("Nick: ");
    ui->textBrowser->setFocusProxy(ui->textEdit);
    csr = ui->textEdit->textCursor(); csr.movePosition( QTextCursor::End, QTextCursor::KeepAnchor);



    t0 = new QTimer(this); connect(t0, SIGNAL(timeout()), this, SLOT(mfnc())); t0->start(65);
    t1 = new QTimer(this); connect(t1, SIGNAL(timeout()), this, SLOT(backf())); t1->start(0);

}

MainWindow::~MainWindow()
{
    delete ui; //delete r_h0;
    if ( socket0 ) delete socket0;
    if ( iOc ) delete iOc;

}

void MainWindow::backf()
{
    if ( !isback ) { timer.reset();

        if ( iOc ) iOc->poll();

        isback = true;

        looptime = timer.elapsed(); }
}

void MainWindow::mfnc()
{
    std::stringstream ss;

    isback=false;
}


void MainWindow::on_pushQuit_clicked()
{
    QApplication::instance()->quit();
}

void MainWindow::on_textEdit_textChanged()
{
    QString ts = ui->textEdit->toPlainText(); int len = ts.length();
    if ( len == 5 ) {

        ui->textEdit->setPlainText("Nick: "); ui->textEdit->setTextCursor( csr );

    }
    else { QChar cc; cc = *( ts.end() - 1 );

    if ( cc.unicode() == 10 ) {

        QString ss = ui->textEdit->toPlainText();
        ss.truncate( ss.size() - 1 );
        ui->textBrowser->append( ss );
        ui->textEdit->setPlainText("Nick: ");
        ui->textEdit->setTextCursor( csr );
        std::string s0 = ss.toStdString();
        std::cout << s0 << " " << &socket0 << std::endl;
        if ( isconnected ) socket0->write_some( asio::buffer( s0, ss.length() ) );

    } }

}
void MainWindow::on_pushCnct_clicked()
{
    if ( !isconnected ) { iOc = new io_context; socket0 = new tcp::socket{*iOc};
        socket0->connect( endpnt, ec ); std::cout  << &socket0 << " " << socket0->is_open() << std::endl;

        if ( !ec ) { isconnected = true;

            ui->textBrowser->append( ">>> Connection established. Welcome to chat ->" );
            ui->pushCnct->setText("Disconnect");

            r_h0.th = &r_h0; r_h0.uu = ui; r_h0.cur_sct = socket0;
            std::cout  << &socket0 << " " << socket0->is_open() << std::endl;
            socket0->async_read_some( asio::buffer( readbuf, 128 ), r_h0 );

         }
         else ui->textBrowser->append( ">>> No connection. Try again ->" ); }

    else {  socket0->write_some( asio::buffer("SC0", 3));
            socket0->close();
            delete socket0; socket0 = nullptr;
            delete iOc; iOc = nullptr;
            isconnected = false;
            ui->textBrowser->append( ">>> Disconnected " );
            ui->pushCnct->setText("Connect");}
}
