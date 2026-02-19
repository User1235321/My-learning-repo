import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import (accuracy_score, confusion_matrix,
                             roc_curve, auc, precision_recall_curve, f1_score,
                             roc_auc_score)
import os

train_path = '../data/bank_scoring_train.csv'
test_path = '../data/bank_scoring_test.csv'

train = pd.read_csv(train_path, sep='\t')
test = pd.read_csv(test_path, sep='\t')

X_train = train.drop('SeriousDlqin2yrs', axis=1)
y_train = train['SeriousDlqin2yrs']
X_test = test.drop('SeriousDlqin2yrs', axis=1)
y_test = test['SeriousDlqin2yrs']

def handle_missing(df):
    df_clean = df.copy()
    df_clean['MonthlyIncome'] = df_clean['MonthlyIncome'].replace(0, np.nan)
    df_clean['MonthlyIncome'] = df_clean['MonthlyIncome'].fillna(df_clean['MonthlyIncome'].median())
    df_clean['NumberOfDependents'] = df_clean['NumberOfDependents'].fillna(0)
    return df_clean

X_train = handle_missing(X_train)
X_test = handle_missing(X_test)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

log_reg = LogisticRegression(class_weight='balanced', max_iter=1000, random_state=42)
log_reg.fit(X_train_scaled, y_train)

rf = RandomForestClassifier(class_weight='balanced', n_estimators=100, random_state=42)
rf.fit(X_train, y_train)

models = {'Logistic Regression': log_reg, 'Random Forest': rf}
results = {}

os.makedirs('../images', exist_ok=True)

for name, model in models.items():
    if name == 'Logistic Regression':
        y_pred = model.predict(X_test_scaled)
        y_prob = model.predict_proba(X_test_scaled)[:, 1]
    else:
        y_pred = model.predict(X_test)
        y_prob = model.predict_proba(X_test)[:, 1]

    acc = accuracy_score(y_test, y_pred)
    f1 = f1_score(y_test, y_pred)
    roc_auc = roc_auc_score(y_test, y_prob)

    results[name] = {'accuracy': acc, 'f1_score': f1, 'roc_auc': roc_auc}

    cm = confusion_matrix(y_test, y_pred)
    plt.figure(figsize=(6,5))
    sns.heatmap(cm, annot=True, fmt='d', cmap='Blues')
    plt.title(f'Confusion Matrix - {name}')
    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    plt.tight_layout()
    plt.savefig(f'../images/confusion_matrix_{name.replace(" ", "_")}.png')
    plt.show()
    plt.close()

    fpr, tpr, _ = roc_curve(y_test, y_prob)
    plt.figure()
    plt.plot(fpr, tpr, label=f'ROC curve (area = {roc_auc:.2f})')
    plt.plot([0,1],[0,1],'k--')
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title(f'ROC Curve - {name}')
    plt.legend(loc='lower right')
    plt.tight_layout()
    plt.savefig(f'../images/roc_curve_{name.replace(" ", "_")}.png')
    plt.show()
    plt.close()

    precision, recall, _ = precision_recall_curve(y_test, y_prob)
    plt.figure()
    plt.plot(recall, precision)
    plt.xlabel('Recall')
    plt.ylabel('Precision')
    plt.title(f'Precision-Recall Curve - {name}')
    plt.tight_layout()
    plt.savefig(f'../images/pr_curve_{name.replace(" ", "_")}.png')
    plt.show()
    plt.close()

results_df = pd.DataFrame(results).T
print(results_df)

best_model = results_df['f1_score'].idxmax()
print(f"Best model based on F1-score for class 1: {best_model}")

with open('../images/comparison.txt', 'w') as f:
    f.write(str(results_df))
    f.write(f"\nBest model: {best_model}")

plt.figure(figsize=(8,5))
results_df[['f1_score', 'roc_auc']].plot(kind='bar')
plt.title('Model Comparison')
plt.ylabel('Score')
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('../images/model_comparison.png')
plt.show()
plt.close()
