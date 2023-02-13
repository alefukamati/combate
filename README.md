# <img src= "https://live.staticflickr.com/65535/52687302733_022da3c6ca_n.png" height ="35" width ="35" /> Combate 

Repositório de arquivos do squad de combate da Project Neon. O repositório está divido com relação aos projetos da equipe, sendo que atualmente a equipe possui 2 robôs na categoria _Antweight_ (454 g):
- `Doente:` robô do tipo rampa móvel, controlado por motores brushless através de locomoção tangenciada;
- `Cirrose:` robô de arma ativa do tipo _vertical spinner_ controlado por motores N20.

1 robô na categoria _Fairyweight_ (150 g): 
- `Soluço:` robô do tipo rampa móvel, controlado por motores N20.

~~E 1 robô na categoria *Beetleweight*:~~

Os arquivos referentes a eles estão separados com base nos núcleos que constituem os robôs, são eles: 
 - `Eletrônica:` onde estarão os arquivos dos circuitos desenvolvidos no KiCad para o controle dos robôs através da ESP32;
 - `Mecânica:` onde estarão os arquivos da modelagem mecânica dos robôs desenvolvidas no Fusion 360;
 - `Software:` onde estarão os códigos utilizados para o controle dos robôs, sendo subdividos nas duas formas de controle adotadas pela equipe atualmente, o controle de PS4 e o controle utilizando o protocolo ESP-NOW.
 
 Em cada diretório relacionado a esses núcleos, serão encontrados os arquivos e informações à respeito do desenvolvimento.

# Instalação
Para realizar o carregamento dos códigos nos robôs, é necessário que alguns programas estejam instalados no computador. A IDE utilizada para o desenvolvimento dos códigos é a **Arduino IDE**, que fornece uma forma bem simples de realizar o desenvolvimento dos códigos para a ESP32.
A configuração do sistema para o uso da Arduino IDE com a ESP32 pode ser feita através das seguintes etapas:
1. Primeiramente deve ser realizada a instalação do driver para o chip de conversão USB-Serial - esse driver permite a ESP32 seja reconhecida pelo sistema. O chip pode variar em relação ao modelo da ESP32 utilizado, porém os mais comuns são o [CH340](https://www.blogdarobotica.com/2020/03/21/instalando-driver-serial-para-arduinos-com-chip-ch340/) e o [CP2102](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads).
2. A segunda etapa é adicionar a ESP32 ao gerenciador de placas da Arduino IDE, isso pode ser feito diretamente pelas configurações da Arduino IDE através de algumas [etapas](https://www.fernandok.com/2018/09/instalando-esp32-no-arduino-ide-metodo.html).
