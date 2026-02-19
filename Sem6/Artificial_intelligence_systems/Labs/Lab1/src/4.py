import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import os
from sklearn.svm import SVC
from sklearn.metrics import confusion_matrix, accuracy_score, ConfusionMatrixDisplay

os.makedirs('../images', exist_ok=True)

def plot_decision_boundary(ax, clf, X, y, title, show_sv=False):
    h = 0.02
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    ax.contourf(xx, yy, Z, alpha=0.3, cmap=ListedColormap(['red', 'green']))
    ax.scatter(X[y==0, 0], X[y==0, 1], c='red', edgecolors='k', label='red')
    ax.scatter(X[y==1, 0], X[y==1, 1], c='green', edgecolors='k', label='green')
    if show_sv:
        ax.scatter(clf.support_vectors_[:, 0], clf.support_vectors_[:, 1],
                   s=100, linewidth=1, facecolors='none', edgecolors='blue', label='support vectors')
    ax.set_xlim(xx.min(), xx.max())
    ax.set_ylim(yy.min(), yy.max())
    ax.set_title(title)
    ax.legend()

print("\n=== Пункт a (датасет a): линейное ядро ===")
data_train = pd.read_csv('../data/svmdata_a.txt', sep='\t')
data_test = pd.read_csv('../data/svmdata_a_test.txt', sep='\t')
color_col = 'Colors' if 'Colors' in data_train.columns else 'Color'
X_train = data_train[['X1', 'X2']].values
y_train = (data_train[color_col] == 'green').astype(int)
X_test = data_test[['X1', 'X2']].values
y_test = (data_test[color_col] == 'green').astype(int)

clf_linear = SVC(kernel='linear', C=1.0, random_state=42)
clf_linear.fit(X_train, y_train)
y_pred_train = clf_linear.predict(X_train)
y_pred_test = clf_linear.predict(X_test)
print("Точность на train:", accuracy_score(y_train, y_pred_train))
print("Точность на test:", accuracy_score(y_test, y_pred_test))
print("Количество опорных векторов:", len(clf_linear.support_vectors_))

fig, axes = plt.subplots(2, 2, figsize=(14, 12))
plot_decision_boundary(axes[0,0], clf_linear, X_train, y_train,
                       f'Обучающая выборка (линейное ядро)', show_sv=True)
plot_decision_boundary(axes[0,1], clf_linear, X_test, y_test,
                       f'Тестовая выборка (линейное ядро)', show_sv=True)

cm_train = confusion_matrix(y_train, y_pred_train)
cm_test = confusion_matrix(y_test, y_pred_test)
disp_train = ConfusionMatrixDisplay(cm_train, display_labels=['red', 'green'])
disp_train.plot(ax=axes[1,0])
axes[1,0].set_title('Матрица ошибок на train')
disp_test = ConfusionMatrixDisplay(cm_test, display_labels=['red', 'green'])
disp_test.plot(ax=axes[1,1])
axes[1,1].set_title('Матрица ошибок на test')

plt.suptitle('Датасет a, линейное ядро')
plt.tight_layout()
plt.savefig('../images/svm_a_linear_combined.png', dpi=150)
plt.show()

print("\n=== Пункт b (датасет b): подбор параметра C ===")
data_train = pd.read_csv('../data/svmdata_b.txt', sep='\t')
data_test = pd.read_csv('../data/svmdata_b_test.txt', sep='\t')
color_col = 'Colors' if 'Colors' in data_train.columns else 'Color'
X_train = data_train[['X1', 'X2']].values
y_train = (data_train[color_col] == 'green').astype(int)
X_test = data_test[['X1', 'X2']].values
y_test = (data_test[color_col] == 'green').astype(int)

C_range = np.logspace(-3, 3, 7)
train_scores = []
test_scores = []
for C in C_range:
    clf = SVC(kernel='linear', C=C, random_state=42)
    clf.fit(X_train, y_train)
    train_scores.append(clf.score(X_train, y_train))
    test_scores.append(clf.score(X_test, y_test))

plt.figure(figsize=(8,6))
plt.semilogx(C_range, train_scores, 'o-', label='train accuracy')
plt.semilogx(C_range, test_scores, 's-', label='test accuracy')
plt.xlabel('C')
plt.ylabel('Accuracy')
plt.title('Зависимость точности от C (датасет b, линейное ядро)')
plt.legend()
plt.grid(True)
plt.savefig('../images/svm_b_C_tuning.png', dpi=150)
plt.show()

best_C = C_range[np.argmax(test_scores)]
print(f"Оптимальное C по тестовой точности: {best_C:.3f}")
print("При этом точность на train:", train_scores[np.argmax(test_scores)])
print("На тесте:", test_scores[np.argmax(test_scores)])

print("\nПопытка достичь нулевой ошибки на train и test:")
for C in [1e3, 1e4, 1e5]:
    clf = SVC(kernel='linear', C=C, random_state=42)
    clf.fit(X_train, y_train)
    print(f"C={C:.0e}, train acc={clf.score(X_train, y_train):.3f}, test acc={clf.score(X_test, y_test):.3f}")
print("Для данного набора линейное ядро не может дать нулевую ошибку, если данные нелинейно разделимы.")

print("\n=== Пункт c (датасет c): различные ядра ===")
data_train = pd.read_csv('../data/svmdata_c.txt', sep='\t')
data_test = pd.read_csv('../data/svmdata_c_test.txt', sep='\t')
color_col = 'Colors' if 'Colors' in data_train.columns else 'Color'
X_train = data_train[['X1', 'X2']].values
y_train = (data_train[color_col] == 'green').astype(int)
X_test = data_test[['X1', 'X2']].values
y_test = (data_test[color_col] == 'green').astype(int)

