#include "bankingWindow.h"

#include <QDate>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFrame>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>

namespace {
QString money(double amount) {
    return QLocale(QLocale::English, QLocale::UnitedStates).toCurrencyString(amount);
}

QLabel* makeLabel(const QString& text, const QString& objectName = {}) {
    auto* label = new QLabel(text);
    if (!objectName.isEmpty()) {
        label->setObjectName(objectName);
    }
    return label;
}

QFrame* makeMetricCard(const QString& title, QLabel*& valueLabel, const QString& detail) {
    auto* card = new QFrame;
    card->setObjectName("metricCard");
    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 18, 22, 18);
    layout->setSpacing(10);
    layout->addWidget(makeLabel(title, "mutedLabel"));
    valueLabel = makeLabel("0", "metricValue");
    layout->addWidget(valueLabel);
    layout->addWidget(makeLabel(detail, "mutedSmallLabel"));
    return card;
}

QTableWidget* makeTable(const QStringList& headers) {
    auto* table = new QTableWidget(0, headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setShowGrid(false);
    table->setAlternatingRowColors(false);
    table->setFocusPolicy(Qt::NoFocus);
    table->setMinimumHeight(190);
    return table;
}

QTableWidgetItem* makeCell(const QString& text, Qt::Alignment alignment = Qt::AlignVCenter | Qt::AlignLeft) {
    auto* item = new QTableWidgetItem(text);
    item->setTextAlignment(alignment);
    return item;
}
}

BankingWindow::BankingWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Chase Banking");
    resize(1240, 820);
    setMinimumSize(980, 680);
    setupUi();
    updateDashboard();
}

