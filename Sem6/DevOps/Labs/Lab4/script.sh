#!/bin/bash

gnome-terminal -- bash -c "
ssh -t debian@192.168.99.100 '
set -e

export PATH=\"\$HOME/.local/bin:\$PATH\"

GREEN=\"\\033[0;32m\"
RED=\"\\033[0;31m\"
BLUE=\"\\033[0;34m\"
NC=\"\\033[0m\"

echo -e \"${BLUE}=== Демонстрация лабораторной работы №4 ===${NC}\"

cd ~/work/devops_course/LAB_4/deploy_scripts

echo -e \"\n${BLUE}0. Проверка Ansible (установка, если нужно)${NC}\"
if ! command -v ansible &> /dev/null; then
    echo \"Ansible не найден, устанавливаю...\"
    sudo ./scripts/install_ansible.sh
    export PATH=\"\$PATH:/home/debian/.local/bin\"
    source ~/.bashrc
else
    echo \"Ansible уже установлен\"
fi

echo -e \"\n${BLUE}1. Версия Ansible${NC}\"
ansible --version | head -1

echo -e \"\n${BLUE}2. Проверка подключения к узлам (ping)${NC}\"
make ping

echo -e \"\n${BLUE}3. Проверка kubectl${NC}\"
if ! command -v kubectl &> /dev/null; then
    echo \"${RED}kubectl не найден. Кластер Kubernetes не развернут.${NC}\"
    exit 1
fi
KUBECTL_VERSION=\$(kubectl version --client -o json | jq -r .clientVersion.gitVersion 2>/dev/null || kubectl version --client | head -1)
echo \"kubectl: \$KUBECTL_VERSION\"

echo -e \"\n${BLUE}4. Состояние узлов Kubernetes${NC}\"
kubectl get nodes -o wide

echo -e \"\n${BLUE}5. Компоненты кластера${NC}\"
kubectl get all -A | grep -E \"NAMESPACE|kube-system|metallb|ingress|dev\" | head -20

echo -e \"\n${BLUE}6. Версия Helm${NC}\"
if command -v helm &> /dev/null; then
    helm version | head -1
else
    echo \"Helm не установлен\"
fi

echo -e \"\n${BLUE}7. Ingress Controller External-IP${NC}\"
EXTERNAL_IP=\$(kubectl get svc -n ingress-nginx ingress-nginx-controller -o jsonpath=\"{.status.loadBalancer.ingress[0].ip}\" 2>/dev/null)
if [ -z \"\$EXTERNAL_IP\" ]; then
    echo \"${RED}EXTERNAL-IP не присвоен (pending)${NC}\"
    exit 1
else
    echo \"${GREEN}EXTERNAL-IP: \$EXTERNAL_IP${NC}\"
fi

echo -e \"\n${BLUE}8. Секрет для доступа к registry в namespace dev${NC}\"
if kubectl get secrets -n dev gitlab-insecure-registry &>/dev/null; then
    echo \"${GREEN}Секрет gitlab-insecure-registry найден в namespace dev${NC}\"
else
    echo \"${RED}Секрет не найден${NC}\"
fi

echo -e \"\n${BLUE}9. Загрузка образа из приватного registry (тестовый под)${NC}\"
cat <<EOF | kubectl apply -f -
apiVersion: v1
kind: Pod
metadata:
  name: test-registry-pod
  namespace: dev
spec:
  containers:
  - name: test-container
    image: 192.168.99.100:5050/adminforg/my-cpp-app:latest
    imagePullPolicy: Always
    ports:
    - containerPort: 8080
  imagePullSecrets:
  - name: gitlab-insecure-registry
EOF

echo \"Ожидание запуска пода...\"
sleep 10
POD_STATUS=\$(kubectl get pod -n dev test-registry-pod -o jsonpath=\"{.status.phase}\" 2>/dev/null)
if [ \"\$POD_STATUS\" = \"Running\" ]; then
    echo \"${GREEN}Под успешно запущен, образ загружен из registry${NC}\"
else
    echo \"${RED}Статус пода: \$POD_STATUS${NC}\"
    kubectl describe pod -n dev test-registry-pod | tail -10
fi
kubectl delete pod -n dev test-registry-pod --ignore-not-found=true

echo -e \"\n${BLUE}10. Развертывание тестового приложения для Ingress (из приватного registry)${NC}\"
cat <<EOF | kubectl apply -f -
apiVersion: apps/v1
kind: Deployment
metadata:
  name: my-app-deployment
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
      containers:
      - name: my-app
        image: 192.168.99.100:5050/adminforg/my-cpp-app:latest
        ports:
        - containerPort: 8080
      imagePullSecrets:
      - name: gitlab-insecure-registry
---
apiVersion: v1
kind: Service
metadata:
  name: my-app-service
  namespace: dev
spec:
  selector:
    app: my-app
  ports:
  - port: 80
    targetPort: 8080
---
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: test-ingress
  namespace: dev
spec:
  ingressClassName: nginx
  rules:
  - host: 192.168.99.200.nip.io
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: my-app-service
            port:
              number: 80
EOF

echo \"Ожидание запуска подов приложения...\"
sleep 15

APP_POD_STATUS=\$(kubectl get pod -n dev -l app=my-app -o jsonpath=\"{.items[0].status.phase}\" 2>/dev/null)
if [ \"\$APP_POD_STATUS\" = \"Running\" ]; then
    echo \"${GREEN}Приложение запущено${NC}\"
else
    echo \"${RED}Приложение не запустилось, статус: \$APP_POD_STATUS${NC}\"
    kubectl describe pod -n dev -l app=my-app
fi

echo -e \"\n${BLUE}11. Проверка работы Ingress через curl к http://192.168.99.200.nip.io${NC}\"
HTTP_CODE=\$(curl -s -o /dev/null -w \"%{http_code}\" http://192.168.99.200.nip.io 2>/dev/null || echo \"000\")
if [ \"\$HTTP_CODE\" -eq 200 ]; then
    RESPONSE=\$(curl -s http://192.168.99.200.nip.io)
    echo \"${GREEN}Ingress доступен, HTTP код: \$HTTP_CODE${NC}\"
    echo \"Ответ приложения: \${RESPONSE}\"
else
    echo \"${RED}Ingress не отвечает (HTTP \$HTTP_CODE)${NC}\"
    INGRESS_POD=\$(kubectl get pods -n ingress-nginx -l app.kubernetes.io/component=controller -o jsonpath=\"{.items[0].metadata.name}\" 2>/dev/null)
    if [ -n \"\$INGRESS_POD\" ]; then
        echo \"Логи Ingress Controller:\"
        kubectl logs -n ingress-nginx \$INGRESS_POD --tail=10
    fi
fi

echo -e \"\n${BLUE}12. Очистка тестового приложения и Ingress${NC}\"
kubectl delete ingress test-ingress -n dev --ignore-not-found=true
kubectl delete service my-app-service -n dev --ignore-not-found=true
kubectl delete deployment my-app-deployment -n dev --ignore-not-found=true

echo -e \"\n${GREEN}=== Демонстрация завершена ===${NC}\"
echo -e \"Нажмите Enter для закрытия окна...\"
read
' ; exec bash
"
