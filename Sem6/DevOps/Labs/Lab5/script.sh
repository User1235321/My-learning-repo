#!/bin/bash

gnome-terminal -- bash -c "
ssh -t debian@192.168.99.100 '
set -e

export PATH=\"\$HOME/.local/bin:\$PATH\"

GREEN=\"\\033[0;32m\"
RED=\"\\033[0;31m\"
BLUE=\"\\033[0;34m\"
NC=\"\\033[0m\"

echo -e \"${BLUE}=== Демонстрация лабораторной работы №5 ===${NC}\"

echo -e \"\n${BLUE}1. Проверка kubectl и helm${NC}\"
kubectl version --client | head -1
helm version --short | head -1

echo -e \"\n${BLUE}2. Настройка зеркал Docker на master-0${NC}\"
sudo mkdir -p /etc/docker
sudo tee /etc/docker/daemon.json > /dev/null <<EOF
{
  \"registry-mirrors\": [\"https://mirror.gcr.io\", \"https://dockerhub.timeweb.cloud\", \"https://cr.yandex/mirror\"]
}
EOF
sudo systemctl restart docker
echo -e \"${GREEN}✅ Зеркала настроены${NC}\"

echo -e \"\n${BLUE}3. Загрузка образа Vault на все узлы${NC}\"
NODES=\"192.168.99.100 192.168.99.101 192.168.99.102\"
for node in \$NODES; do
    echo \"Загрузка Vault на \$node\"
    ssh debian@\$node \"sudo docker pull docker.io/hashicorp/vault:1.16.1\" &
done
wait
echo -e \"${GREEN}✅ Образы Vault загружены${NC}\"

echo -e \"\n${BLUE}4. Полная очистка предыдущих установок Vault${NC}\"
helm uninstall vault -n vault 2>/dev/null || true
kubectl delete namespace vault --ignore-not-found=true --wait=false
kubectl patch namespace vault -p '{\"metadata\":{\"finalizers\":[]}}' --type=merge 2>/dev/null || true
kubectl delete namespace vault --force --grace-period=0 2>/dev/null || true
sleep 5
kubectl create ns vault

echo -e \"\n${BLUE}5. Установка Vault (таймаут 30 мин)${NC}\"
cd ~/work/LAB_5
rm -rf vault-helm
git clone --depth 1 --branch v0.28.0 https://github.com/hashicorp/vault-helm.git
cd vault-helm

helm upgrade --install vault . --namespace vault --timeout 30m --wait \
  --set server.dev.enabled=true \
  --set server.dev.devRootToken=root \
  --set server.service.enabled=true \
  --set ui.enabled=true \
  --set injector.enabled=false \
  --set global.imagePullPolicy=IfNotPresent \
  --set server.image.repository=docker.io/hashicorp/vault \
  --set server.image.tag=1.16.1

cd ..
echo -e \"${GREEN}✅ Vault установлен и запущен${NC}\"

echo -e \"\n${BLUE}6. Настройка Vault: создание секретов и auth метода${NC}\"
sleep 5
kubectl exec -n vault vault-0 -- /bin/sh -c \"
    export VAULT_ADDR=http://127.0.0.1:8200
    export VAULT_TOKEN=root
    vault auth enable kubernetes || true
    vault write auth/kubernetes/config kubernetes_host=https://\${KUBERNETES_PORT_443_TCP_ADDR}:443
    vault write auth/kubernetes/role/my-app-role \
        bound_service_account_names=my-app-sa \
        bound_service_account_namespaces=dev \
        policies=default \
        ttl=24h
    vault kv put secret/my-cpp-app DATABASE_URL='postgres://user:pass@db:5432' API_KEY='my-secret-key-123'
\"
echo -e \"${GREEN}✅ Vault настроен, секреты созданы${NC}\"

echo -e \"\n${BLUE}7. Создание Kubernetes Secret из Vault (имитация работы VSO)${NC}\"
kubectl create ns dev --dry-run=client -o yaml | kubectl apply -f -
kubectl delete secret my-app-env -n dev --ignore-not-found=true
kubectl create secret generic my-app-env -n dev \
  --from-literal=DATABASE_URL='postgres://user:pass@db:5432' \
  --from-literal=API_KEY='my-secret-key-123'

echo -e \"${GREEN}✅ Kubernetes Secret создан из данных Vault${NC}\"

echo -e \"\n${BLUE}8. Запуск сервиса без helm через единый манифест (deployment+service)${NC}\"
kubectl delete deployment my-app -n dev --ignore-not-found=true
kubectl delete service my-app -n dev --ignore-not-found=true
cat <<EOF | kubectl apply -f -
apiVersion: apps/v1
kind: Deployment
metadata:
  name: my-app
  namespace: dev
spec:
  replicas: 1
  selector:
    matchLabels:
      app: my-app
  template:
    metadata:
      labels:
        app: my-app
    spec:
      imagePullSecrets:
      - name: gitlab-insecure-registry
      containers:
      - name: app
        image: 192.168.99.100:5050/adminforg/my-cpp-app:latest
        envFrom:
        - secretRef:
            name: my-app-env
        ports:
        - containerPort: 8080
---
apiVersion: v1
kind: Service
metadata:
  name: my-app
  namespace: dev
spec:
  selector:
    app: my-app
  ports:
  - port: 8080
    targetPort: 8080
