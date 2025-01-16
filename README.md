# Embedded Systems

**CAN**

Der MCP2515 CAN-Controller verfügt über verschiedene Betriebsmodi:

**15.1 Betriebsmodi des MCP2515 CAN-Controllers**

- **Normal Mode**: Der Standardmodus, in dem der Controller Daten sowohl senden als auch empfangen kann.
- **Sleep Mode**: Ein stromsparender Modus, bei dem der Controller in einen Ruhezustand versetzt wird, um Energie zu sparen.
- **Loopback Mode**: Ein Testmodus, bei dem gesendete Nachrichten intern zurückgeleitet werden, um die Funktionalität des Controllers zu überprüfen.
- **Listen-Only Mode**: Ein rein passiver Modus, in dem nur empfangene Nachrichten verarbeitet werden, ohne selbst Daten zu senden.
- **Configuration Mode**: Dieser Modus wird verwendet, um den MCP2515 einzurichten, bevor die Kommunikation beginnt. Dabei können Parameter wie Bitrate, Filter und Masken konfiguriert werden.

**15.2 Loopback Device**  
Ein Loopback Device bezeichnet einen virtuellen Netzwerkadapter, der für Testzwecke eingesetzt wird. Damit kann ein simuliertes Netzwerk erstellt werden, das Softwaretests ohne physische Netzwerkanbindung ermöglicht.

**Loopback-Modus**  
Der Loopback-Modus ermöglicht es, gesendete CAN-Nachrichten direkt auf der Empfängerseite desselben Geräts zu überprüfen. Dies stellt sicher, dass die grundlegende Kommunikation innerhalb des Treibers korrekt funktioniert, ohne dass externe Hardware benötigt wird.

**Codebeispiel für den Raspberry Pi Pico**  
Das folgende Codebeispiel zeigt die Initialisierung des CAN-Treibers und die Durchführung eines Tests im Loopback-Modus:

```c
/* CAN initialisieren */
can_init(CAN_LOOPBACK);
/* Nachricht senden und empfangen */
can_send_message("Hello CAN");
if (can_receive_message(buffer)) {
    printf("Empfangen: %s", buffer);
}
```

**LED-Ansteuerung und Serial Monitor**  
Im Testaufbau erfolgt die LED-Steuerung über empfangene CAN-Nachrichten, während Debugging-Informationen auf dem Serial Monitor angezeigt werden. Dies bietet sowohl visuelle als auch textbasierte Möglichkeiten zur Überprüfung der Funktionalität.

**Kommunikationstest mit erweiterten Tools**  
Zur Validierung der CAN-Kommunikation können Werkzeuge wie ein Logikanalysator und der Natural Mode verwendet werden.

**Lociganalyzer: Überwachung und Visualisierung**  
Ein Lociganalyzer wird eingesetzt, um die CAN-Kommunikation zu überwachen und in Echtzeit darzustellen. Er ermöglicht detaillierte Einblicke in die gesendeten und empfangenen Datenpakete.

**Testkonfiguration für den Logikanalysator:**

- Baudrate: 500 kbit/s
- CAN-ID-Filter: Alle IDs werden zugelassen (keine Filterung).

**Natural Mode: Ungefilterte Analyse**  
Im Natural Mode wird die CAN-Kommunikation direkt und ohne Filter analysiert. Dies erlaubt eine präzise Fehlersuche, da alle Nachrichten vollständig sichtbar sind. Der Modus ist besonders hilfreich, um Probleme in der Echtzeitkommunikation zu diagnostizieren und zu beheben.

**Beispielausgabe im Serial Monitor**  
Ein typisches Ergebnis der CAN-Kommunikation wird im Serial Monitor angezeigt, um empfangene Nachrichten darzustellen. Ein Beispiel:

```
CAN Message Received:
ID: 0x123
Data: 0x01 0x02 0x03
```

Hier ist die vollständige und ausführliche Dokumentation, die auf deine Anforderungen eingeht, inklusive Platzhalter für Bilder und Logic Analyzer-Ausgabe:

#### **"9.1 Embedded Systems" Kommunikationschnittstellen**

Der MCP2515 CAN-Controller stellt verschiedene Kommunikationsschnittstellen bereit, die die Datenübertragung in Embedded-Systemen ermöglichen. Dazu gehören:

