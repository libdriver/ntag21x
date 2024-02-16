[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTAG21X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntag21x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

NTAG 213, NTAG 215 und NTAG 216 wurden von NXP® Semiconductors als Standard-NFC-Tag-ICs entwickelt, die in Massenmarktanwendungen wie Einzelhandel, Spiele und Unterhaltungselektronik in Kombination mit NFC-Geräten oder NFC-kompatibler Näherungskopplung verwendet werden sollen Geräte. NTAG 213, NTAG 215 und NTAG 216 (von nun an allgemein als NTAG 21x bezeichnet) sind so konzipiert, dass sie den NFC-Forum-Typ-2-Tag- und ISO/IEC14443-Typ-A-Spezifikationen vollständig entsprechen. Zu den Zielanwendungen gehören Out-of-Home- und Printmedien-Smart Werbung, SoLoMo-Anwendungen, Produktauthentifizierung, NFC-Regaletiketten und mobile Begleiter-Tags. Zielanwendungsfälle umfassen intelligente Out-of-Home-Werbung, Produktauthentifizierung, mobile Begleiter-Tags, Bluetooth®- oder Wi-Fi-Kopplung, elektronische Regaletiketten und Unternehmen Karten. Der NTAG 21x-Speicher kann auch segmentiert werden, um mehrere Anwendungen gleichzeitig zu implementieren. Dank der hohen Eingangskapazität sind NTAG 21x-Tag-ICs besonders auf Anwendungen zugeschnitten, die einen geringen Platzbedarf erfordern, ohne Kompromisse bei der Leistung einzugehen. Kleine NFC-Tags lassen sich beispielsweise einfacher in Produktetiketten oder elektronische Geräte einbetten. Die mechanischen und elektrischen Spezifikationen des NTAG 21x sind auf die Anforderungen von Inlay- und Tag-Herstellern zugeschnitten.

LibDriver NTAG21X ist der voll funktionsfähige Treiber von NTAG21X, der von LibDriver gestartet wurde. Er bietet die Funktion zum Lesen von Seiten, Schreiben von Seiten usw. LibDriver ist MISRA-konform.

Dieses Repository enthält ein Submodul und Sie sollten "--recursive" hinzufügen, um das gesamte Projekt zu klonen.

```shell
git clone https://github.com/libdriver/ntag21x.git --recursive
```

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver NTAG21X-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver NTAG21X RF.

/test enthält den Testcode des LibDriver NTAG21X-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver NTAG21X-Beispielcode.

/doc enthält das LibDriver NTAG21X-Offlinedokument.

/Datenblatt enthält NTAG21X-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige RF-Schnittstellenvorlage und stellen Sie Ihren Plattform-RF-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_ntag21x_basic.h"

uint8_t res;
uint8_t i;
uint8_t id[8];
ntag21x_capability_container_t type;
ntag21x_version_t version;
uint32_t cnt;
uint8_t signature[32];
uint8_t number[7];
uint8_t data[16];
uint16_t len;
uint8_t pwd[4];
uint8_t pack[2];

/* init */
res = ntag21x_basic_init();
if (res != 0)
{
    return 1;
}

/* search */
res = ntag21x_basic_search(&type, id, 50);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}

/* ouput the type */
if (type == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
{
    ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
}
else if (type == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
{
    ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
}
else if (type == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
{
    ntag21x_interface_debug_print("ntag21x: find ntag216 card.\n");
}
else
{
    ntag21x_interface_debug_print("ntag21x: invalid type.\n");
}
ntag21x_interface_debug_print("ntag21x: id is ");
for (i = 0; i < 8; i++)
{
    ntag21x_interface_debug_print("0x%02X ", id[i]);
}
ntag21x_interface_debug_print("\n");

...
    
/* get the version */
res = ntag21x_basic_get_version(&version);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: fixed_header is 0x%02X\n", version.fixed_header);
ntag21x_interface_debug_print("ntag21x: vendor_id is 0x%02X\n", version.vendor_id);
ntag21x_interface_debug_print("ntag21x: product_type is 0x%02X\n", version.product_type);
ntag21x_interface_debug_print("ntag21x: product_subtype is 0x%02X\n", version.product_subtype);
ntag21x_interface_debug_print("ntag21x: major_product_version is 0x%02X\n", version.major_product_version);
ntag21x_interface_debug_print("ntag21x: minor_product_version is 0x%02X\n", version.minor_product_version);
ntag21x_interface_debug_print("ntag21x: storage_size is 0x%02X\n", version.storage_size);
ntag21x_interface_debug_print("ntag21x: protocol_type is 0x%02X\n", version.protocol_type);

...

/* read the counter */
res = ntag21x_basic_read_counter(&cnt);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("read counter %d.\n", cnt);

...

/* read the signature */
res = ntag21x_basic_read_signature(signature);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: signature is ");
for (i = 0; i < 32; i++)
{
    ntag21x_interface_debug_print("0x%02X ", signature[i]);
}
ntag21x_interface_debug_print("\n");

...

/* get the serial number */
res = ntag21x_basic_get_serial_number(number);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: serial number is ");
for (i = 0; i < 7; i++)
{
    ntag21x_interface_debug_print("0x%02X ", id[i]);
}
ntag21x_interface_debug_print("\n");

...

/* read data */
res = ntag21x_basic_read(0, data);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: read page %d: ", 0);
for (i = 0; i < 4; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...

/* write data */
res = ntag21x_basic_write(20, data);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: write page %d: ", 20);
for (i = 0; i < 4; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...
    
/* read data */
res = ntag21x_basic_read_four_pages(0, data);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: read page %d: ", 0);
for (i = 0; i < 16; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...

/* read pages */
len = 16;
res = ntag21x_basic_read_pages(0, 0, data, &len);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: read pages %d - %d: ", 0, 0);
for (i = 0; i < len; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...

/* authenticate */
res = ntag21x_basic_authenticate(pwd, pack);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}

...
    
(void)ntag21x_basic_deinit();

return 0;
```

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ntag21x/index.html](https://www.libdriver.com/docs/ntag21x/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.