void BankingWindow::setupUi() {
    setStyleSheet(R"(
        QMainWindow, QWidget#appRoot { background: #f3f5f8; color: #17243a; }
        QWidget { font-family: "SF Pro Display", "Segoe UI", sans-serif; font-size: 14px; }
        QFrame#sidebar { background: #111d31; border: 0; }
        QLabel#brandMark { background: #ffffff; color: #123263; border-radius: 12px; font-size: 20px; font-weight: 800; }
        QLabel#brandName { color: #ffffff; font-size: 15px; font-weight: 800; letter-spacing: 1px; }
        QLabel#brandCaption { color: #91a0b7; font-size: 10px; letter-spacing: 1.4px; }
        QLabel#sidebarSection { color: #74839b; font-size: 10px; font-weight: 700; letter-spacing: 1.4px; padding: 18px 12px 6px; }
        QToolButton#navButton { color: #b7c2d3; background: transparent; border: 0; border-radius: 9px; text-align: left; padding: 12px 14px; font-weight: 600; }
        QToolButton#navButton:hover { background: #1b2b45; color: #ffffff; }
        QToolButton#navButton:checked { background: #243a5d; color: #ffffff; }
        QFrame#profileCard { background: #1a2a43; border: 1px solid #293b56; border-radius: 12px; }
        QLabel#avatar { background: #dce8f8; color: #183966; border-radius: 18px; font-weight: 800; }
        QLabel#profileName { color: #ffffff; font-weight: 700; }
        QLabel#profileCaption { color: #91a0b7; font-size: 11px; }
        QFrame#topBar { background: #ffffff; border: 0; border-bottom: 1px solid #e8ebf0; }
        QLabel#pageTitle { color: #15243b; font-size: 21px; font-weight: 750; }
        QLabel#pageSubtitle { color: #7d899a; font-size: 12px; }
        QLabel#dateLabel { color: #69778b; font-size: 12px; }
        QFrame#metricCard, QFrame#contentCard, QFrame#quickCard { background: #ffffff; border: 1px solid #e8ebf0; border-radius: 14px; }
        QLabel#mutedLabel { color: #748196; font-size: 12px; font-weight: 600; }
        QLabel#metricValue { color: #17243a; font-size: 27px; font-weight: 750; }
        QLabel#mutedSmallLabel { color: #9aa4b2; font-size: 11px; }
        QLabel#sectionTitle { color: #1a2940; font-size: 15px; font-weight: 700; }
        QLabel#sectionSubtitle { color: #8a96a7; font-size: 11px; }
        QFrame#featuredCard { background: #16325a; border: 0; border-radius: 16px; }
        QLabel#cardEyebrow { color: #b6c8e3; font-size: 10px; font-weight: 700; letter-spacing: 1.4px; }
        QLabel#cardName { color: #ffffff; font-size: 19px; font-weight: 700; }
        QLabel#cardNumber { color: #c4d2e7; font-size: 13px; letter-spacing: 1px; }
        QLabel#cardBalance { color: #ffffff; font-size: 25px; font-weight: 750; }
        QLabel#cardEmpty { color: #d4dfef; font-size: 13px; }
        QPushButton { border: 0; border-radius: 8px; padding: 10px 15px; font-weight: 650; }
        QPushButton#primaryButton { color: #ffffff; background: #2058bc; }
        QPushButton#primaryButton:hover { background: #174a9f; }
        QPushButton#secondaryButton { color: #2a4264; background: #edf2f8; }
        QPushButton#secondaryButton:hover { background: #e1e9f3; }
        QPushButton#outlineButton { color: #dce8f8; background: transparent; border: 1px solid #61799b; }
        QPushButton#outlineButton:hover { background: #23436d; }
        QTableWidget { background: #ffffff; color: #26364d; border: 1px solid #e8ebf0; border-radius: 12px; padding: 4px; selection-background-color: #e8f0fc; selection-color: #173c70; }
        QHeaderView::section { color: #7d899a; background: #ffffff; border: 0; border-bottom: 1px solid #e8ebf0; padding: 12px 10px; font-size: 11px; font-weight: 700; }
        QTableWidget::item { border-bottom: 1px solid #f0f2f5; padding: 7px; }
        QLineEdit, QSpinBox, QDoubleSpinBox { background: #ffffff; color: #1b2b42; border: 1px solid #dfe4eb; border-radius: 7px; padding: 9px 10px; min-height: 20px; }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 1px solid #4276c7; }
        QDialog { background: #f7f8fa; }
        QDialog QLabel { color: #24334a; }
        QDialog QLabel#dialogTitle { font-size: 19px; font-weight: 750; }
        QLabel#emptyLabel { color: #8a96a7; padding: 12px; }
    )");

    auto* root = new QWidget;
    root->setObjectName("appRoot");
    auto* rootLayout = new QHBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto* sidebar = new QFrame;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(238);
    auto* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(18, 22, 18, 18);
    sidebarLayout->setSpacing(8);

    auto* brandLayout = new QHBoxLayout;
    auto* brandMark = makeLabel("C", "brandMark");
    brandMark->setAlignment(Qt::AlignCenter);
    brandMark->setFixedSize(40, 40);
    auto* brandText = new QVBoxLayout;
    brandText->setSpacing(2);
    brandText->addWidget(makeLabel("CHASE", "brandName"));
    brandText->addWidget(makeLabel("PERSONAL BANKING", "brandCaption"));
    brandLayout->addWidget(brandMark);
    brandLayout->addSpacing(10);
    brandLayout->addLayout(brandText);
    brandLayout->addStretch();
    sidebarLayout->addLayout(brandLayout);
    sidebarLayout->addSpacing(35);
    sidebarLayout->addWidget(makeLabel("MENU", "sidebarSection"));

    auto makeNavButton = [sidebarLayout](const QString& text) {
        auto* button = new QToolButton;
        button->setObjectName("navButton");
        button->setText(text);
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button->setCheckable(true);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sidebarLayout->addWidget(button);
        return button;
    };

    overviewNavButton_ = makeNavButton("  Overview");
    accountsNavButton_ = makeNavButton("  Accounts");
    activityNavButton_ = makeNavButton("  Activity");
    overviewNavButton_->setChecked(true);
    connect(overviewNavButton_, &QToolButton::clicked, this, [this] { setPage(Page::Overview); });
    connect(accountsNavButton_, &QToolButton::clicked, this, [this] { setPage(Page::Accounts); });
    connect(activityNavButton_, &QToolButton::clicked, this, [this] { setPage(Page::Activity); });

    sidebarLayout->addStretch();
    auto* profileCard = new QFrame;
    profileCard->setObjectName("profileCard");
    auto* profileLayout = new QHBoxLayout(profileCard);
    profileLayout->setContentsMargins(12, 12, 10, 12);
    profileLayout->setSpacing(10);
    auto* avatar = makeLabel("CP", "avatar");
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setFixedSize(36, 36);
    auto* profileText = new QVBoxLayout;
    profileText->setSpacing(2);
    profileText->addWidget(makeLabel("Bank customer", "profileName"));
    profileText->addWidget(makeLabel("Personal account", "profileCaption"));
    profileLayout->addWidget(avatar);
    profileLayout->addLayout(profileText);
    sidebarLayout->addWidget(profileCard);

    auto* content = new QWidget;
    auto* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    auto* topBar = new QFrame;
    topBar->setObjectName("topBar");
    auto* topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(30, 18, 30, 18);
    auto* titleColumn = new QVBoxLayout;
    titleColumn->setSpacing(3);
    pageTitle_ = makeLabel("Overview", "pageTitle");
    pageSubtitle_ = makeLabel("A clear view of your money.", "pageSubtitle");
    titleColumn->addWidget(pageTitle_);
    titleColumn->addWidget(pageSubtitle_);
    topBarLayout->addLayout(titleColumn);
    topBarLayout->addStretch();
    dateLabel_ = makeLabel(QDate::currentDate().toString("dddd, MMMM d"), "dateLabel");
    topBarLayout->addWidget(dateLabel_);
    topBarLayout->addSpacing(20);
    auto* addAccountButton = new QPushButton("＋  New account");
    addAccountButton->setObjectName("primaryButton");
    connect(addAccountButton, &QPushButton::clicked, this, &BankingWindow::showCreateAccountDialog);
    topBarLayout->addWidget(addAccountButton);
    contentLayout->addWidget(topBar);

    pageStack_ = new QStackedWidget;
    pageStack_->setContentsMargins(0, 0, 0, 0);
    pageStack_->addWidget(buildOverviewPage());
    pageStack_->addWidget(buildAccountsPage());
    pageStack_->addWidget(buildActivityPage());
    contentLayout->addWidget(pageStack_, 1);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content, 1);
    setCentralWidget(root);
}

QWidget* BankingWindow::buildOverviewPage() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 24, 30, 26);
    layout->setSpacing(18);

    auto* metrics = new QHBoxLayout;
    metrics->setSpacing(14);
    metrics->addWidget(makeMetricCard("TOTAL BALANCE", totalBalanceValue_, "Across all your accounts"), 2);
    metrics->addWidget(makeMetricCard("ACCOUNTS", accountCountValue_, "Accounts you manage"), 1);
    metrics->addWidget(makeMetricCard("ACTIVITY", activityCountValue_, "Recorded transactions"), 1);
    layout->addLayout(metrics);

    auto* featureRow = new QHBoxLayout;
    featureRow->setSpacing(14);
    auto* featuredCard = new QFrame;
    featuredCard->setObjectName("featuredCard");
    featuredCard->setMinimumHeight(190);
    auto* featuredLayout = new QVBoxLayout(featuredCard);
    featuredLayout->setContentsMargins(24, 20, 24, 20);
    featuredLayout->setSpacing(8);
    featuredLayout->addWidget(makeLabel("ACTIVE ACCOUNT", "cardEyebrow"));
    activeAccountName_ = makeLabel("No account selected", "cardName");
    featuredLayout->addWidget(activeAccountName_);
    activeAccountNumber_ = makeLabel("Create an account to get started", "cardNumber");
    featuredLayout->addWidget(activeAccountNumber_);
    featuredLayout->addStretch();
    activeAccountBalance_ = makeLabel(money(0), "cardBalance");
    featuredLayout->addWidget(activeAccountBalance_);
    activeAccountEmpty_ = makeLabel("Create your first account to see it here.", "cardEmpty");
    activeAccountEmpty_->hide();
    featuredLayout->addWidget(activeAccountEmpty_);
    featureRow->addWidget(featuredCard, 3);

    auto* quickCard = new QFrame;
    quickCard->setObjectName("quickCard");
    quickCard->setMinimumWidth(220);
    auto* quickLayout = new QVBoxLayout(quickCard);
    quickLayout->setContentsMargins(20, 20, 20, 20);
    quickLayout->setSpacing(12);
    quickLayout->addWidget(makeLabel("Quick actions", "sectionTitle"));
    quickLayout->addWidget(makeLabel("Manage your selected account.", "sectionSubtitle"));
    cardDepositButton_ = new QPushButton("Deposit funds");
    cardDepositButton_->setObjectName("primaryButton");
    cardWithdrawButton_ = new QPushButton("Withdraw funds");
    cardWithdrawButton_->setObjectName("secondaryButton");
    auto* newAccountButton = new QPushButton("＋  Add an account");
    newAccountButton->setObjectName("secondaryButton");
    connect(cardDepositButton_, &QPushButton::clicked, this, [this] { showTransactionDialog(true); });
    connect(cardWithdrawButton_, &QPushButton::clicked, this, [this] { showTransactionDialog(false); });
    connect(newAccountButton, &QPushButton::clicked, this, &BankingWindow::showCreateAccountDialog);
    quickLayout->addSpacing(4);
    quickLayout->addWidget(cardDepositButton_);
    quickLayout->addWidget(cardWithdrawButton_);
    quickLayout->addStretch();
    quickLayout->addWidget(newAccountButton);
    featureRow->addWidget(quickCard, 2);
    layout->addLayout(featureRow);

    auto* activityHeader = new QHBoxLayout;
    auto* activityTitle = new QVBoxLayout;
    activityTitle->setSpacing(3);
    activityTitle->addWidget(makeLabel("Recent activity", "sectionTitle"));
    activityTitle->addWidget(makeLabel("Your latest account updates.", "sectionSubtitle"));
    activityHeader->addLayout(activityTitle);
    activityHeader->addStretch();
    auto* viewAllButton = new QPushButton("View all");
    viewAllButton->setObjectName("secondaryButton");
    connect(viewAllButton, &QPushButton::clicked, this, [this] { setPage(Page::Activity); });
    activityHeader->addWidget(viewAllButton);
    layout->addLayout(activityHeader);

    overviewActivityTable_ = makeTable({"DESCRIPTION", "ACCOUNT", "DATE", "AMOUNT"});
    overviewActivityTable_->setMinimumHeight(190);
    layout->addWidget(overviewActivityTable_, 1);
    overviewEmpty_ = makeLabel("No activity yet. Deposits and withdrawals will appear here.", "emptyLabel");
    overviewEmpty_->setAlignment(Qt::AlignCenter);
    layout->addWidget(overviewEmpty_);
    return page;
}

QWidget* BankingWindow::buildAccountsPage() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 26, 30, 30);
    layout->setSpacing(16);

    auto* intro = new QVBoxLayout;
    intro->setSpacing(4);
    intro->addWidget(makeLabel("Your accounts", "sectionTitle"));
    intro->addWidget(makeLabel("Select an account to make it active, or add a new one.", "sectionSubtitle"));
    layout->addLayout(intro);

    accountsTable_ = makeTable({"ACCOUNT HOLDER", "ACCOUNT NUMBER", "BALANCE"});
    accountsTable_->setMinimumHeight(300);
    connect(accountsTable_, &QTableWidget::cellClicked, this, [this](int row, int) {
        setSelectedAccount(row);
    });
    layout->addWidget(accountsTable_, 1);

    auto* actions = new QHBoxLayout;
    actions->addStretch();
    auto* createButton = new QPushButton("＋  Create account");
    createButton->setObjectName("primaryButton");
    connect(createButton, &QPushButton::clicked, this, &BankingWindow::showCreateAccountDialog);
    actions->addWidget(createButton);
    layout->addLayout(actions);
    return page;
}

QWidget* BankingWindow::buildActivityPage() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 26, 30, 30);
    layout->setSpacing(16);

    auto* intro = new QVBoxLayout;
    intro->setSpacing(4);
    intro->addWidget(makeLabel("Account activity", "sectionTitle"));
    intro->addWidget(makeLabel("A record of deposits, withdrawals, and account openings.", "sectionSubtitle"));
    layout->addLayout(intro);

    activityTable_ = makeTable({"DESCRIPTION", "ACCOUNT", "DATE & TIME", "AMOUNT"});
    activityTable_->setMinimumHeight(320);
    layout->addWidget(activityTable_, 1);
    activityEmpty_ = makeLabel("No activity to show yet.", "emptyLabel");
    activityEmpty_->setAlignment(Qt::AlignCenter);
    layout->addWidget(activityEmpty_);
    return page;
}

