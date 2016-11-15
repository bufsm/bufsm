# Server

## Build docker image
```
docker build -t bufsm .
```

## Run docker image
```
docker run --name bufsm -v ${PWD}:/home/flask -e VIRTUAL_HOST=bufsm.dalmago.xyz -e VIRTUAL_PORT=5000 -d bufsm
```
