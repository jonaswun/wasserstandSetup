# Wasserstandsmessung Kanal
## Arduino
### Spannungsmessung
Um eine Spannung von 0-10V am Arduino zu messen wird diese an einem Spannungsteiler halbiert und an die Analogen Eingänge des Arduino geführt. Der Arduino selbst hat einen dauerhaften Offset von -0.15V. Somit ergibt sich eine Abweichung von -3%. 

### Datenübertraung
Zur Datenübertragung wird eine Serielle Schnitstelle per USB verwendet. Diese überträgt die Daten mit 9600 Baud vom Arduino zum Rasbperry Pi

## Raspberry Pi
### Datenverarbeitung:
Der Raspberry Pi empfängt die Daten und sendet diese unverändert als MQTT Client unter dem Topic "/wasserstand/percentValue" an Node Red.

## Nod Red
Node Red Empfängt die Daten und sendet diese an die Influx Datenbank "wasserstand"

## Grafana 
Grafana Empfängt die Daten und berechnet diese auf Grundlage folgender Annahmen:
Bei einem gewünschten Wasserstand von 961.55m ü. NN ergibt sich ein von der Sonde gemessener Wert von 66.7%. Da die Sonde einen Messbereich von 0-100cm besitzt ergibt sich hiermit einen wert von 66.7cm. Um somit auf die Tatsächliche Höhe des Wasserstand zu kommen werden folgende Formeln verwendet.

Höhe = (-Prozent)+79.7
Meereshöhe = 961.68 - (Höhe * 0.01)
Somit ergibt sich folgender Graph: 
[](https://github.com/jonaswun/wasserstandSetup/blob/main/Doc/WasserstandVerlauf.png)