void BankingWindow::setPage(Page page) {
    currentPage_ = page;
    overviewNavButton_->setChecked(page == Page::Overview);
    accountsNavButton_->setChecked(page == Page::Accounts);
    activityNavButton_->setChecked(page == Page::Activity);

    switch (page) {
    case Page::Overview:
        pageStack_->setCurrentIndex(0);
        pageTitle_->setText("Overview");
        pageSubtitle_->setText("A clear view of your money.");
        break;
    case Page::Accounts:
        pageStack_->setCurrentIndex(1);
        pageTitle_->setText("Accounts");
        pageSubtitle_->setText("Manage your bank accounts.");
        break;
    case Page::Activity:
        pageStack_->setCurrentIndex(2);
        pageTitle_->setText("Activity");
        pageSubtitle_->setText("Keep track of every account update.");
        break;
    }
}

void BankingWindow::updateDashboard() {
    double totalBalance = 0;
    for (const auto& account : accounts_) {
        totalBalance += account.getBalance();
    }
    totalBalanceValue_->setText(money(totalBalance));
    accountCountValue_->setText(QString::number(accounts_.size()));
    activityCountValue_->setText(QString::number(transactions_.size()));

    const bool hasSelection = selectedAccountIndex_ >= 0
        && selectedAccountIndex_ < static_cast<int>(accounts_.size());
    cardDepositButton_->setEnabled(hasSelection);
    cardWithdrawButton_->setEnabled(hasSelection);
    activeAccountName_->setVisible(hasSelection);
    activeAccountNumber_->setVisible(hasSelection);
    activeAccountBalance_->setVisible(hasSelection);
    activeAccountEmpty_->setVisible(!hasSelection);
    if (hasSelection) {
        const auto& account = accounts_[selectedAccountIndex_];
        activeAccountName_->setText(QString::fromStdString(account.firstName + " " + account.lastName));
        activeAccountNumber_->setText(QString("ACCOUNT  ••••  %1").arg(account.accountNum, 4, 10, QChar('0')));
        activeAccountBalance_->setText(money(account.getBalance()));
    }

    updateAccountsTable();
    updateActivityTable(overviewActivityTable_, overviewEmpty_, true);
    updateActivityTable(activityTable_, activityEmpty_, false);
}

