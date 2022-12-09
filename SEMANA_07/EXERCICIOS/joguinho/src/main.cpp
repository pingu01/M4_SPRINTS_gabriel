#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Wire.h"

using namespace std;

const char *ssid = "pegavirus";
const char *password = "12341234";

// pinos
#define LED1 15 

int ordem[4] = {1, 2, 3, 4}; // vetor com a ordem dos jogadors

AsyncWebServer web_server(80); // cria o servidor web

// botoes do menu
String botaoMenu(int classe, int numero);

// função que retorna um botão padrão para as outras páginas que depende da porta
String botaoMagistrado(int port, int alvo, int jogador);

// head html
String head();

// função que retorna o html do menu
String menuHtml();

// funções que retornam o html do assassino, do detetive e da vítima respectivamente e leva o alvo de suas ações para as outras funções
String impostorHtml();
String magistradoHtml(int alvo);
String mascaraHtml(int port);


void expulsar(int alvo);  // magistrado pode expulsar um jogador

// setup void
void setup()
{
    Serial.begin(9600); // inicia o serial

    Wire.begin(47, 48); // 47-> LCA, 48 -> LCL

    // define o pino do led
    pinMode(LED1, OUTPUT);
    digitalWrite(LED1, HIGH);


    // tenta conectar ao wifi solicitado até conseguir
    Serial.println("Connecting to Wifi.. ");
    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        delay(800);
        Serial.print(".");
    }

    // página do menu
    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", menuHtml()); });

    // página do impostor
    web_server.on("/81", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", impostorHtml()); });

    // página do magistrado
    web_server.on("/82", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", magistradoHtml(-1)); });

    // páginas das mascaras
    web_server.on("/83", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", mascaraHtml(83)); });
    web_server.on("/84", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", mascaraHtml(84)); });

    // funções do magistrado
    web_server.on("/811", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", magistradoHtml(0)); });
    web_server.on("/812", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", magistradoHtml(1)); });
    web_server.on("/813", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", magistradoHtml(2)); });
    web_server.on("/814", HTTP_GET, [](AsyncWebServerRequest *req)
                  { req->send(200, "text/html; charset=UTF-8", magistradoHtml(3)); });


    // inicia o servidor web
    web_server.begin();
}

// loop void
void loop()
{
    // printa o ip do servidor
    Serial.println(WiFi.localIP());
    
    delay(10000);
}

// retorna o botão que referencia a página padrão das portas das classes
String botaoMenu(int classe, int numero)
{
    String button = "<a class=\"botao\" href=\"8";
    button.concat(String(classe));
    button.concat("\" style=\" background-color: yellow;");
    button.concat("color : black;");
    button.concat("\"> Jogador ");
    button.concat(String(numero + 1));
    button.concat("</a><br/>");
    return button;
}

// constroi o botão padrão para as páginas do magistrado
String botaoMagistrado(int port, int alvo, int jogador)
{
    String label = "Expulsar jogador ";
    expulsar(alvo); // expulsa o jogador

    // construção da tag <a></a>
    String button = "<a class=\"botao\" href=\"";
    button.concat(String(port));
    button.concat(String(jogador + 1));
    button.concat("\"><button class=\"botao\" style=\"background-color: yellow ");
    button.concat("; color: black");
    button.concat(";\">");
    button.concat(label);
    button.concat(String(jogador + 1));
    button.concat("</button>");
    button.concat("</a>");
    return button;
}
// retorna o head do html
String head()
{
    String head = "";
    head.concat("<head>");
    head.concat("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");

    head.concat("<style>");

    head.concat("body{ text-align: center; font-family: Lucida Console; font-size: 17px; } ");

    head.concat("p{ color: #555; font-size: 14px; text-color: black; } ");

    head.concat("a{text-decoration: none;} ");

    head.concat(".botao{");
    head.concat("color: #fff;");
    head.concat("text-color: black;");
    head.concat("outline: none;");
    head.concat("display: flex;");
    head.concat("align-items: center;");
    head.concat("justify-content: center;");
    head.concat("border: 1px solid #555;");
    head.concat("border-radius:20px;");
    head.concat("width: 150px;");
    head.concat("height: 40px;");
    head.concat("margin: 11px;");
    head.concat("margin-left: auto;");
    head.concat("margin-right: auto;");
    head.concat("cursor: pointer; } ");

    head.concat("h1 { text-align: center; } ");

    head.concat("</style>");
    head.concat("</head>");
    return head;
}

String menuHtml()
{
    String html = "";
    html = head();
    html.concat("<body>");
    html.concat("<h1> Jogo seita das mascaras </h1><br/>");
    html.concat("<p> O equilíbrio deve ser prezado pela seita das mascaras, e algo afetou o equilíbrio. Porém um impostor (que não tem informações), tentou invadir a seita das máscaras, a função do magistrado é descobrir quem é esse impostor por meio de perguntas que não revelem o que afetou o equilíbrio, as máscaras devem provar que são máscaras, e o impostor deve tentar enganar a seita. </p>");

    for (int i = 0; i < 4; i++)
    {
        html.concat(botaoMenu(ordem[i], i));
    }

    html.concat("</body>");
    html.concat("</html>");
    return html;
}

// retorna o html da página de ações
String magistradoHtml(int alvo)
{
    String html = "";
    html = head();
    html.concat("<body>");
    html.concat("<h1> Você é o magistrado, o que afetou o equilíbrio foi: </h1> <div> <a href=\"/\"> Voltar </a> </div> <br/> </div>");
    html.concat("<p> Uma adaga amaldiçoada </p>");
    html.concat("<p> Descubra quem é o impostor </p>");

    for (int i = 0; i < 4; i++)
    {
        html.concat(botaoMagistrado(81, alvo, i));
    }

    html.concat("</body></html>");

    return html;
}

String impostorHtml()
{
    String html = "";
    html = head();
    html.concat("<body>");
    html.concat("<div>");
    html.concat("<h1> Você é o Impostor, se defenda e não deixe que descubram </h1> <div> <a href=\"/\"> Voltar </a> </div> <br/> </div>");
    html.concat("</body></html>");
    return html;
}

String mascaraHtml(int port)
{
    String html = "";
    html = head();
    html.concat("<body>");
    html.concat("<h1> Você é uma Mascara, o que afetou o equilíbrio foi: </h1> <div> <a href=\"/\"> Voltar </a> </div> <br/> </div>");
    html.concat("<p> Uma adaga amaldiçoada </p>");
    html.concat("<p> Se defenda e ajude o magistrado a descobrir quem foi é o impostor (sem dizer o que afetou o equilíbrio) </p>");
    html.concat("</body></html>");

    return html;
}


//expulsa um jogador da seita das mascaras
void expulsar(int alvo)
{
    // se o alvo for o impostor, a luz referenciada pisca
    if (alvo == 0)
    {
        digitalWrite(LED1, LOW);
        delay(500);
        digitalWrite(LED1, HIGH);
        delay(500);
        digitalWrite(LED1, LOW);
        delay(500);
        digitalWrite(LED1, HIGH);
        delay(500);

    }

}

