import pandas as pd
import numpy as np
from sklearn.model_selection import cross_val_score, StratifiedKFold
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier, StackingClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt
import warnings
warnings.filterwarnings('ignore')

import os
os.makedirs('../images', exist_ok=True)

print("Загрузка данных...")
train_df = pd.read_csv('../data/titanic_train.csv')
test_df = pd.read_csv('../data/titanic_test.csv')

print(f"Размер обучающего набора: {train_df.shape}")
print(f"Размер тестового набора: {test_df.shape}")
print()

def preprocess_titanic(df, is_train=True):
    data = df.copy()
    
    if is_train:
        y = data['Survived']
        data = data.drop(['Survived', 'PassengerId'], axis=1)
    else:
        passenger_ids = data['PassengerId']
        data = data.drop(['PassengerId'], axis=1)
    
    data['Title'] = data['Name'].apply(lambda x: x.split(',')[1].split('.')[0].strip())
    data['Title'] = data['Title'].replace(['Lady', 'the Countess', 'Countess', 'Capt', 'Col', 'Don', 
                                            'Dr', 'Major', 'Rev', 'Sir', 'Jonkheer', 'Dona', 'Mme'], 'Rare')
    data['Title'] = data['Title'].replace(['Mlle', 'Ms'], 'Miss')
    data['Title'] = data['Title'].replace('Mme', 'Mrs')
    
    data['FamilySize'] = data['SibSp'] + data['Parch'] + 1
    data['IsAlone'] = (data['FamilySize'] == 1).astype(int)
    
    data['Cabin'] = data['Cabin'].fillna('U')
    data['Deck'] = data['Cabin'].apply(lambda x: str(x)[0])
    
    data['Age'] = data.groupby('Title')['Age'].transform(lambda x: x.fillna(x.median()))
    data['Age'] = data['Age'].fillna(data['Age'].median())
    
    data['Fare'] = data.groupby('Pclass')['Fare'].transform(lambda x: x.fillna(x.median()))
    data['Fare'] = data['Fare'].fillna(data['Fare'].median())
    
    data['Embarked'] = data['Embarked'].fillna('S')
    
    data['Sex'] = (data['Sex'] == 'male').astype(int)
    
    data = pd.get_dummies(data, columns=['Title', 'Deck', 'Embarked'], drop_first=True)
    
    data = data.drop(['Name', 'Ticket', 'Cabin'], axis=1)
    
    if is_train:
        return data, y
    else:
        return data, passenger_ids

print("Предобработка данных...")
X_train, y_train = preprocess_titanic(train_df, is_train=True)
X_test, passenger_ids = preprocess_titanic(test_df, is_train=False)

print(f"Признаки после предобработки: {X_train.shape[1]}")
print()

missing_cols = set(X_train.columns) - set(X_test.columns)
for col in missing_cols:
    X_test[col] = 0

X_test = X_test[X_train.columns]

print("Нормализация данных...")
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)
print("Нормализация завершена")
print()

base_models = [
    ('rf', RandomForestClassifier(n_estimators=100, max_depth=5, random_state=42)),
    ('gbm', GradientBoostingClassifier(n_estimators=100, max_depth=3, random_state=42)),
    ('svm', SVC(kernel='rbf', C=1.0, gamma='scale', probability=True, random_state=42)),
    ('knn', KNeighborsClassifier(n_neighbors=7)),
    ('dt', DecisionTreeClassifier(max_depth=4, random_state=42))
]

meta_model = LogisticRegression(max_iter=1000, random_state=42)

stacking_clf = StackingClassifier(
    estimators=base_models,
    final_estimator=meta_model,
    cv=5,
    stack_method='predict_proba'
)

print("Оценка качества моделей с помощью кросс-валидации...")
cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)