void BankingWindow::updateAccountsTable() {
    if (!accountsTable_) {
        return;
    }
    accountsTable_->setRowCount(static_cast<int>(accounts_.size()));
    for (int row = 0; row < static_cast<int>(accounts_.size()); ++row) {
        const auto& account = accounts_[row];
        auto* holder = makeCell(QString::fromStdString(account.firstName + " " + account.lastName));
        auto* number = makeCell(QString::number(account.accountNum), Qt::AlignVCenter | Qt::AlignLeft);
        auto* balance = makeCell(money(account.getBalance()), Qt::AlignVCenter | Qt::AlignRight);
        if (row == selectedAccountIndex_) {
            holder->setText("●  " + holder->text());
            holder->setForeground(QColor("#2058bc"));
        }
        accountsTable_->setItem(row, 0, holder);
        accountsTable_->setItem(row, 1, number);
        accountsTable_->setItem(row, 2, balance);
    }
}

void BankingWindow::updateActivityTable(QTableWidget* table, QLabel* emptyLabel, bool recentOnly) {
    if (!table || !emptyLabel) {
        return;
    }
    const int count = recentOnly ? std::min(5, static_cast<int>(transactions_.size()))
                                 : static_cast<int>(transactions_.size());
    table->setRowCount(count);
    for (int row = 0; row < count; ++row) {
        const auto& transaction = transactions_[transactions_.size() - 1 - row];
        auto* type = makeCell(transaction.type);
        auto* account = makeCell(QString("%1  ·  •••• %2")
                                     .arg(transaction.accountHolder)
                                     .arg(transaction.accountNumber, 4, 10, QChar('0')));
        auto* timestamp = makeCell(transaction.timestamp.toString(
            recentOnly ? "MMM d, yyyy" : "MMM d, yyyy  h:mm AP"));
        const QString amountText = transaction.amount == 0
            ? "—"
            : QString("%1%2").arg(transaction.incoming ? "+" : "−", money(transaction.amount));
        auto* amount = makeCell(amountText, Qt::AlignVCenter | Qt::AlignRight);
        if (transaction.amount != 0) {
            amount->setForeground(QColor(transaction.incoming ? "#16804a" : "#b94c4c"));
        }
        table->setItem(row, 0, type);
        table->setItem(row, 1, account);
        table->setItem(row, 2, timestamp);
        table->setItem(row, 3, amount);
    }
    const bool hasTransactions = !transactions_.empty();
    table->setVisible(hasTransactions);
    emptyLabel->setVisible(!hasTransactions);
}

