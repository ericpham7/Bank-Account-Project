#pragma once

#include "bankAccount.h"

#include <QDateTime>
#include <QMainWindow>
#include <QString>
#include <vector>

class QLabel;
class QPushButton;
class QStackedWidget;
class QTableWidget;
class QToolButton;

class BankingWindow : public QMainWindow {
public:
    explicit BankingWindow(QWidget* parent = nullptr);

private:
    struct Transaction {
        QString type;
        QString accountHolder;
        int accountNumber;
        double amount;
        bool incoming;
        QDateTime timestamp;
    };

    enum class Page {
        Overview,
        Accounts,
        Activity
    };

    std::vector<bankAccount> accounts_;
    std::vector<Transaction> transactions_;
    int selectedAccountIndex_ = -1;
    Page currentPage_ = Page::Overview;

    QStackedWidget* pageStack_ = nullptr;
    QLabel* pageTitle_ = nullptr;
    QLabel* pageSubtitle_ = nullptr;
    QLabel* dateLabel_ = nullptr;
    QLabel* totalBalanceValue_ = nullptr;
    QLabel* accountCountValue_ = nullptr;
    QLabel* activityCountValue_ = nullptr;
    QLabel* activeAccountName_ = nullptr;
    QLabel* activeAccountNumber_ = nullptr;
    QLabel* activeAccountBalance_ = nullptr;
    QLabel* activeAccountEmpty_ = nullptr;
    QLabel* overviewEmpty_ = nullptr;
    QLabel* activityEmpty_ = nullptr;
    QTableWidget* accountsTable_ = nullptr;
    QTableWidget* overviewActivityTable_ = nullptr;
    QTableWidget* activityTable_ = nullptr;
    QPushButton* cardDepositButton_ = nullptr;
    QPushButton* cardWithdrawButton_ = nullptr;
    QToolButton* overviewNavButton_ = nullptr;
    QToolButton* accountsNavButton_ = nullptr;
    QToolButton* activityNavButton_ = nullptr;

    void setupUi();
    QWidget* buildOverviewPage();
    QWidget* buildAccountsPage();
    QWidget* buildActivityPage();
    void setPage(Page page);
    void updateDashboard();
    void updateAccountsTable();
    void updateActivityTable(QTableWidget* table, QLabel* emptyLabel, bool recentOnly);
    void addTransaction(const QString& type, const bankAccount& account, double amount, bool incoming);
    void showCreateAccountDialog();
    void showTransactionDialog(bool deposit);
    void setSelectedAccount(int index);
};
