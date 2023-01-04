# Wasserstandsmessung und Kommunikation Kanal
Das Repository beinhaltet die alte Wasserstandmessung mittels eines "normalen" Arduinos. Im zweiten Teil wird ein [Industrial Arduino](https://www.industrialshields.com/shop/product/004001001300-plc-arduino-ardbox-dali-20-i-os-relay-hf-modbus-1924?search=ardbox+relay#attr=202,184,2507,1465,749,983,211,738,798,1724,806) verwendet 

### Wasserstandsmessung
Der Wasserstand wird über eine VEGA Wasserstandssonde gemessen. 
Dabei wird eine VEGAMET 507 Z verwendet. Diese bietet einen 0-10V Ausgang Proportional zum Wasserstand, welcher mittels des Industrial Arduinos ausgelesen wird.
### Datenübertraung
Zur Datenübertragung wird die vom Industrial Arduino angebotene RS485 Schnittstelle wervendet. Die Kommunikation basiert dabei auf selbstdefinierten String Stichwörter. Auf der Seite des Raspberry Pi sorgt ein RS485 to USB Konverter für die passende Schnittstelle.

| Stichwort   	| Bedeutung                                            	|
|-------------	|------------------------------------------------------	|
| wasserstand 	| Senden des Wasserstands an den Pi                    	|
| zylinder    	| Befehl zur Zylinderbewegung abhängig vom Wasserstand 	|
| targetValue 	| Setzten des neuen Sollwerts                          	|
| getsollwert 	| Senden des momentanen Sollwerts an den Pi            	|


### Datenverarbeitung:
Auf Seiten des Raspberry Pis wird die Kommunikation über Node Red ermöglicht. Der dafür verwendete Flow ist im [Flow File](wasserstandNodeRedFlow.json) zu finden.


Die berechnung des Tatsächlichen Wasserstand erfolgt über eine Formel in Grafana

`960.96+($A*0.01)`

`$A` bezieht sich auf den Wasserstand in Prozent
Die Leistung der Turbine wird mittels eines Leistungsmesser ermittelt. Diser verfügt über eine S0 Impulsschnittstelle, welche durch einen ESP32 ausgelesen wird. Dieser liefert die Daten über die Serielle Schnittstelle an den Raspberry Pi.

![Node Red Doku](Doc/NodeRedDoku.pdf)
## Kalibrierung Wasserstand 