kernels_c = {
    'linear': SVC(kernel='linear', C=1.0, random_state=42),
    'poly_1': SVC(kernel='poly', degree=1, C=1.0, random_state=42),
    'poly_2': SVC(kernel='poly', degree=2, C=1.0, random_state=42),
    'poly_3': SVC(kernel='poly', degree=3, C=1.0, random_state=42),
    'poly_4': SVC(kernel='poly', degree=4, C=1.0, random_state=42),
    'poly_5': SVC(kernel='poly', degree=5, C=1.0, random_state=42),
    'sigmoid': SVC(kernel='sigmoid', C=1.0, random_state=42),
    'rbf': SVC(kernel='rbf', C=1.0, gamma='scale', random_state=42)
}

trained_c = {}
for name, clf in kernels_c.items():
    clf.fit(X_train, y_train)
    trained_c[name] = clf
    train_acc = clf.score(X_train, y_train)
    test_acc = clf.score(X_test, y_test)
    print(f"{name}: train={train_acc:.3f}, test={test_acc:.3f}")

fig_c, axes_c = plt.subplots(4, 4, figsize=(24, 20))
axes_c = axes_c.flatten()
for idx, (name, clf) in enumerate(trained_c.items()):
    plot_decision_boundary(axes_c[2*idx], clf, X_train, y_train, f'{name} (train)')
    plot_decision_boundary(axes_c[2*idx+1], clf, X_test, y_test, f'{name} (test)')
for i in range(2*len(kernels_c), 16):
    axes_c[i].axis('off')
fig_c.suptitle('Датасет c, все ядра (train и test)')
fig_c.tight_layout()
fig_c.savefig('../images/svm_c_kernels_c.png', dpi=150)
plt.show()

print("\n=== Пункт d (датасет d): ядра без линейного ===")
data_train = pd.read_csv('../data/svmdata_d.txt', sep='\t')
data_test = pd.read_csv('../data/svmdata_d_test.txt', sep='\t')
color_col = 'Colors' if 'Colors' in data_train.columns else 'Color'
X_train = data_train[['X1', 'X2']].values
y_train = (data_train[color_col] == 'green').astype(int)
X_test = data_test[['X1', 'X2']].values
y_test = (data_test[color_col] == 'green').astype(int)

kernels_d = {
    'poly_1': SVC(kernel='poly', degree=1, C=1.0, random_state=42),
    'poly_2': SVC(kernel='poly', degree=2, C=1.0, random_state=42),
    'poly_3': SVC(kernel='poly', degree=3, C=1.0, random_state=42),
    'poly_4': SVC(kernel='poly', degree=4, C=1.0, random_state=42),
    'poly_5': SVC(kernel='poly', degree=5, C=1.0, random_state=42),
    'sigmoid': SVC(kernel='sigmoid', C=1.0, random_state=42),
    'rbf': SVC(kernel='rbf', C=1.0, gamma='scale', random_state=42)
}

trained_d = {}
for name, clf in kernels_d.items():
    clf.fit(X_train, y_train)
    trained_d[name] = clf
    train_acc = clf.score(X_train, y_train)
    test_acc = clf.score(X_test, y_test)
    print(f"{name}: train={train_acc:.3f}, test={test_acc:.3f}")

fig_d, axes_d = plt.subplots(4, 4, figsize=(24, 20))
axes_d = axes_d.flatten()
for idx, (name, clf) in enumerate(trained_d.items()):
    plot_decision_boundary(axes_d[2*idx], clf, X_train, y_train, f'{name} (train)')
    plot_decision_boundary(axes_d[2*idx+1], clf, X_test, y_test, f'{name} (test)')
for i in range(2*len(kernels_d), 16):
    axes_d[i].axis('off')
fig_d.suptitle('Датасет d, ядра без линейного (train и test)')
fig_d.tight_layout()
fig_d.savefig('../images/svm_d_kernels_d.png', dpi=150)
plt.show()

print("\n=== Пункт e (датасет e): влияние гаммы (RBF) ===")
data_train = pd.read_csv('../data/svmdata_e.txt', sep='\t')
data_test = pd.read_csv('../data/svmdata_e_test.txt', sep='\t')
color_col = 'Colors' if 'Colors' in data_train.columns else 'Color'
X_train = data_train[['X1', 'X2']].values
y_train = (data_train[color_col] == 'green').astype(int)
X_test = data_test[['X1', 'X2']].values
y_test = (data_test[color_col] == 'green').astype(int)

gamma_values = [0.1, 1.0, 10.0, 100.0]
trained_gamma = {}
for gamma in gamma_values:
    clf = SVC(kernel='rbf', C=1.0, gamma=gamma, random_state=42)
    clf.fit(X_train, y_train)
    trained_gamma[gamma] = clf
    train_acc = clf.score(X_train, y_train)
    test_acc = clf.score(X_test, y_test)
    print(f"gamma={gamma}: train={train_acc:.3f}, test={test_acc:.3f}")

fig_gamma, axes_gamma = plt.subplots(4, 2, figsize=(16, 20))
axes_gamma = axes_gamma.flatten()
for idx, (gamma, clf) in enumerate(trained_gamma.items()):
    plot_decision_boundary(axes_gamma[2*idx], clf, X_train, y_train, f'gamma={gamma} (train)')
    plot_decision_boundary(axes_gamma[2*idx+1], clf, X_test, y_test, f'gamma={gamma} (test)')
fig_gamma.suptitle('Датасет e, RBF ядро, разные gamma (train и test)')
fig_gamma.tight_layout()
fig_gamma.savefig('../images/svm_e_gamma_all.png', dpi=150)
plt.show()

print("При малой гамме граница гладкая (недообучение), при большой — изрезанная (переобучение).")
