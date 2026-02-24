import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB

spam_df = pd.read_csv('../data/spam.csv')
spam_df = spam_df.drop(spam_df.columns[0], axis=1)
X_spam = spam_df.drop('type', axis=1).values.astype(float)
y_spam = spam_df['type'].map({'spam': 1, 'nonspam': 0}).values

def load_tic_tac_toe(filename):
    with open(filename, 'r') as f:
        lines = [line.strip().split(',') for line in f]
    data = []
    for row in lines:
        features = row[:9]
        label = row[9]
        feat_num = [1 if v == 'x' else 2 if v == 'o' else 3 for v in features]
        data.append(feat_num + [1 if label == 'positive' else 0])
    return np.array(data)

data_ttt = load_tic_tac_toe('../data/tic_tac_toe.txt')
X_ttt = data_ttt[:, :-1].astype(float)
y_ttt = data_ttt[:, -1].astype(int)

test_sizes = np.linspace(0.1, 0.9, 9)
train_acc_spam = []
test_acc_spam = []
train_acc_ttt = []
test_acc_ttt = []

RANDOM_STATE = 42

for ts in test_sizes:
    X_train, X_test, y_train, y_test = train_test_split(
        X_spam, y_spam, test_size=ts, stratify=y_spam, random_state=RANDOM_STATE
    )
    model = GaussianNB()
    model.fit(X_train, y_train)
    train_acc_spam.append(model.score(X_train, y_train))
    test_acc_spam.append(model.score(X_test, y_test))

    X_train, X_test, y_train, y_test = train_test_split(
        X_ttt, y_ttt, test_size=ts, stratify=y_ttt, random_state=RANDOM_STATE
    )
    model = GaussianNB()
    model.fit(X_train, y_train)
    train_acc_ttt.append(model.score(X_train, y_train))
    test_acc_ttt.append(model.score(X_test, y_test))

fig, axes = plt.subplots(1, 2, figsize=(12, 5))

axes[0].plot(test_sizes, train_acc_spam, 'o-', label='Точность на обучении')
axes[0].plot(test_sizes, test_acc_spam, 's-', label='Точность на тесте')
axes[0].set_xlabel('Доля тестовых данных')
axes[0].set_ylabel('Точность')
axes[0].set_title('Набор данных "spam"')
axes[0].legend()
axes[0].grid(True)

axes[1].plot(test_sizes, train_acc_ttt, 'o-', label='Точность на обучении')
axes[1].plot(test_sizes, test_acc_ttt, 's-', label='Точность на тесте')
axes[1].set_xlabel('Доля тестовых данных')
axes[1].set_ylabel('Точность')
axes[1].set_title('Набор данных "tic-tac-toe"')
axes[1].legend()
axes[1].grid(True)

plt.tight_layout()
plt.savefig('../images/accuracy_vs_testsize.png', dpi=150)
plt.show()

print("Доля теста\tspam_train\tspam_test\tttt_train\tttt_test")
for i, ts in enumerate(test_sizes):
    print(f"{ts:.1f}\t{train_acc_spam[i]:.3f}\t{test_acc_spam[i]:.3f}\t{train_acc_ttt[i]:.3f}\t{test_acc_ttt[i]:.3f}")
