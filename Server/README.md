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

## Usage

Post Coordinates
```
curl -i -X POST -H "Content-Type: application/json" -d '{"thing":"thing_name",
        "key":"TOKEN", "lat":"31.123", "lon":"123.123"}' http://bufsm.dalmago.xyz/api/things
```
Get GEOJson:

http://bufsm.dalmago.xyz/api/list

[Front-end](http://bufsm.dalmago.xyz/map)