model_scores = {}
for name, model in base_models:
    scores = cross_val_score(model, X_train_scaled, y_train, cv=cv, scoring='accuracy')
    model_scores[name] = {'mean': scores.mean(), 'std': scores.std()}
    print(f"{name}: accuracy = {scores.mean():.4f} (+/- {scores.std():.4f})")

stacking_scores = cross_val_score(stacking_clf, X_train_scaled, y_train, cv=cv, scoring='accuracy')
model_scores['stacking'] = {'mean': stacking_scores.mean(), 'std': stacking_scores.std()}
print(f"stacking: accuracy = {stacking_scores.mean():.4f} (+/- {stacking_scores.std():.4f})")
print()

print("Обучение стекинг-классификатора на всех обучающих данных...")
stacking_clf.fit(X_train_scaled, y_train)
print("Обучение завершено")
print()

y_pred_proba = stacking_clf.predict_proba(X_test_scaled)
y_pred = stacking_clf.predict(X_test_scaled)

submission = pd.DataFrame({
    'PassengerId': passenger_ids,
    'Survived': y_pred
})

submission.to_csv('../data/titanic_predictions.csv', index=False)
print("Предсказания сохранены в файл '../data/titanic_predictions.csv'")
print()

print("Визуализация результатов...")

plt.figure(figsize=(12, 6))
models = list(model_scores.keys())
means = [model_scores[m]['mean'] for m in models]
stds = [model_scores[m]['std'] for m in models]
colors = ['skyblue'] * (len(models) - 1) + ['orange']

plt.bar(models, means, yerr=stds, capsize=5, color=colors, alpha=0.7)
plt.xlabel('Модели')
plt.ylabel('Точность (accuracy)')
plt.title('Сравнение точности базовых моделей и стекинг-классификатора')
plt.ylim([0.75, 0.9])
plt.grid(True, alpha=0.3, axis='y')

for i, (model, mean, std) in enumerate(zip(models, means, stds)):
    plt.text(i, mean + 0.005, f'{mean:.3f}', ha='center', va='bottom', fontweight='bold')

plt.savefig('../images/stacking_model_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

rf_model = stacking_clf.named_estimators_['rf']
feature_importance = pd.DataFrame({
    'feature': X_train.columns,
    'importance': rf_model.feature_importances_
}).sort_values('importance', ascending=False).head(15)

plt.figure(figsize=(10, 8))
plt.barh(range(len(feature_importance)), feature_importance['importance'].values, color='teal')
plt.yticks(range(len(feature_importance)), feature_importance['feature'].values)
plt.xlabel('Важность признака')
plt.title('Топ-15 наиболее важных признаков (по Random Forest)')
plt.gca().invert_yaxis()
plt.grid(True, alpha=0.3, axis='x')
plt.savefig('../images/stacking_feature_importance.png', dpi=300, bbox_inches='tight')
plt.show()

train_pred = stacking_clf.predict(X_train_scaled)
train_accuracy = accuracy_score(y_train, train_pred)

print(f"\nТочность на обучающем наборе: {train_accuracy:.4f}")
print("\nРаспределение предсказаний на тестовом наборе:")
print(pd.Series(y_pred).value_counts())

print("\nВероятности предсказаний на тестовом наборе:")
proba_df = pd.DataFrame(y_pred_proba, columns=['P(Not Survived)', 'P(Survived)'])
print(proba_df.describe())

print("\n" + "="*60)
print("ВЫВОДЫ")
print("="*60)
print("1. Стекинг-классификатор объединяет предсказания нескольких базовых моделей")
print("2. Качество стекинга обычно выше или равно качеству лучшей базовой модели")
print(f"3. Лучшая базовая модель (gbm): {model_scores['gbm']['mean']:.4f}")
print(f"4. Стекинг: {model_scores['stacking']['mean']:.4f}")
print("5. Важнейшие признаки: пол (Sex), класс (Pclass), возраст (Age), размер семьи")
print("6. Предсказания сохранены в формате, готовом для Kaggle")
print("\nГрафики сохранены в директорию ../images/")
