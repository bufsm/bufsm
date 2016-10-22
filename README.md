# bUFSM
### O circular da UFSM

A aplicação para o ~~ônibus circular da UFSM~~ bUFSM tem por objetivo melhorar a rotina das pessoas que o utilizam, informando em tempo real ~~?~~ a localização do mesmo.

> Este projeto é desenvolvido voluntariamente por alunos da UFSM que utilizam o bUFSM diariamente, sem nenhum vínculo lucrativo. Totalmente Open Source. Feel free to contribute. Baseado no [SBU Smart Transit](http://smarttransit.cewit.stonybrook.edu/smarttransit/).

### Hardwares propostos:

O hardware em si é dividido em duas partes principais: localização e comunicação. Existem algumas estruturas já propostas, porém ainda se faz necessária a decisão.

- Módulo GPS/GLONASS + GPRS
- Módulo GPS/GLONASS + Gateway LoRA
- Módulo GPS/GLONASS + Módulos LoRA distribuídos pelo campus 
- Módulos LoRA distribuídos pelo campus e localização por triangulação destes
- ESP8266 (localização através do monitoramento das redes WiFi disponíveis)

### Estrutura:
Basicamente dividida em Servidor e Mobile
#### Servidor:
Aplicação Django que utiliza *serializer* com JSON para enviar os dados pro app Mobile. Dentre as opções abaixo, algumas rodam na *cloud* e algumas não. A vantagem de rodar na *cloud* é a não dependência de uma conexão de internet pessoal e não depender de uma estrutura de rede já existente. Opções propostas:

- Digital Ocean - cloud - é pago, porém pode-se rodar na conta de alguém que esteja disposto(a) a  emprestar.
- OpenShift.com - cloud - gratuito, porém mais difícil de configurar.
- Raspberry Pi - local - gratuito, fácil de executar, porém depende da conexão de internet doméstica.

### Mobile:
App desenvolvido com [React Native](http://facebook.github.io/react-native/), devido ao rápido desenvolvimento e ao mesmo ser compatível com Android e iOS. A ser disponibilizado na Google Play (limitado a contas @...ufsm.br?), porém dificilmente será disponibilizado na Apple Store (a não ser que alguém disponibilize sua conta para publicar).
