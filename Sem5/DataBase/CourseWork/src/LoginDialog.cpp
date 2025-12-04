#include "LoginDialog.h"
#include "AuthManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>  // Добавьте эту строку

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Авторизация");
    setFixedSize(300, 150);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Поле логина
    QHBoxLayout* loginLayout = new QHBoxLayout();
    loginLayout->addWidget(new QLabel("Логин:"));
    usernameEdit = new QLineEdit();
    loginLayout->addWidget(usernameEdit);
    layout->addLayout(loginLayout);
    
    // Поле пароля
    QHBoxLayout* passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(new QLabel("Пароль:"));
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordEdit);
    layout->addLayout(passwordLayout);
    
    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Войти");
    cancelButton = new QPushButton("Отмена");
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    // Подключение сигналов
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(cancelButton, &QPushButton::clicked, this, &LoginDialog::reject);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
}

QString LoginDialog::getUsername() const { 
    return usernameEdit->text(); 
}

QString LoginDialog::getPassword() const { 
    return passwordEdit->text(); 
}

bool LoginDialog::isAuthenticated() const { 
    return authenticated; 
}

bool LoginDialog::isAdmin() const { 
    return admin; 
}

void LoginDialog::onLoginClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль");
        return;
    }

    if (AuthManager::authenticateUser(username.toStdString(), password.toStdString())) {
        authenticated = true;
        admin = AuthManager::isAdmin(username.toStdString());
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
        passwordEdit->clear();
    }
}
