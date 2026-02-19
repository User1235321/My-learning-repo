import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split, cross_val_score, GridSearchCV
from sklearn.tree import DecisionTreeClassifier, plot_tree
from sklearn.metrics import accuracy_score, confusion_matrix, ConfusionMatrixDisplay
import os

os.makedirs('../images', exist_ok=True)

glass_data = pd.read_csv('../data/glass.csv')
glass_data = glass_data.drop('Id', axis=1)
X_glass = glass_data.drop('Type', axis=1)
y_glass = glass_data['Type']

X_train_g, X_test_g, y_train_g, y_test_g = train_test_split(X_glass, y_glass, test_size=0.2, stratify=y_glass, random_state=42)

depths = range(1, 21)
train_scores_g = []
test_scores_g = []
for depth in depths:
    tree = DecisionTreeClassifier(max_depth=depth, random_state=42)
    tree.fit(X_train_g, y_train_g)
    train_scores_g.append(accuracy_score(y_train_g, tree.predict(X_train_g)))
    test_scores_g.append(accuracy_score(y_test_g, tree.predict(X_test_g)))

plt.figure(figsize=(10,6))
plt.plot(depths, train_scores_g, 'o-', label='train accuracy')
plt.plot(depths, test_scores_g, 's-', label='test accuracy')
plt.xlabel('max_depth')
plt.ylabel('Accuracy')
plt.title('Glass: зависимость точности от глубины дерева')
plt.legend()
plt.grid(True)
plt.savefig('../images/glass_depth_accuracy.png', dpi=150)
plt.show()

criterions = ['gini', 'entropy']
cv_scores_g = {}
for crit in criterions:
    scores = cross_val_score(DecisionTreeClassifier(criterion=crit, random_state=42), X_glass, y_glass, cv=5)
    cv_scores_g[crit] = scores.mean()
    print(f"Glass: criterion={crit}, CV accuracy={scores.mean():.3f} (+/-{scores.std():.2f})")

best_depth = depths[np.argmax(test_scores_g)]
final_tree_g = DecisionTreeClassifier(max_depth=best_depth, criterion='gini', random_state=42)
final_tree_g.fit(X_train_g, y_train_g)
print(f"Glass: лучшая глубина = {best_depth}, test accuracy = {accuracy_score(y_test_g, final_tree_g.predict(X_test_g)):.3f}")

plt.figure(figsize=(20,10))
plot_tree(final_tree_g, filled=True, feature_names=X_glass.columns, class_names=[str(c) for c in sorted(y_glass.unique())])
plt.title(f'Glass дерево (max_depth={best_depth})')
plt.savefig('../images/glass_tree.png', dpi=150)
plt.show()

spam_data = pd.read_csv('../data/spam7.csv')
X_spam = spam_data.drop('yesno', axis=1)
y_spam = spam_data['yesno'].map({'y':1, 'n':0})

X_train_s, X_test_s, y_train_s, y_test_s = train_test_split(X_spam, y_spam, test_size=0.2, stratify=y_spam, random_state=42)

param_grid = {
    'criterion': ['gini', 'entropy'],
    'max_depth': [3, 5, 7, 10, None],
    'min_samples_split': [2, 5, 10],
    'min_samples_leaf': [1, 2, 4]
}
tree = DecisionTreeClassifier(random_state=42)
grid_search = GridSearchCV(tree, param_grid, cv=5, scoring='accuracy', n_jobs=-1)
grid_search.fit(X_train_s, y_train_s)
print(f"Spam: лучшие параметры = {grid_search.best_params_}")
print(f"Spam: лучшая CV точность = {grid_search.best_score_:.3f}")

best_tree_s = grid_search.best_estimator_
y_pred_s = best_tree_s.predict(X_test_s)
test_acc_s = accuracy_score(y_test_s, y_pred_s)
print(f"Spam: test accuracy = {test_acc_s:.3f}")

plt.figure(figsize=(20,10))
plot_tree(best_tree_s, filled=True, feature_names=X_spam.columns, class_names=['n','y'])
plt.title('Spam оптимальное дерево')
plt.savefig('../images/spam_tree.png', dpi=150)
plt.show()

importances = best_tree_s.feature_importances_
indices = np.argsort(importances)[::-1]
plt.figure(figsize=(8,6))
plt.bar(range(X_spam.shape[1]), importances[indices])
plt.xticks(range(X_spam.shape[1]), X_spam.columns[indices], rotation=45)
plt.title('Важность признаков (spam)')
plt.tight_layout()
plt.savefig('../images/spam_importances.png', dpi=150)
plt.show()

cm = confusion_matrix(y_test_s, y_pred_s)
disp = ConfusionMatrixDisplay(cm, display_labels=['n','y'])
disp.plot()
plt.title('Матрица ошибок (spam)')
plt.savefig('../images/spam_cm.png', dpi=150)
plt.show()