void BankingWindow::addTransaction(const QString& type, const bankAccount& account, double amount, bool incoming) {
    transactions_.push_back({
        type,
        QString::fromStdString(account.firstName + " " + account.lastName),
        account.accountNum,
        amount,
        incoming,
        QDateTime::currentDateTime()
    });
}

void BankingWindow::setSelectedAccount(int index) {
    if (index < 0 || index >= static_cast<int>(accounts_.size())) {
        return;
    }
    selectedAccountIndex_ = index;
    updateDashboard();
}

void BankingWindow::showCreateAccountDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Create an account");
    dialog.setMinimumWidth(390);
    auto* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(24, 22, 24, 20);
    layout->setSpacing(15);
    layout->addWidget(makeLabel("Create an account", "dialogTitle"));
    layout->addWidget(makeLabel("Enter the account holder's details to get started.", "sectionSubtitle"));

    auto* form = new QFormLayout;
    form->setSpacing(12);
    auto* accountNumber = new QSpinBox;
    accountNumber->setRange(1, 999999999);
    accountNumber->setValue(accounts_.empty() ? 1001 : accounts_.back().accountNum + 1);
    auto* firstName = new QLineEdit;
    firstName->setPlaceholderText("First name");
    auto* lastName = new QLineEdit;
    lastName->setPlaceholderText("Last name");
    auto* startingBalance = new QDoubleSpinBox;
    startingBalance->setRange(0, 999999999.99);
    startingBalance->setDecimals(2);
    startingBalance->setPrefix("$ ");
    form->addRow("Account number", accountNumber);
    form->addRow("First name", firstName);
    form->addRow("Last name", lastName);
    form->addRow("Starting balance", startingBalance);
    layout->addLayout(form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
    auto* createButton = buttons->addButton("Create account", QDialogButtonBox::AcceptRole);
    createButton->setObjectName("primaryButton");
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    connect(createButton, &QPushButton::clicked, &dialog, [this, &dialog, accountNumber, firstName, lastName, startingBalance] {
        const QString first = firstName->text().trimmed();
        const QString last = lastName->text().trimmed();
        if (first.isEmpty() || last.isEmpty()) {
            QMessageBox::warning(&dialog, "Missing details", "Enter both a first and last name.");
            return;
        }
        const int number = accountNumber->value();
        const auto duplicate = std::find_if(accounts_.begin(), accounts_.end(), [number](const bankAccount& account) {
            return account.accountNum == number;
        });
        if (duplicate != accounts_.end()) {
            QMessageBox::warning(&dialog, "Account number in use", "Choose a unique account number.");
            return;
        }

        accounts_.emplace_back(number, first.toStdString(), last.toStdString(), startingBalance->value());
        selectedAccountIndex_ = static_cast<int>(accounts_.size()) - 1;
        addTransaction("Account opened", accounts_.back(), startingBalance->value(), true);
        updateDashboard();
        dialog.accept();
    });
    layout->addWidget(buttons);
    dialog.exec();
}

