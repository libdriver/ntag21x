[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTAG21X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntag21x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NTAG 213, NTAG 215 및 NTAG 216은 NXP® Semiconductors에서 NFC 장치 또는 NFC 호환 근접 커플링과 함께 소매, 게임, 소비자 전자 제품과 같은 대중 시장 애플리케이션에 사용하기 위해 표준 NFC 태그 IC로 개발했습니다. 장치. NTAG 213, NTAG 215 및 NTAG 216(지금부터 일반적으로 NTAG 21x라고 함)은 NFC 포럼 유형 2 태그 및 ISO/IEC14443 유형 A 사양을 완전히 준수하도록 설계되었습니다. 대상 응용 프로그램에는 가정 외 및 인쇄 미디어 스마트가 포함됩니다. 광고, SoLoMo 애플리케이션, 제품 인증, NFC 선반 라벨 및 모바일 컴패니언 태그. 대상 사용 사례에는 집 밖의 스마트 광고, 제품 인증, 모바일 컴패니언 태그, Bluetooth® 또는 Wi-Fi 페어링, 전자 선반 라벨 및 비즈니스가 포함됩니다. 카드. NTAG 21x 메모리는 동시에 여러 애플리케이션을 구현하도록 분할할 수도 있습니다. 높은 입력 정전 용량 덕분에 NTAG 21x 태그 IC는 성능 저하 없이 작은 설치 공간이 필요한 애플리케이션에 특히 적합합니다. 소형 NFC 태그는 예를 들어 제품 라벨이나 전자 장치에 더 쉽게 삽입할 수 있습니다. NTAG 21x의 기계적 및 전기적 사양은 인레이 및 태그 제조업체의 요구 사항을 충족하도록 맞춤화되었습니다.

LibDriver NTAG21X는 LibDriver에서 출시한 NTAG21X의 전체 기능 드라이버입니다. 페이지 읽기, 페이지 쓰기 등의 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

이 리포지토리에는 하위 모듈이 포함되어 있으며 전체 프로젝트를 복제하려면 "--recursive"를 추가해야 합니다.

```shell
git clone https://github.com/libdriver/ntag21x.git --recursive
```

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver NTAG21X의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver NTAG21X용 플랫폼 독립적인 RF 버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver NTAG21X드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver NTAG21X프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver NTAG21X오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 NTAG21X데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 RF 버스 템플릿을 참조하여 지정된 플랫폼에 대한 RF 버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ntag21x/index.html](https://www.libdriver.com/docs/ntag21x/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.