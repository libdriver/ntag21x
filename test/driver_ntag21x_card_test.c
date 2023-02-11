/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_ntag21x_card_test.c
 * @brief     driver ntag21x card test source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-06-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/06/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ntag21x_card_test.h"
#include <stdlib.h>

static ntag21x_handle_t gs_handle;        /**< ntag21x handle */

/**
 * @brief  card test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ntag21x_card_test(void)
{
    uint8_t res;
    uint8_t p;
    uint8_t limit_check;
    uint32_t i;
    uint32_t cnt;
    uint8_t id[4];
    uint8_t number[7];
    uint8_t signature[32];
    uint8_t data[16];
    uint8_t data_check[16];
    uint8_t page[32];
    uint8_t pack[2];
    uint8_t pwd[4];
    uint8_t lock[3];
    uint8_t lock_check[3];
    uint16_t len;
    ntag21x_info_t info;
    ntag21x_type_t type;
    ntag21x_capability_container_t container;
    ntag21x_version_t version;
    ntag21x_mirror_t mirror;
    ntag21x_mirror_byte_t byte;
    ntag21x_modulation_mode_t mode;
    ntag21x_bool_t enable;
    
    /* link functions */
    DRIVER_NTAG21X_LINK_INIT(&gs_handle, ntag21x_handle_t);
    DRIVER_NTAG21X_LINK_CONTACTLESS_INIT(&gs_handle, ntag21x_interface_contactless_init);
    DRIVER_NTAG21X_LINK_CONTACTLESS_DEINIT(&gs_handle, ntag21x_interface_contactless_deinit);
    DRIVER_NTAG21X_LINK_CONTACTLESS_TRANSCEIVER(&gs_handle, ntag21x_interface_contactless_transceiver);
    DRIVER_NTAG21X_LINK_DELAY_MS(&gs_handle, ntag21x_interface_delay_ms);
    DRIVER_NTAG21X_LINK_DEBUG_PRINT(&gs_handle, ntag21x_interface_debug_print);
    
    /* get information */
    res = ntag21x_info(&info);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        ntag21x_interface_debug_print("ntag21x: chip is %s.\n", info.chip_name);
        ntag21x_interface_debug_print("ntag21x: manufacturer is %s.\n", info.manufacturer_name);
        ntag21x_interface_debug_print("ntag21x: interface is %s.\n", info.interface);
        ntag21x_interface_debug_print("ntag21x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ntag21x_interface_debug_print("ntag21x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ntag21x_interface_debug_print("ntag21x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ntag21x_interface_debug_print("ntag21x: max current is %0.2fmA.\n", info.max_current_ma);
        ntag21x_interface_debug_print("ntag21x: max temperature is %0.1fC.\n", info.temperature_max);
        ntag21x_interface_debug_print("ntag21x: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    ntag21x_interface_debug_print("ntag21x: start card test.\n");
    
    /* init */
    res = ntag21x_init(&gs_handle);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: init failed.\n");
        
        return 1;
    }
    
    /* wake up */
    res = ntag21x_wake_up(&gs_handle, &type);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: wake up failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    if (type == NTAG21X_TYPE_213_5_6)
    {
        ntag21x_interface_debug_print("ntag21x: find ntag213/5/6 card.\n");
    }
    else
    {
        ntag21x_interface_debug_print("ntag21x: invalid type.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* anti collision cl1 */
    res = ntag21x_anticollision_cl1(&gs_handle, id);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: anti collision cl1 failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* select cl1 */
    res = ntag21x_select_cl1(&gs_handle, id);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: select cl1 failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: id1 is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
    
    /* anti collision cl2 */
    res = ntag21x_anticollision_cl2(&gs_handle, id);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: anti collision cl2 failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* anti collision cl2 */
    res = ntag21x_anticollision_cl2(&gs_handle, id);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: anti collision cl2 failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: id2 is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
    
    /* get capability container */
    res = ntag21x_get_capability_container(&gs_handle, &container);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get capability container failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    if (container == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
    {
        ntag21x_interface_debug_print("ntag21x: get capability container ntag213.\n");
    }
    else if (container == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
    {
        ntag21x_interface_debug_print("ntag21x: get capability container ntag215.\n");
    }
    else if (container == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
    {
        ntag21x_interface_debug_print("ntag21x: get capability container ntag216.\n");
    }
    else
    {
        ntag21x_interface_debug_print("ntag21x: unknown type.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get serial number */
    res = ntag21x_get_serial_number(&gs_handle, number);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get serial number failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: serial number is ");
    for (i = 0; i < 7; i++)
    {
        ntag21x_interface_debug_print("0x%02X ", number[i]);
    }
    ntag21x_interface_debug_print("\n");
    
    /* read signature */
    res = ntag21x_read_signature(&gs_handle, signature);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: read signature failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: signature is ");
    for (i = 0; i < 32; i++)
    {
        ntag21x_interface_debug_print("0x%02X ", signature[i]);
    }
    ntag21x_interface_debug_print("\n");
    
    ntag21x_interface_debug_print("ntag21x: read four pages from page 0.\n");
    
    /* read four pages */
    res = ntag21x_read_four_pages(&gs_handle, 0x00, data_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: read four pages failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < 16; i++)
    {
        ntag21x_interface_debug_print("0x%02X ", data_check[i]);
    }
    ntag21x_interface_debug_print("\n");
    
    ntag21x_interface_debug_print("ntag21x: read page 3.\n");
    
    /* read page */
    res = ntag21x_read_page(&gs_handle, 0x3, data_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: read page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < 4; i++)
    {
        ntag21x_interface_debug_print("0x%02X ", data_check[i]);
    }
    ntag21x_interface_debug_print("\n");
    
    ntag21x_interface_debug_print("ntag21x: fast read page from page 0 - page 5.\n");
    
    /* fast read page */
    len = 32;
    res = ntag21x_fast_read_page(&gs_handle, 0x00, 0x5, page, &len);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: fast read page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < 32; i++)
    {
        ntag21x_interface_debug_print("0x%02X ", page[i]);
    }
    ntag21x_interface_debug_print("\n");
    
    ntag21x_interface_debug_print("ntag21x: compatibility write page 16.\n");
    for (i = 0; i < 4; i++)
    {
        data[i] = rand() % 256;
    }
    res = ntag21x_compatibility_write_page(&gs_handle, 16, data);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: compatibility write page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    res = ntag21x_read_page(&gs_handle, 16, data_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: read page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check page %s.\n", memcmp(data, data_check, 4) == 0 ? "ok" : "error");
    
    ntag21x_interface_debug_print("ntag21x: write page 17.\n");
    for (i = 0; i < 4; i++)
    {
        data[i] = rand() % 256;
    }
    res = ntag21x_write_page(&gs_handle, 17, data);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: write page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    res = ntag21x_read_page(&gs_handle, 17, data_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: read page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check page %s.\n", memcmp(data, data_check, 4) == 0 ? "ok" : "error");
    
    /* check the password */
    pwd[0] = 0xFF;
    pwd[1] = 0xFF;
    pwd[2] = 0xFF;
    pwd[3] = 0xFF;
    pack[0] = 0x00;
    pack[1] = 0x00;
    res = ntag21x_set_password(&gs_handle, pwd);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set password failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    res = ntag21x_set_pack(&gs_handle, pack);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set pack failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: authenticate.\n");
    res = ntag21x_authenticate(&gs_handle, pwd, pack);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: authenticate failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get version */
    res = ntag21x_get_version(&gs_handle, &version);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get version failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
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
    
    /* set dynamic lock */
    lock[0] = 0x00;
    lock[1] = 0x00;
    lock[2] = 0x00;
    res = ntag21x_set_dynamic_lock(&gs_handle, lock);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set dynamic lock failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set dynamic lock 0x%02X 0x%02X 0x%02X\n", lock[0], lock[1], lock[2]);
    res = ntag21x_get_dynamic_lock(&gs_handle, lock_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get dynamic lock failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: get dynamic lock 0x%02X 0x%02X 0x%02X\n", lock_check[0], lock_check[1], lock_check[2]);
    
    /* set static lock */
    lock[0] = 0x00;
    lock[1] = 0x00;
    res = ntag21x_set_static_lock(&gs_handle, lock);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set static lock failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set static lock 0x%02X 0x%02X\n", lock[0], lock[1]);
    res = ntag21x_get_static_lock(&gs_handle, lock_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get static lock failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: get static lock 0x%02X 0x%02X\n", lock_check[0], lock_check[1]);
    
    /* set uid and nfc counter mirror */
    res = ntag21x_set_mirror(&gs_handle, NTAG21X_MIRROR_UID_NFC_COUNTER_ASCII);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set uid and nfc counter mirror.\n");
    res = ntag21x_get_mirror(&gs_handle, &mirror);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror %s.\n", mirror == NTAG21X_MIRROR_UID_NFC_COUNTER_ASCII ? "ok" : "error");
    
    /* set nfc counter mirror */
    res = ntag21x_set_mirror(&gs_handle, NTAG21X_MIRROR_NFC_COUNTER_ASCII);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set nfc counter mirror.\n");
    res = ntag21x_get_mirror(&gs_handle, &mirror);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror %s.\n", mirror == NTAG21X_MIRROR_NFC_COUNTER_ASCII ? "ok" : "error");
    
    /* set uid ascii mirror */
    res = ntag21x_set_mirror(&gs_handle, NTAG21X_MIRROR_UID_ASCII);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set uid ascii mirror.\n");
    res = ntag21x_get_mirror(&gs_handle, &mirror);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror %s.\n", mirror == NTAG21X_MIRROR_UID_ASCII ? "ok" : "error");
    
    /* set no ascii mirror */
    res = ntag21x_set_mirror(&gs_handle, NTAG21X_MIRROR_NO_ASCII);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set no ascii mirror.\n");
    res = ntag21x_get_mirror(&gs_handle, &mirror);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror %s.\n", mirror == NTAG21X_MIRROR_NO_ASCII ? "ok" : "error");
    
    /* set mirror byte 3 */
    res = ntag21x_set_mirror_byte(&gs_handle, NTAG21X_MIRROR_BYTE_3);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set mirror byte 3.\n");
    res = ntag21x_get_mirror_byte(&gs_handle, &byte);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror byte %s.\n", byte == NTAG21X_MIRROR_BYTE_3 ? "ok" : "error");
    
    /* set mirror byte 2 */
    res = ntag21x_set_mirror_byte(&gs_handle, NTAG21X_MIRROR_BYTE_2);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set mirror byte 2.\n");
    res = ntag21x_get_mirror_byte(&gs_handle, &byte);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror byte %s.\n", byte == NTAG21X_MIRROR_BYTE_2 ? "ok" : "error");
    
    /* set mirror byte 1 */
    res = ntag21x_set_mirror_byte(&gs_handle, NTAG21X_MIRROR_BYTE_1);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set mirror byte 1.\n");
    res = ntag21x_get_mirror_byte(&gs_handle, &byte);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror byte %s.\n", byte == NTAG21X_MIRROR_BYTE_1 ? "ok" : "error");
    
    /* set mirror byte 0 */
    res = ntag21x_set_mirror_byte(&gs_handle, NTAG21X_MIRROR_BYTE_0);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set mirror byte 0.\n");
    res = ntag21x_get_mirror_byte(&gs_handle, &byte);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror byte failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror byte %s.\n", byte == NTAG21X_MIRROR_BYTE_0 ? "ok" : "error");
    
    /* set normal mode */
    res = ntag21x_set_modulation_mode(&gs_handle, NTAG21X_MODULATION_MODE_NORMAL);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set modulation mode failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set normal mode.\n");
    res = ntag21x_get_modulation_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get modulation mode failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check modulation mode %s.\n", mode == NTAG21X_MODULATION_MODE_NORMAL ? "ok" : "error");
    
    /* set strong mode */
    res = ntag21x_set_modulation_mode(&gs_handle, NTAG21X_MODULATION_MODE_STRONG);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set modulation mode failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set strong mode.\n");
    res = ntag21x_get_modulation_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get modulation mode failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check modulation mode %s.\n", mode == NTAG21X_MODULATION_MODE_STRONG ? "ok" : "error");
    
    /* set mirror page */
    res = ntag21x_set_mirror_page(&gs_handle, 0x00);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set mirror page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set mirror page 0x00.\n");
    res = ntag21x_get_mirror_page(&gs_handle, &p);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get mirror page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check mirror page %s.\n", p == 0 ? "ok" : "error");
    
    /* set protect start page */
    res = ntag21x_set_protect_start_page(&gs_handle, 0xFF);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set protect start page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set protect start page 0xFF.\n");
    res = ntag21x_get_protect_start_page(&gs_handle, &p);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get protect start page failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check protect start page %s.\n", p == 0xFF ? "ok" : "error");
    
    /* read access protection */
    res = ntag21x_set_access(&gs_handle, NTAG21X_ACCESS_READ_PROTECTION, NTAG21X_BOOL_FALSE);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: read access protection disable.\n");
    res = ntag21x_get_access(&gs_handle, NTAG21X_ACCESS_READ_PROTECTION, &enable);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check read access protection %s.\n", enable == NTAG21X_BOOL_FALSE ? "ok" : "error");
    
    /* user configuration protection */
    res = ntag21x_set_access(&gs_handle, NTAG21X_ACCESS_USER_CONF_PROTECTION, NTAG21X_BOOL_FALSE);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: user configuration protection disable.\n");
    res = ntag21x_get_access(&gs_handle, NTAG21X_ACCESS_USER_CONF_PROTECTION, &enable);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check user configuration protection %s.\n", enable == NTAG21X_BOOL_FALSE ? "ok" : "error");
    
    /* nfc counter protection */
    res = ntag21x_set_access(&gs_handle, NTAG21X_ACCESS_NFC_COUNTER, NTAG21X_BOOL_TRUE);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: nfc counter protection enable.\n");
    res = ntag21x_get_access(&gs_handle, NTAG21X_ACCESS_NFC_COUNTER, &enable);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check nfc counter protection %s.\n", enable == NTAG21X_BOOL_TRUE ? "ok" : "error");
    
    /* nfc counter password protection */
    res = ntag21x_set_access(&gs_handle, NTAG21X_ACCESS_NFC_COUNTER_PASSWORD_PROTECTION, NTAG21X_BOOL_FALSE);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: nfc counter password protection enable.\n");
    res = ntag21x_get_access(&gs_handle, NTAG21X_ACCESS_NFC_COUNTER_PASSWORD_PROTECTION, &enable);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get access failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check nfc counter password protection %s.\n", enable == NTAG21X_BOOL_FALSE ? "ok" : "error");
    
    /* set authenticate limitation  */
    res = ntag21x_set_authenticate_limitation(&gs_handle, 7);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: set authenticate limitation failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: set authenticate limitation 7.\n");
    res = ntag21x_get_authenticate_limitation(&gs_handle, &limit_check);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: get authenticate limitation failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: check authenticate limitation %s.\n", limit_check == 7 ? "ok" : "error");
    
    /* read counter */
    res = ntag21x_read_counter(&gs_handle, &cnt);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: read counter failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    ntag21x_interface_debug_print("ntag21x: read counter %d.\n", cnt);
    
    /* halt */
    res = ntag21x_halt(&gs_handle);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: halt failed.\n");
        (void)ntag21x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish register */
    ntag21x_interface_debug_print("ntag21x: finish card test.\n");
    (void)ntag21x_deinit(&gs_handle);
    
    return 0;
}
