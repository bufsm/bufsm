# bUFSM
### O circular da UFSM

A aplicação para o ~~ônibus circular da UFSM~~ bUFSM tem por objetivo melhorar a rotina das pessoas que o utilizam, informando em tempo real ~~?~~ a localização do mesmo.

> Este projeto é desenvolvido voluntariamente por alunos da UFSM que utilizam o bUFSM diariamente, sem nenhum vínculo lucrativo. Totalmente Open Source. Feel free to contribute.

### Estruturas propostas:

O hardware em si é dividido em duas partes principais: localização e comunicação. Existem algumas estruturas já propostas, porém ainda se faz necessária a decisão.

- Módulo GPS/GLONASS + GPRS
- Módulo GPS/GLONASS + Gateway LoRA
- Módulo GPS/GLONASS + Módulos LoRA distribuídos pelo campus 
- Módulos LoRA distribuídos pelo campus e localização por triangulação destes
- ESP8266 (localização através do monitoramento das redes WiFi disponíveis)
