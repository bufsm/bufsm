# bUFSM [![Build Status](https://travis-ci.org/bufsm/bufsm.svg?branch=master)](https://travis-ci.org/bufsm/bufsm)
### O circular da UFSM

A aplicação para o ~~ônibus intracampus da UFSM~~ bUFSM tem por objetivo melhorar a rotina das pessoas que o utilizam, informando em tempo real ~~?~~ a localização do mesmo.

> Este projeto é desenvolvido voluntariamente por alunos da UFSM que utilizam o bUFSM diariamente, sem nenhum vínculo lucrativo. Totalmente Open Source. Feel free to contribute. Baseado no [SBU Smart Transit](http://smarttransit.cewit.stonybrook.edu/smarttransit/).

### Hardware:

O hardware em si é dividido em duas partes principais: localização e comunicação. Para satisfazer ambas, utilizou-se o módulo GPS + GSM A7, da Ai-Thinker. Um Arduino Nano também  utilizado, para controlar o módulo via comandos AT.
O esquemático pode ser encontrado [aqui](https://github.com/bufsm/bufsm/tree/master/embedded/schematic).

Pretende-se, num futuro não muito distante ~~?~~, implementar a comunicação via LoRa, uma vez que no há um custo mensal para a transmisso de dados, como o GSM.

### Estrutura:

#### Embedded:

O código embarcado no Arduino Nano comanda o módulo A7, lendo as coordenadas geográficas do ônibus e enviando-as, via protocolo MQTT, para um *broker*. Essas mensagens com as coordenadas são então replicadas para cada cliente que está com o *app* aberto.

#### Front-end:

Desenvolvido como uma página Web, segue os conceitos de um [Progressive Web App](https://developers.google.com/web/progressive-web-apps/). Este se caracteriza por ser uma página acessível de qualquer navegador em qualquer plataforma, ainda permitindo que a mesma seja "Instalada" na tela inicial de um aparelho *mobile*, agindo de forma semelhante a um aplicativo nativo.
