# Server

## Build docker image
```
docker build -t bufsm .
```

## Run docker image
```
docker run --name bufsm -v ${PWD}:/home/flask -p 5000:5000 bufsm
```
