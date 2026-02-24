import numpy as np
import matplotlib.pyplot as plt
import os
from sklearn.model_selection import train_test_split
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis
from sklearn.metrics import (accuracy_score, confusion_matrix, ConfusionMatrixDisplay,
                             roc_curve, auc, precision_recall_curve, average_precision_score)
import warnings
warnings.filterwarnings('ignore')

os.makedirs('../images', exist_ok=True)

mean_minus = [8, 25]
std_minus = 3
mean_plus = [7, 10]
std_plus = 2
n_minus = 50
n_plus = 50
random_state = 42
np.random.seed(random_state)

X_minus = np.random.normal(loc=mean_minus, scale=std_minus, size=(n_minus, 2))
y_minus = -np.ones(n_minus)

X_plus = np.random.normal(loc=mean_plus, scale=std_plus, size=(n_plus, 2))
y_plus = np.ones(n_plus)

X = np.vstack((X_minus, X_plus))
y = np.hstack((y_minus, y_plus)).astype(int)

plt.figure(figsize=(6,5))
plt.scatter(X[y==-1, 0], X[y==-1, 1], c='red', label='Класс -1', alpha=0.7)
plt.scatter(X[y==1, 0], X[y==1, 1], c='blue', label='Класс 1', alpha=0.7)
plt.xlabel('X1')
plt.ylabel('X2')
plt.title('Сгенерированные данные')
plt.legend()
plt.grid(True)
plt.savefig('../images/data_scatter.png', dpi=150)
plt.show()

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, stratify=y, random_state=random_state
)

qda = QuadraticDiscriminantAnalysis()
qda.fit(X_train, y_train)

y_pred = qda.predict(X_test)

acc = accuracy_score(y_test, y_pred)
print(f"Точность на тесте: {acc:.3f}")

cm = confusion_matrix(y_test, y_pred)
disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=['-1', '1'])
disp.plot(cmap='Blues')
plt.title('Матрица ошибок')
plt.savefig('../images/confusion_matrix.png', dpi=150)
plt.show()

y_proba = qda.predict_proba(X_test)[:, 1]
fpr, tpr, thresholds = roc_curve(y_test, y_proba, pos_label=1)
roc_auc = auc(fpr, tpr)
print(f"ROC AUC = {roc_auc:.3f}")

plt.figure()
plt.plot(fpr, tpr, label=f'ROC кривая (AUC = {roc_auc:.3f})')
plt.plot([0,1], [0,1], 'k--', label='Случайный')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('ROC-кривая')
plt.legend()
plt.grid(True)
plt.savefig('../images/roc_curve.png', dpi=150)
plt.show()

precision, recall, _ = precision_recall_curve(y_test, y_proba, pos_label=1)
pr_auc = average_precision_score(y_test, y_proba)
print(f"Average Precision = {pr_auc:.3f}")

plt.figure()
plt.plot(recall, precision, label=f'PR кривая (AP = {pr_auc:.3f})')
plt.xlabel('Recall')
plt.ylabel('Precision')
plt.title('Precision-Recall кривая')
plt.legend()
plt.grid(True)
plt.savefig('../images/pr_curve.png', dpi=150)
plt.show()

print("\nАнализ:")
print("Классы хорошо разделяются по признаку X2 (средние 25 и 10 при СКО 3 и 2).")
print("Поэтому классификатор показывает практически идеальные результаты.")
if acc > 0.95 and roc_auc > 0.99:
    print("Классификатор можно считать отличным.")
else:
    print("Классификатор хороший, но возможны ошибки из-за случайности.")
