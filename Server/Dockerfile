FROM debian:jessie

RUN apt-get update && apt-get -y install python-pip python-dev libsqlite3-dev vim \
	&& rm -rf /var/lib/apt/lists/* 

WORKDIR /home/flask

ENTRYPOINT pip install -r requirements.txt && python app.py

EXPOSE 5000
