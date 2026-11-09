#include "bankingWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);
    QApplication::setApplicationName("Chase Banking");
    QApplication::setOrganizationName("Bank Account Project");

    BankingWindow window;
    window.show();

    return application.exec();
}