void BankingWindow::showTransactionDialog(bool deposit) {
    if (selectedAccountIndex_ < 0 || selectedAccountIndex_ >= static_cast<int>(accounts_.size())) {
        QMessageBox::information(this, "Select an account", "Choose or create an account before making a transaction.");
        return;
    }

    bankAccount& account = accounts_[selectedAccountIndex_];
    QDialog dialog(this);
    dialog.setWindowTitle(deposit ? "Deposit funds" : "Withdraw funds");
    dialog.setMinimumWidth(390);
    auto* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(24, 22, 24, 20);
    layout->setSpacing(14);
    layout->addWidget(makeLabel(deposit ? "Deposit funds" : "Withdraw funds", "dialogTitle"));
    layout->addWidget(makeLabel(
        QString("Account %1 · %2 %3")
            .arg(account.accountNum)
            .arg(QString::fromStdString(account.firstName))
            .arg(QString::fromStdString(account.lastName)),
        "sectionSubtitle"));

    auto* balanceLabel = makeLabel("Available balance: " + money(account.getBalance()), "mutedLabel");
    layout->addWidget(balanceLabel);
    auto* amount = new QDoubleSpinBox;
    amount->setRange(0.01, 999999999.99);
    amount->setDecimals(2);
    amount->setSingleStep(10);
    amount->setPrefix("$ ");
    amount->setValue(50);
    layout->addWidget(amount);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
    auto* submitButton = buttons->addButton(deposit ? "Confirm deposit" : "Confirm withdrawal", QDialogButtonBox::AcceptRole);
    submitButton->setObjectName("primaryButton");
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    connect(submitButton, &QPushButton::clicked, &dialog, [this, &dialog, &account, amount, deposit] {
        const double value = amount->value();
        const bool succeeded = deposit ? account.deposit(value) : account.withdraw(value);
        if (!succeeded) {
            QMessageBox::warning(&dialog, "Transaction declined",
                deposit ? "Enter a valid amount greater than zero."
                        : "The amount must be positive and cannot exceed the available balance.");
            return;
        }
        addTransaction(deposit ? "Deposit" : "Withdrawal", account, value, deposit);
        updateDashboard();
        dialog.accept();
    });
    layout->addWidget(buttons);
    dialog.exec();
}
