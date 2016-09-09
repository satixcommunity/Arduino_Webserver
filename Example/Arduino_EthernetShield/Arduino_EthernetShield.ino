#include "Ethernet.h"	// Benoetigen wir fuer das Ethernet Shield, ist eine Standart Arduino Library
#include "SPI.h"		// 

// Die LED, die wir ansteuern wollen.
#define LED 5

// Wir muessen eine Mac Adresse fuer die Verbidnung erstellen.
byte macAdresse[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Die IP-Adresse des Arduinos. Achtet darauf, dass die IP
// noch nicht im Netzwerk verwendet wird.
IPAddress ip{ 192, 168, 1, 25 };

// Port 80, da wir das HTTP verwenden wollen.
EthernetServer server(80);

// Der String, der spaeter das HTTP request lesen wird.
String read;

void setup() 
{
	Serial.begin(9600);

	pinMode(LED, OUTPUT);

	Ethernet.begin(macAdresse, ip);

	server.begin();

	Serial.print("Server IP: ");
	Serial.println(Ethernet.localIP());
}

void loop()
{
	// Hier erstellen wir die Moeglichkeit, als CLient
	// eine Verbindung aufzubauen.
	EthernetClient client = server.available();

	if (client)
	{
		Serial.println("new client");

		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();	// Hier lesen wir, was der client tut und geben
										// es im Seriellen Monitor aus.
				Serial.write(c);

				/*
					Wir lesen jedes Zeichen des char's, dass zuvor die Aktion des
					client gelesen hat ein.
					So das am Ende in unserem Fall folgender String entsteht: ?button-1-on
																			  ?button2-off
				*/
				if (read.length() < 50)
				{
					read += c;
				}

				if (c == '\n')
				{
					/*
						Bis zum Strich wird nur HTML geschrieben.
						Wenn du kein HTML kannst und es gerner lernen moechtest
						schau doch einfach hier vorbei: http://www.w3schools.com/html/default.asp
					*/
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					client.println("<head>");
					client.println("<link rel='stylesheet' type='text/css' href='http://felix.satixcommunity.eu/stylesheet.css'/>");
					client.println("<title>Satix Community</title>");
					client.println("<img id='banner' src='http://felix.satixcommunity.eu/fb_cover.png' alt='SatixCommunityBanner'/>");
					client.println("</head>");
					client.println("<body>");
					client.println("<h1>Arduino mit Ethernet Shield</h1>");
					client.println("<hr />");
					client.println("<div>");
					client.println("<a class='button' href=\"/?button-1-on\"\">High</a>");
					client.println("<a class='button' href=\"/?button-1-off\"\">Low</a>");
					client.println("</div>");
					client.println("<br />");
					client.println("<p>www.satixcommunity.de</p>");
					client.println("</body>");
					client.println("</html>");
					/*
						-----------------------------------------
					*/

					delay(1);

					client.stop();

					// Wenn jetzt ein Button gedrueckt wird,
					// wird der Befehl ausgelesen und die LED geht an/aus. 
					if (read.indexOf("?button-1-on") > 0)
					{
						digitalWrite(LED, HIGH);
					}
					if (read.indexOf("?button-1-off") > 0)
					{
						digitalWrite(LED, LOW);
					}

					// Hier leeren wir unseren String, damit wir neue Befehle aufnhemen koennen.
					read = "";
				}
			}
		}
	}
}