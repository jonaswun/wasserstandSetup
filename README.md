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
Grafana Empfängt die Daten und berechnet diese auf Grundlage folgender Messwerte:


| Wasserhöhe in cm | Wasserstand in Prozent |
|------------------|------------------------|
| 9.5              | 72.1                   |
| 9.5              | 71.5                   |
| 10.0             | 71.0                   |
| 10.5             | 70.5                   |
| 10.5             | 70.8                   |
| 11.0             | 69.5                   |
| 11.5             | 69.0                   |
| 12.0             | 68.5                   |
| 12.5             | 68.0                   |

Daraus ergeben sich folgende Formeln:

Höhe = (Prozent*(-1))+81
Meereshöhe = 961.68 - (Höhe*0.01)

