#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    LoginDialog(QWidget* parent = nullptr);
    
    QString getUsername() const;
    QString getPassword() const;
    bool isAuthenticated() const;
    bool isAdmin() const;

private slots:
    void onLoginClicked();

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QPushButton* cancelButton;
    bool authenticated = false;
    bool admin = false;
};

#endif
