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
 * @file      driver_ntag21x.h
 * @brief     driver ntag21x header file
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

#ifndef DRIVER_NTAG21X_H
#define DRIVER_NTAG21X_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ntag21x_driver ntag21x driver function
 * @brief    ntag21x driver modules
 * @{
 */

/**
 * @addtogroup ntag21x_basic_driver
 * @{
 */

/**
 * @brief ntag21x bool enumeration definition
 */
typedef enum
{
    NTAG21X_BOOL_FALSE = 0x00,        /**< false */
    NTAG21X_BOOL_TRUE  = 0x01,        /**< true */
} ntag21x_bool_t;

/**
 * @brief ntag21x type enumeration definition
 */
typedef enum
{
    NTAG21X_TYPE_INVALID = 0x00,        /**< invalid */
    NTAG21X_TYPE_213_5_6 = 0x01,        /**< ntag213/5/6 */
} ntag21x_type_t;

/**
 * @brief ntag21x capability container enumeration definition
 */
typedef enum
{
    NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213 = 0x00,        /**< ntag213 */
    NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215 = 0x01,        /**< ntag215 */
    NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216 = 0x02,        /**< ntag216 */
} ntag21x_capability_container_t;

/**
 * @brief ntag21x mirror enumeration definition
 */
typedef enum
{
    NTAG21X_MIRROR_NO_ASCII              = 0x00,        /**< no ascii mirror */
    NTAG21X_MIRROR_UID_ASCII             = 0x01,        /**< uid ascii mirror */
    NTAG21X_MIRROR_NFC_COUNTER_ASCII     = 0x02,        /**< nfc counter mirror */
    NTAG21X_MIRROR_UID_NFC_COUNTER_ASCII = 0x03,        /**< uid and nfc counter mirror */
} ntag21x_mirror_t;

/**
 * @brief ntag21x mirror byte enumeration definition
 */
typedef enum
{
    NTAG21X_MIRROR_BYTE_0 = 0x00,        /**< byte0 */
    NTAG21X_MIRROR_BYTE_1 = 0x01,        /**< byte1 */
    NTAG21X_MIRROR_BYTE_2 = 0x02,        /**< byte2 */
    NTAG21X_MIRROR_BYTE_3 = 0x03,        /**< byte3 */
} ntag21x_mirror_byte_t;

/**
 * @brief ntag21x modulation mode enumeration definition
 */
typedef enum
{
    NTAG21X_MODULATION_MODE_NORMAL = 0x00,        /**< normal */
    NTAG21X_MODULATION_MODE_STRONG = 0x01,        /**< strong */
} ntag21x_modulation_mode_t;

/**
 * @brief ntag21x access enumeration definition
 */
typedef enum
{
    NTAG21X_ACCESS_READ_PROTECTION                 = 7,        /**< enable or disable read access protection */
    NTAG21X_ACCESS_USER_CONF_PROTECTION            = 6,        /**< enable or disable user configuration protection */
    NTAG21X_ACCESS_NFC_COUNTER                     = 4,        /**< enable or disable nfc counter protection */
    NTAG21X_ACCESS_NFC_COUNTER_PASSWORD_PROTECTION = 3,        /**< enable or disable nfc counter password protection */
} ntag21x_access_t;

/**
 * @brief ntag21x version structure definition
 */
typedef struct ntag21x_version_s
{
    uint8_t fixed_header;                 /**< fixed header */
    uint8_t vendor_id;                    /**< vendor id */
    uint8_t product_type;                 /**< product type */
    uint8_t product_subtype;              /**< product subtype */
    uint8_t major_product_version;        /**< major product version */
    uint8_t minor_product_version;        /**< minor product version */
    uint8_t storage_size;                 /**< storage size */
    uint8_t protocol_type;                /**< protocol type */
} ntag21x_version_t;

/**
 * @brief ntag21x handle structure definition
 */
typedef struct ntag21x_handle_s
{
    uint8_t (*contactless_init)(void);                                             /**< point to a contactless_init function address */
    uint8_t (*contactless_deinit)(void);                                           /**< point to a contactless_deinit function address */
    uint8_t (*contactless_transceiver)(uint8_t *in_buf, uint8_t in_len, 
                                       uint8_t *out_buf, uint8_t *out_len);        /**< point to a contactless_transceiver function address */
    void (*delay_ms)(uint32_t ms);                                                 /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                               /**< point to a debug_print function address */
    uint8_t type;                                                                  /**< ntag type */
    uint8_t inited;                                                                /**< inited flag */
    uint8_t end_page;                                                              /**< end page */
} ntag21x_handle_t;

/**
 * @brief ntag21x information structure definition
 */
typedef struct ntag21x_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ntag21x_info_t;

/**
 * @}
 */

/**
 * @defgroup ntag21x_link_driver ntag21x link driver function
 * @brief    ntag21x link driver modules
 * @ingroup  ntag21x_driver
 * @{
 */

/**
 * @brief     initialize ntag21x_handle_t structure
 * @param[in] HANDLE points to an ntag21x handle structure
 * @param[in] STRUCTURE is ntag21x_handle_t
 * @note      none
 */
#define DRIVER_NTAG21X_LINK_INIT(HANDLE, STRUCTURE)                  memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link contactless_init function
 * @param[in] HANDLE points to an ntag21x handle structure
 * @param[in] FUC points to a contactless_init function address
 * @note      none
 */
#define DRIVER_NTAG21X_LINK_CONTACTLESS_INIT(HANDLE, FUC)           (HANDLE)->contactless_init = FUC

/**
 * @brief     link contactless_deinit function
 * @param[in] HANDLE points to an ntag21x handle structure
 * @param[in] FUC points to a contactless_deinit function address
 * @note      none
 */
#define DRIVER_NTAG21X_LINK_CONTACTLESS_DEINIT(HANDLE, FUC)         (HANDLE)->contactless_deinit = FUC

/**
 * @brief     link contactless_transceiver function
 * @param[in] HANDLE points to an ntag21x handle structure
 * @param[in] FUC points to a contactless_transceiver function address
 * @note      none
 */
#define DRIVER_NTAG21X_LINK_CONTACTLESS_TRANSCEIVER(HANDLE, FUC)    (HANDLE)->contactless_transceiver = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an ntag21x handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_NTAG21X_LINK_DELAY_MS(HANDLE, FUC)                   (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an ntag21x handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_NTAG21X_LINK_DEBUG_PRINT(HANDLE, FUC)                (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ntag21x_basic_driver ntag21x basic driver function
 * @brief    ntag21x basic driver modules
 * @ingroup  ntag21x_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to an ntag21x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ntag21x_info(ntag21x_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an ntag21x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 contactless initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t ntag21x_init(ntag21x_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an ntag21x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 contactless deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_deinit(ntag21x_handle_t *handle);

/**
 * @brief      ntag21x request
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 request failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 type is invalid
 * @note       none
 */
uint8_t ntag21x_request(ntag21x_handle_t *handle, ntag21x_type_t *type);

/**
 * @brief      ntag21x wake up
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 wake up failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 type is invalid
 * @note       none
 */
uint8_t ntag21x_wake_up(ntag21x_handle_t *handle, ntag21x_type_t *type);

/**
 * @brief     ntag21x halt
 * @param[in] *handle points to an ntag21x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 halt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_halt(ntag21x_handle_t *handle);

/**
 * @brief      ntag21x anti collision cl1
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 anti collision cl1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 check error
 * @note       none
 */
uint8_t ntag21x_anticollision_cl1(ntag21x_handle_t *handle, uint8_t id[4]);

/**
 * @brief      ntag21x anti collision cl2
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 anti collision cl2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 check error
 * @note       none
 */
uint8_t ntag21x_anticollision_cl2(ntag21x_handle_t *handle, uint8_t id[4]);

/**
 * @brief     ntag21x select cl1
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *id points to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 select cl1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 sak error
 * @note      none
 */
uint8_t ntag21x_select_cl1(ntag21x_handle_t *handle, uint8_t id[4]);

/**
 * @brief     ntag21x select cl2
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *id points to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 select cl2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 sak error
 * @note      none
 */
uint8_t ntag21x_select_cl2(ntag21x_handle_t *handle, uint8_t id[4]);

/**
 * @brief      ntag21x get the version
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *version points to a version structure
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_version(ntag21x_handle_t *handle, ntag21x_version_t *version);

/**
 * @brief      ntag21x read four pages
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  start_page is the start page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read four pages failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_four_pages(ntag21x_handle_t *handle, uint8_t start_page, uint8_t data[16]);

/**
 * @brief      ntag21x read page
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  page is the page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_page(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4]);

/**
 * @brief         ntag21x fast read page
 * @param[in]     *handle points to an ntag21x handle structure
 * @param[in]     start_page is the start page
 * @param[in]     stop_page is the stop page
 * @param[out]    *data points to a data buffer
 * @param[in,out] *len points to a data length buffer
 * @return        status code
 *                - 0 success
 *                - 1 fast read page failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 stop_page < start_page
 *                - 5 stop_page - start_page + 1 is over 15
 *                - 6 len is invalid
 *                - 7 output_len is invalid
 *                - 8 crc error
 * @note          stop_page - start_page + 1 <= 15
 *                4 * (stop_page - start_page + 1) < len
 */
uint8_t ntag21x_fast_read_page(ntag21x_handle_t *handle, uint8_t start_page, uint8_t stop_page, uint8_t *data, uint16_t *len);

/**
 * @brief     ntag21x compatibility write page
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 compatibility write page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_compatibility_write_page(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4]);

/**
 * @brief     ntag21x write page
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_write_page(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4]);

/**
 * @brief      ntag21x read the counter
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 read counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_counter(ntag21x_handle_t *handle, uint32_t *cnt);

/**
 * @brief      ntag21x read the signature
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *signature points to a signature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read signature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_signature(ntag21x_handle_t *handle, uint8_t signature[32]);

/**
 * @brief      ntag21x get the serial number
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_serial_number(ntag21x_handle_t *handle, uint8_t number[7]);

/**
 * @brief      ntag21x get the capability container
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *container points to a capability container buffer
 * @return     status code
 *             - 0 success
 *             - 1 get capability container failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 *             - 6 data is invalid
 * @note       none
 */
uint8_t ntag21x_get_capability_container(ntag21x_handle_t *handle, ntag21x_capability_container_t *container);

/**
 * @brief     ntag21x set the password
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *pwd points to a password buffer
 * @return    status code
 *            - 0 success
 *            - 1 set pwd failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_set_password(ntag21x_handle_t *handle, uint8_t pwd[4]);

/**
 * @brief     ntag21x set the pack
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 set pack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_set_pack(ntag21x_handle_t *handle, uint8_t pack[2]);

/**
 * @brief     ntag21x authenticate
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *pwd points to a pwd buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 authenticate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 crc error
 *            - 6 pack check failed
 * @note      none
 */
uint8_t ntag21x_authenticate(ntag21x_handle_t *handle, uint8_t pwd[4], uint8_t pack[2]);

/**
 * @brief     ntag21x set the dynamic lock
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set dynamic lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note                                                               ntag213                                                                                    
 *                                                                     byte0                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            lock_page_30_31    lock_page_28_29    lock_page_26_27    lock_page_24_25    lock_page_22_23    lock_page_20_21   lock_page_18_19   lock_page_16_17  
 *                                                                     byte1                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  0                  0                  lock_page_38_39    lock_page_36_37   lock_page_34_35   lock_page_32_33  
 *                                                                     byte2                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  block_page_36_39   block_page_32_35   block_page_28_31   block_page_24_27  block_page_20_23  block_page_16_19 
 *                                                                     ntag215                                                                                    
 *                                                                     byte0                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            lock_page_128_129  lock_page_112_127  lock_page_96_111   lock_page_80_95    lock_page_64_79    lock_page_48_63   lock_page_32_47   lock_page_16_31  
 *                                                                     byte1                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  0                  0                  0                  0                 0                 0                
 *                                                                     byte2                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  0                  0                  block_page_112_129 block_page_80_111 block_page_48_79  block_page_16_47 
 *                                                                     ntag216                                                                                    
 *                                                                     byte0                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            lock_page_128_143  lock_page_112_127  lock_page_96_111   lock_page_80_95    lock_page_64_79    lock_page_48_63   lock_page_32_47   lock_page_16_31  
 *                                                                     byte1                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  lock_page_224_225  lock_page_208_223  lock_page_192_207  lock_page_176_191 lock_page_160_175 lock_page_144_159
 *                                                                     byte2                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  block_page_208_225 block_page_176_207 block_page_144_175 block_page_112_143 block_page_80_111 block_page_48_79  block_page_16_47 
 */
uint8_t ntag21x_set_dynamic_lock(ntag21x_handle_t *handle, uint8_t lock[3]);

/**
 * @brief      ntag21x get the dynamic lock
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *lock points to a lock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get dynamic lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_dynamic_lock(ntag21x_handle_t *handle, uint8_t lock[3]);

/**
 * @brief     ntag21x set the static lock
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set static lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note                                                          byte0                                                    
 *            bit7         bit6         bit5         bit4         bit3         bit2             bit1           bit0        
 *            lock_page_7  lock_page_6  lock_page_5  lock_page_4  lock_page_cc block_page_15_10 block_page_9_2 bock_page_cc
 *                                                                byte1                                                    
 *            bit7         bit6         bit5         bit4         bit3         bit2             bit1           bit0        
 *            lock_page_15 lock_page_14 lock_page_13 lock_page_12 lock_page_11 lock_page_10     lock_page_9    lock_page_8 
 */
uint8_t ntag21x_set_static_lock(ntag21x_handle_t *handle, uint8_t lock[2]);

/**
 * @brief      ntag21x get the static lock
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *lock points to a lock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get static lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_static_lock(ntag21x_handle_t *handle, uint8_t lock[2]);

/**
 * @brief     ntag21x set the mirror
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] mirror is the chip mirror mode
 * @return    status code
 *            - 0 success
 *            - 1 set mirror failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_mirror(ntag21x_handle_t *handle, ntag21x_mirror_t mirror);

/**
 * @brief      ntag21x get the mirror
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *mirror points to a chip mirror mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mirror failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_mirror(ntag21x_handle_t *handle, ntag21x_mirror_t *mirror);

/**
 * @brief     ntag21x set the mirror byte
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] byte is the mirror byte
 * @return    status code
 *            - 0 success
 *            - 1 set mirror byte failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_mirror_byte(ntag21x_handle_t *handle, ntag21x_mirror_byte_t byte);

/**
 * @brief      ntag21x get the mirror byte
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *byte points to a mirror byte buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mirror byte failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_mirror_byte(ntag21x_handle_t *handle, ntag21x_mirror_byte_t *byte);

/**
 * @brief     ntag21x set the modulation mode
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] mode is the modulation mode
 * @return    status code
 *            - 0 success
 *            - 1 set modulation mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_modulation_mode(ntag21x_handle_t *handle, ntag21x_modulation_mode_t mode);

/**
 * @brief      ntag21x get the modulation mode
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *mode points to a modulation mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get modulation mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_modulation_mode(ntag21x_handle_t *handle, ntag21x_modulation_mode_t *mode);

/**
 * @brief     ntag21x set the mirror page
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the mirror page
 * @return    status code
 *            - 0 success
 *            - 1 set mirror page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_mirror_page(ntag21x_handle_t *handle, uint8_t page);

/**
 * @brief      ntag21x get the mirror page
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *page points to a mirror page buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mirror page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_mirror_page(ntag21x_handle_t *handle, uint8_t *page);

/**
 * @brief     ntag21x set the start page of protection
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the start page
 * @return    status code
 *            - 0 success
 *            - 1 set protect start page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_protect_start_page(ntag21x_handle_t *handle, uint8_t page);

/**
 * @brief      ntag21x get the start page of protection
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *page points to a start page buffer
 * @return     status code
 *             - 0 success
 *             - 1 get protect start page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_protect_start_page(ntag21x_handle_t *handle, uint8_t *page);

/**
 * @brief     ntag21x enable or disable access
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] access is the set access
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set access failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_access(ntag21x_handle_t *handle, ntag21x_access_t access, ntag21x_bool_t enable);

/**
 * @brief      ntag21x get the access status
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  access is the set access
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get access failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_access(ntag21x_handle_t *handle, ntag21x_access_t access, ntag21x_bool_t *enable);

/**
 * @brief     ntag21x set the authenticate limitation
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] limit is the authenticate limitation
 * @return    status code
 *            - 0 success
 *            - 1 set authenticate limitation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 limit > 7
 * @note      none
 */
uint8_t ntag21x_set_authenticate_limitation(ntag21x_handle_t *handle, uint8_t limit);

/**
 * @brief      ntag21x get the authenticate limitation
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *limit points to an authenticate limitation buffer
 * @return     status code
 *             - 0 success
 *             - 1 get authenticate limitation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_authenticate_limitation(ntag21x_handle_t *handle, uint8_t *limit);

/**
 * @}
 */

/**
 * @defgroup ntag21x_extern_driver ntag21x extern driver function
 * @brief    ntag21x extern driver modules
 * @ingroup  ntag21x_driver
 * @{
 */

/**
 * @brief         transceiver data
 * @param[in]     *handle points to an ntag21x handle structure
 * @param[in]     *in_buf points to an input buffer
 * @param[in]     in_len is the input length
 * @param[out]    *out_buf points to an output buffer
 * @param[in,out] *out_len points to an output length buffer
 * @return        status code
 *                - 0 success
 *                - 1 transceiver failed
 * @note          none
 */
uint8_t ntag21x_transceiver(ntag21x_handle_t *handle, uint8_t *in_buf, uint8_t in_len, uint8_t *out_buf, uint8_t *out_len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
