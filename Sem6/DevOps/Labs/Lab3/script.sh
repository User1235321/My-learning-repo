#!/bin/bash

gnome-terminal -- bash -c "
ssh -t debian@192.168.99.100 '
JENKINS_URL=\"http://localhost:82\"
JENKINS_USER=\"admin\"
JENKINS_TOKEN=\"116447145902c5de99c8552571da25d574\"
JOB_NAME=\"my-cpp-app\"
REGISTRY_URL=\"192.168.99.100:5050\"
REGISTRY_USER=\"admin\"
REGISTRY_PASS=\"pass\"
IMAGE_NAME=\"adminforg/my-cpp-app\"

echo \"=== Демонстрация лабораторной работы №3 ===\"

echo -e \"\n1. Проверка Jenkins\"
HTTP_CODE=\$(curl -s -o /dev/null -w \"%{http_code}\" -u \${JENKINS_USER}:\${JENKINS_TOKEN} \${JENKINS_URL}/api/json)
if [ \"\$HTTP_CODE\" != \"200\" ]; then
    echo \"Jenkins недоступен (HTTP \$HTTP_CODE)\"
    exit 1
fi
echo \"Jenkins доступен\"

echo -e \"\n2. Проверка существования задачи\"
JOB_URL=\"\${JENKINS_URL}/job/\${JOB_NAME}/api/json\"
HTTP_CODE=\$(curl -s -o /dev/null -w \"%{http_code}\" -u \${JENKINS_USER}:\${JENKINS_TOKEN} \${JOB_URL})
if [ \"\$HTTP_CODE\" != \"200\" ]; then
    echo \"Задача \${JOB_NAME} не существует (HTTP \$HTTP_CODE)\"
    exit 1
fi
echo \"Задача найдена\"

echo -e \"\n3. Запуск сборки\"
BUILD_URL=\"\${JENKINS_URL}/job/\${JOB_NAME}/build\"
HTTP_CODE=\$(curl -s -X POST -u \${JENKINS_USER}:\${JENKINS_TOKEN} -H \"Content-Type: application/x-www-form-urlencoded\" -d \"\" \"\${BUILD_URL}\" --write-out \"%{http_code}\" --output /dev/null)
if [ \"\$HTTP_CODE\" != \"201\" ] && [ \"\$HTTP_CODE\" != \"200\" ]; then
    echo \"Ошибка запуска сборки (HTTP \$HTTP_CODE)\"
    exit 1
fi
echo \"Сборка запущена\"

QUEUED_ITEM_URL=\$(curl -s -u \${JENKINS_USER}:\${JENKINS_TOKEN} \"\${JENKINS_URL}/job/\${JOB_NAME}/api/json\" | jq -r '.nextBuildNumber')
if [ \"\$QUEUED_ITEM_URL\" = \"null\" ] || [ -z \"\$QUEUED_ITEM_URL\" ]; then
    echo \"Не удалось определить номер сборки\"
    exit 1
fi
BUILD_NUMBER=\$((QUEUED_ITEM_URL - 1))
if [ \$BUILD_NUMBER -eq 0 ]; then
    BUILD_NUMBER=1
fi
echo \"Номер сборки: \${BUILD_NUMBER}\"

BUILD_DETAILS_URL=\"\${JENKINS_URL}/job/\${JOB_NAME}/\${BUILD_NUMBER}/api/json\"
echo -n \"Ожидание завершения сборки\"
while true; do
    BUILD_STATUS=\$(curl -s -u \${JENKINS_USER}:\${JENKINS_TOKEN} \${BUILD_DETAILS_URL} | jq -r '.building')
    if [ \"\$BUILD_STATUS\" = \"false\" ]; then
        break
    fi
    echo -n \".\"
    sleep 2
done
echo \"\"

RESULT=\$(curl -s -u \${JENKINS_USER}:\${JENKINS_TOKEN} \${BUILD_DETAILS_URL} | jq -r '.result')
if [ \"\$RESULT\" != \"SUCCESS\" ]; then
    echo \"Сборка завершилась с ошибкой: \$RESULT\"
    exit 1
fi
echo \"Сборка успешна\"

echo -e \"\n4. Лог сборки\"
LOG_URL=\"\${JENKINS_URL}/job/\${JOB_NAME}/\${BUILD_NUMBER}/consoleText\"
curl -s -u \${JENKINS_USER}:\${JENKINS_TOKEN} \${LOG_URL}

echo -e \"\n5. Проверка Registry\"
REPO_URL=\"http://\${REGISTRY_URL}/v2/\${IMAGE_NAME}/tags/list\"
HTTP_CODE=\$(curl -s -o /dev/null -w \"%{http_code}\" -u \${REGISTRY_USER}:\${REGISTRY_PASS} \${REPO_URL})
if [ \"\$HTTP_CODE\" != \"200\" ]; then
    echo \"Образ не найден в Registry (HTTP \$HTTP_CODE)\"
    exit 1
fi
TAGS=\$(curl -s -u \${REGISTRY_USER}:\${REGISTRY_PASS} \${REPO_URL} | jq -r '.tags[]')
echo \"Образ найден. Теги: \${TAGS}\"

echo -e \"\n6. Запуск контейнера из образа\"
docker login -u \${REGISTRY_USER} -p \${REGISTRY_PASS} \${REGISTRY_URL} > /dev/null 2>&1
docker pull \${REGISTRY_URL}/\${IMAGE_NAME}:latest
docker run -d -p 8081:8080 --name demo-lab3 \${REGISTRY_URL}/\${IMAGE_NAME}:latest
sleep 2
RESPONSE=\$(curl -s http://localhost:8081)
if [ \"\$RESPONSE\" = \"Hello from C++ container!\" ]; then
    echo \"Контейнер работает корректно\"
else
    echo \"Контейнер не отвечает\"
    docker logs demo-lab3
fi
docker stop demo-lab3 > /dev/null 2>&1
docker rm demo-lab3 > /dev/null 2>&1

echo -e \"\nДемонстрация завершена\"
echo -e \"Нажмите Enter для закрытия окна...\"
read
' ; exec bash
"
