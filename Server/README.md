# Server

## Run docker image
```
docker run --name bufsm -v /home/bufsm/Server:/home/flask -d bufsm
```


## To Generate Image:
```
docker stop bufsm # stop container
docker rm bufsm # delete container
docker rmi bufsm # remove image
docker build -t bufsm .
```

## To debug RUNNING container:
```
docker exec -i -t bufsm /bin/bash
```