- **SPI (Serial Peripheral Interface)**: Ermöglicht die Kommunikation zwischen dem MCP2515 und Mikrocontrollern wie dem Raspberry Pi Pico.
- **CAN-Bus**: Überträgt Datenpakete zwischen Geräten, typischerweise in Anwendungen wie Fahrzeugnetzwerken oder Industrieautomatisierung.

Einsatzgebiete:

- CAN-Bus wird häufig in Fahrzeugen verwendet, um Steuergeräte wie Motor- und Bremssysteme miteinander zu verbinden.
- SPI dient der Verbindung zwischen Mikrocontroller und Peripheriegeräten, wie z. B. Sensoren oder Kommunikationsmodulen.

**Gegenüberstellung von SPI und CAN-Bus**:

- **SPI (Serial Peripheral Interface)**:
  - Geschwindigkeit: Sehr schnell, bis zu mehreren MHz.
  - Anzahl der Geräte: Unterstützt mehrere Geräte, erfordert jedoch separate CS-Leitungen.
  - Einsatzgebiete: Sensoren, Displays, SD-Karten.
- **CAN-Bus**:
  - Geschwindigkeit: Bis zu 1 Mbps.
  - Anzahl der Geräte: Unterstützt viele Geräte ohne zusätzliche Leitungen.
  - Einsatzgebiete: Fahrzeugnetzwerke, Industrieautomatisierung.

#### "9.1Embedded Systems" Mikrocontroller Initialisierung

Der Raspberry Pi Pico wurde als Mikrocontroller für die Kommunikation mit dem MCP2515 CAN-Controller eingesetzt. Die Initialisierung umfasst:

1. **Hardware-Verkabelung**:
   - Der MCP2515 wurde über SPI mit dem Raspberry Pi Pico verbunden.
   - Power- und Ground-Verbindungen wurden hergestellt.
   - Weitere Signalleitungen wie SCK, MOSI, MISO und CS wurden korrekt verbunden. 


1. **Software-Konfiguration**:
   - Der Mikrocontroller wurde mit der notwendigen Firmware versehen.
   - Die CAN-Kommunikation wurde im Loopback Mode initialisiert, um die Funktionalität zu überprüfen. Dafür drückt man bevor man ihm anschließt 15 Sekunden auf den Knopf.

#### **Mögliche Probleme und Lösungen**:

- **Problem**: Keine Verbindung zwischen Mikrocontroller und MCP2515.
  - **Lösung**: Überprüfung der SPI-Leitungen und korrekten Baudrate.
- **Problem**: Datenverlust bei der Übertragung.
  - **Lösung**: Anpassung der Puffergröße und Nutzung eines Logikanalysators.


### **Logic Analyzer**

Ein **Logic Analyzer** ist ein Werkzeug zur Analyse digitaler Signale. Er erfasst und visualisiert die Zustände von digitalen Signalen (HIGH und LOW) über die Zeit. Dies ist besonders nützlich, um Datenkommunikation, Timing und Fehler in digitalen Schaltungen zu analysieren.

### **Hauptfunktionen**

1. **Signalaufzeichnung**
   - Überwacht und speichert digitale Signale von mehreren Kanälen gleichzeitig.
2. **Analyse und Visualisierung**
   - Zeigt Signalverläufe in einer zeitlichen Abfolge, um Protokolle wie SPI, I2C oder UART zu analysieren.
3. **Trigger-Funktion**
   - Ermöglicht das gezielte Aufzeichnen von Signalen bei bestimmten Ereignissen, z. B. bei einem Wechsel von LOW zu HIGH.

### **Einsatzgebiete**

- Debugging von digitalen Schaltungen.
- Analyse von Kommunikationsprotokollen (z. B. SPI, I2C, CAN).
- Prüfung und Fehlersuche in Mikrocontroller-Schaltungen.

### **Vorteile**

- Einfache Visualisierung digitaler Signale.
- Effiziente Fehlersuche in Echtzeitkommunikation.
- Unterstützt komplexe Protokollanalysen.

### **Nachteile**

- Kann nur digitale Signale erfassen (keine analogen Daten).
- Je nach Modell kann die Kanalkapazität begrenzt sein.

[Logic Analyzer &#8226; Wolles Elektronikkiste](https://wolles-elektronikkiste.de/logic-analyzer#:~:text=Kurz%20gesagt%3A%20Ein%20Logic%20Analyzer,Zust%C3%A4nde%20HIGH%20und%20LOW%20kennt.)
