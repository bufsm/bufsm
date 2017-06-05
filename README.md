# bUFSM [![Build Status](https://travis-ci.org/dalmago/bufsm.svg?branch=master)](https://travis-ci.org/dalmago/bufsm)
### O circular da UFSM

A aplicação para o ~~ônibus intracampus da UFSM~~ bUFSM tem por objetivo melhorar a rotina das pessoas que o utilizam, informando em tempo real ~~?~~ a localização do mesmo.

> Este projeto é desenvolvido voluntariamente por alunos da UFSM que utilizam o bUFSM diariamente, sem nenhum vínculo lucrativo. Totalmente Open Source. Feel free to contribute. Baseado no [SBU Smart Transit](http://smarttransit.cewit.stonybrook.edu/smarttransit/).

### Hardware:

O hardware em si é dividido em duas partes principais: localização e comunicação. Para satisfazer ambas, utilizou-se o módulo GPS + GSM A7, da Ai-Thinker. Um Arduino Nano também  utilizado, para controlar o módulo via comandos AT.
O esquemático pode ser encontrado [aqui](https://github.com/dalmago/bufsm/tree/master/embedded/schematic).

Pretende-se, num futuro não muito distante ~~?~~, implementar a comunicação via LoRa, uma vez que no há um custo mensal para a transmisso de dados, como o GSM.

### Estrutura:

#### Mobile:
Update:
Em vez do desenvolvimento de um app nativo, indica-se o uso de uma grande tendência para os próximos anos, que são os [Progressive Web Apps](https://developers.google.com/web/progressive-web-apps/), o que é um leque de características que fazem com que uma página Web tenha uma experiência mobile muito similar ao de um aplicativo nativo. Exemplos são encontrados [aqui](https://pwa.rocks).


