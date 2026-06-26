gnome-terminal -- bash -c "
ssh -t debian@192.168.99.100 '
cd /home/debian/Lab2/3
docker stop http-server
docker stop my-server

echo -e \"\033[32mTask 3\033[0m\"

echo -e \"\n\n\033[1mdocker build -t my-http-server .\033[0m\"
docker build -t my-http-server .
sleep 1

echo -e \"\n\n\033[1mdocker run -d -p 8080:8080 --name my-server my-http-server\033[0m\"
docker run -d -p 8080:8080 --name my-server my-http-server
sleep 1

echo -e \"\n\n\033[1mdocker logs my-server\033[0m\"
docker logs my-server
sleep 1

echo -e \"\n\n\033[1mcurl http://localhost:8080\033[0m\"
curl http://localhost:8080
sleep 1

echo -e \"\n\n\033[1mdocker stop my-server\033[0m\"
docker stop my-server
sleep 1

echo -e \"\n\n\033[1mdocker rm my-server\033[0m\"
docker rm my-server
sleep 1



echo -e \"\033[32mTask 4\033[0m\"
cd ../4/

echo -e \"\n\n\033[1mdocker compose up -d --build\033[0m\"
docker compose up -d --build
sleep 1

echo -e \"\n\n\033[1mdocker compose ps\033[0m\"
docker compose ps
sleep 1

echo -e \"\n\n\033[1mdocker logs timer\033[0m\"
docker logs timer
sleep 10

echo -e \"\n\n\033[1mdocker logs http-server\033[0m\"
docker logs http-server
sleep 1

echo -e \"\n\n\033[1mtimeout 10s docker logs -f timer\033[0m\"
timeout 10s docker logs -f timer
sleep 1

exec bash
'
"