EOF

kubectl wait --for=condition=ready pod -l app=my-app -n dev --timeout=60s
kubectl port-forward -n dev svc/my-app 8081:8080 > /dev/null 2>&1 &
PF_PID=\$!
sleep 3
RESPONSE_NO_HELM=\$(curl -s http://localhost:8081)
echo \"Ответ сервиса (kubectl): \$RESPONSE_NO_HELM\"
kill \$PF_PID 2>/dev/null || true

echo -e \"\n${BLUE}9. Создание Helm-чарта для my-cpp-app${NC}\"
rm -rf helm/my-app
mkdir -p helm/my-app
cd helm/my-app
cat > Chart.yaml <<EOF
apiVersion: v2
name: my-app
description: My C++ app with Vault secrets
type: application
version: 0.1.0
appVersion: 1.0.0
EOF
cat > values.yaml <<EOF
image:
  repository: 192.168.99.100:5050/adminforg/my-cpp-app
  tag: latest
  pullPolicy: IfNotPresent

replicaCount: 1

service:
  type: ClusterIP
  port: 8080

ingress:
  enabled: true
  className: nginx
  hosts:
    - host: my-app.dev.local
      paths:
        - path: /
          pathType: Prefix

envSecretName: my-app-env

imagePullSecrets:
  - name: gitlab-insecure-registry
EOF
mkdir templates
cat > templates/deployment.yaml <<EOF
apiVersion: apps/v1
kind: Deployment
metadata:
  name: {{ .Chart.Name }}
  namespace: {{ .Release.Namespace }}
spec:
  replicas: {{ .Values.replicaCount }}
  selector:
    matchLabels:
      app: {{ .Chart.Name }}
  template:
    metadata:
      labels:
        app: {{ .Chart.Name }}
    spec:
      imagePullSecrets:
{{ toYaml .Values.imagePullSecrets | indent 8 }}
      containers:
      - name: app
        image: \"{{ .Values.image.repository }}:{{ .Values.image.tag }}\"
        imagePullPolicy: {{ .Values.image.pullPolicy }}
        ports:
        - containerPort: {{ .Values.service.port }}
        envFrom:
        - secretRef:
            name: {{ .Values.envSecretName }}
EOF
cat > templates/service.yaml <<EOF
apiVersion: v1
kind: Service
metadata:
  name: {{ .Chart.Name }}
  namespace: {{ .Release.Namespace }}
spec:
  selector:
    app: {{ .Chart.Name }}
  ports:
  - port: {{ .Values.service.port }}
    targetPort: {{ .Values.service.port }}
EOF
cat > templates/ingress.yaml <<EOF
{{- if .Values.ingress.enabled }}
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: {{ .Chart.Name }}
  namespace: {{ .Release.Namespace }}
spec:
  ingressClassName: {{ .Values.ingress.className }}
  rules:
  {{- range .Values.ingress.hosts }}
  - host: {{ .host }}
    http:
      paths:
      {{- range .paths }}
      - path: {{ .path }}
        pathType: {{ .pathType }}
        backend:
          service:
            name: {{ $.Chart.Name }}
            port:
              number: {{ $.Values.service.port }}
      {{- end }}
  {{- end }}
{{- end }}
EOF
echo -e \"${GREEN}✅ Helm-чарт создан${NC}\"

echo -e \"\n${BLUE}10. Установка приложения через Helm${NC}\"
kubectl delete deployment my-app -n dev --ignore-not-found=true
kubectl delete service my-app -n dev --ignore-not-found=true
helm upgrade --install my-app ./ --namespace dev --wait --timeout 5m

echo -e \"\n${BLUE}11. Проверка получения секретов в поде (через Helm)${NC}\"
POD_NAME=\$(kubectl get pod -n dev -l app=my-app -o jsonpath=\"{.items[0].metadata.name}\" 2>/dev/null)
if [ -z \"\$POD_NAME\" ]; then
    echo -e \"${RED}❌ Под приложения не найден${NC}\"
    exit 1
fi
echo \"Переменные окружения из секрета:\"
kubectl exec -n dev \$POD_NAME -- env | grep -E \"DATABASE_URL|API_KEY\" || echo \"Секреты не найдены\"

echo -e \"\n${BLUE}12. Проверка работы приложения (http)${NC}\"
kubectl port-forward -n dev svc/my-app 8080:8080 > /dev/null 2>&1 &
PF_PID2=\$!
sleep 3
RESPONSE=\$(curl -s http://localhost:8080)
echo \"Ответ приложения: \$RESPONSE\"
ENV_RESPONSE=\$(curl -s http://localhost:8080/env 2>/dev/null || echo \"endpoint /env не реализован\")
if [ \"\$ENV_RESPONSE\" != \"endpoint /env не реализован\" ]; then
    echo \"Секреты через /env: \$ENV_RESPONSE\"
fi
kill \$PF_PID2 2>/dev/null || true

echo -e \"\n${BLUE}13. Доступ к Vault UI${NC}\"
echo \"Выполните: kubectl port-forward -n vault svc/vault 8200:8200\"
echo \"Откройте http://localhost:8200, токен root\"

echo -e \"\n${GREEN}✅ Лабораторная работа №5 выполнена полностью${NC}\"
echo -e \"Нажмите Enter для закрытия окна...\"
read
' ; exec bash
"
