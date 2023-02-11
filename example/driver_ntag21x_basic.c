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
 * @file      driver_ntag21x_basic.c
 * @brief     driver ntag21x basic source file
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

#include "driver_ntag21x_basic.h"

static ntag21x_handle_t gs_handle;        /**< ntag21x handle */

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
static void s_debug_print(const char *const fmt, ...)
{
    (void)fmt;
    
    return;
}

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ntag21x_basic_init(void)
{
    uint8_t res;
    
    /* link function */
    DRIVER_NTAG21X_LINK_INIT(&gs_handle, ntag21x_handle_t);
    DRIVER_NTAG21X_LINK_CONTACTLESS_INIT(&gs_handle, ntag21x_interface_contactless_init);
    DRIVER_NTAG21X_LINK_CONTACTLESS_DEINIT(&gs_handle, ntag21x_interface_contactless_deinit);
    DRIVER_NTAG21X_LINK_CONTACTLESS_TRANSCEIVER(&gs_handle, ntag21x_interface_contactless_transceiver);
    DRIVER_NTAG21X_LINK_DELAY_MS(&gs_handle, ntag21x_interface_delay_ms);
#ifndef NO_DEBUG
    DRIVER_NTAG21X_LINK_DEBUG_PRINT(&gs_handle, ntag21x_interface_debug_print);
#else
    DRIVER_NTAG21X_LINK_DEBUG_PRINT(&gs_handle, s_debug_print);
#endif
    
    /* init */
    res = ntag21x_init(&gs_handle);
    if (res != 0)
    {
        ntag21x_interface_debug_print("ntag21x: init failed.\n");
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ntag21x_basic_deinit(void)
{
    uint8_t res;
    
    /* deinit */
    res = ntag21x_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example halt
 * @return status code
 *         - 0 success
 *         - 1 halt failed
 * @note   none
 */
uint8_t ntag21x_basic_halt(void)
{
    uint8_t res;
    
    /* halt */
    res = ntag21x_halt(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t ntag21x_basic_wake_up(void)
{
    uint8_t res;
    ntag21x_type_t type;
    
    /* wake up */
    res = ntag21x_wake_up(&gs_handle, &type);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example search
 * @param[out] *type points to a type buffer
 * @param[out] *id points to an id buffer
 * @param[in]  timeout is the check times
 * @return     status code
 *             - 0 success
 *             - 1 timeout
 * @note       none
 */
uint8_t ntag21x_basic_search(ntag21x_capability_container_t *type, uint8_t id[8], int32_t timeout)
{
    uint8_t res;
    ntag21x_type_t t;
    
    /* loop */
    while (1)
    {
        /* request */
        res = ntag21x_request(&gs_handle, &t);
        if (res == 0)
        {
            /* anti collision_cl1 */
            res = ntag21x_anticollision_cl1(&gs_handle, id);
            if (res == 0)
            {
                /* cl1 */
                res = ntag21x_select_cl1(&gs_handle, id);
                if (res == 0)
                {
                    /* anti collision_cl2 */
                    res = ntag21x_anticollision_cl2(&gs_handle, id + 4);
                    if (res == 0)
                    {
                        /* cl2 */
                        res = ntag21x_select_cl2(&gs_handle, id + 4);
                        if (res == 0)
                        {
                            res = ntag21x_get_capability_container(&gs_handle, type);
                            if (res == 0)
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        
        /* delay */
        ntag21x_interface_delay_ms(MIFARE_NTAG21X_DEFAULT_SEARCH_DELAY_MS);
        
        /* check the timeout */
        if (timeout < 0)
        {
            /* never timeout */
            continue;
        }
        else
        {
            /* timeout */
            if (timeout == 0)
            {
                return 1;
            }
            else
            {
                /* timout-- */
                timeout--;
            }
        }
    }
}

/**
 * @brief      basic example read
 * @param[in]  page is the read page
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ntag21x_basic_read(uint8_t page, uint8_t data[4])
{
    uint8_t res;
    
    /* read */
    res = ntag21x_read_page(&gs_handle, page, data);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read four pages
 * @param[in]  start_page is the start page
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ntag21x_basic_read_four_pages(uint8_t start_page, uint8_t data[16])
{
    uint8_t res;
    
    /* read */
    res = ntag21x_read_four_pages(&gs_handle, start_page, data);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief         basic example read pages
 * @param[in]     start_page is the start page
 * @param[in]     stop_page is the stop page
 * @param[out]    *data points to a data buffer
 * @param[in,out] *len points to a data length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          stop_page - start_page + 1 <= 15
 *                4 * (stop_page - start_page + 1) < len
 */
uint8_t ntag21x_basic_read_pages(uint8_t start_page, uint8_t stop_page, uint8_t *data, uint16_t *len)
{
    uint8_t res;
    
    /* fast read */
    res = ntag21x_fast_read_page(&gs_handle, start_page, stop_page, data, len);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example write
 * @param[in] page is the written page
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ntag21x_basic_write(uint8_t page, uint8_t data[4])
{
    uint8_t res;
    
    /* write */
    res = ntag21x_write_page(&gs_handle, page, data);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get version
 * @param[out] *version points to a version structure
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 * @note       none
 */
uint8_t ntag21x_basic_get_version(ntag21x_version_t *version)
{
    uint8_t res;
    
    /* get the version */
    res = ntag21x_get_version(&gs_handle, version);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read counter
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 read counter failed
 * @note       none
 */
uint8_t ntag21x_basic_read_counter(uint32_t *cnt)
{
    uint8_t res;
    
    /* read the counter */
    res = ntag21x_read_counter(&gs_handle, cnt);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read signature
 * @param[out] *signature points to a signature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read signature failed
 * @note       none
 */
uint8_t ntag21x_basic_read_signature(uint8_t signature[32])
{
    uint8_t res;
    
    /* read the signature */
    res = ntag21x_read_signature(&gs_handle, signature);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get serial number
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t ntag21x_basic_get_serial_number(uint8_t number[7])
{
    uint8_t res;
    
    /* get serial number */
    res = ntag21x_get_serial_number(&gs_handle, number);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set password and pack
 * @param[in] *pwd points to a password buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 set password pack failed
 * @note      none
 */
uint8_t ntag21x_basic_set_password_pack(uint8_t pwd[4], uint8_t pack[2])
{
    uint8_t res;
    
    /* set password */
    res = ntag21x_set_password(&gs_handle, pwd);
    if (res != 0)
    {
        return 1;
    }
    
    /* set pack */
    res = ntag21x_set_pack(&gs_handle, pack);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example authenticate
 * @param[in] *pwd points to a password buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 authenticate failed
 * @note      none
 */
uint8_t ntag21x_basic_authenticate(uint8_t pwd[4], uint8_t pack[2])
{
    uint8_t res;
    
    /* authenticate */
    res = ntag21x_authenticate(&gs_handle, pwd, pack);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set dynamic lock
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set dynamic lock failed
 * @note      none
 */
uint8_t ntag21x_basic_set_dynamic_lock(uint8_t lock[3])
{
    uint8_t res;
    
    /* set dynamic lock */
    res = ntag21x_set_dynamic_lock(&gs_handle, lock);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set static lock
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set static lock failed
 * @note      none
 */
uint8_t ntag21x_basic_set_static_lock(uint8_t lock[2])
{
    uint8_t res;
    
    /* set static lock */
    res = ntag21x_set_static_lock(&gs_handle, lock);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set mirror
 * @param[in] mirror is the chip mirror mode
 * @param[in] byte is the mirror byte
 * @param[in] page is the mirror page
 * @return    status code
 *            - 0 success
 *            - 1 set mirror failed
 * @note      none
 */
uint8_t ntag21x_basic_set_mirror(ntag21x_mirror_t mirror, ntag21x_mirror_byte_t byte, uint8_t page)
{
    uint8_t res;
    
    /* set mirror */
    res = ntag21x_set_mirror(&gs_handle, mirror);
    if (res != 0)
    {
        return 1;
    }
    
    /* set mirror byte */
    res = ntag21x_set_mirror_byte(&gs_handle, byte);
    if (res != 0)
    {
        return 1;
    }
    
    /* set mirror page */
    res = ntag21x_set_mirror_page(&gs_handle, page);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set modulation mode
 * @param[in] mode is the modulation mode
 * @return    status code
 *            - 0 success
 *            - 1 set modulation mode failed
 * @note      none
 */
uint8_t ntag21x_basic_set_modulation_mode(ntag21x_modulation_mode_t mode)
{
    uint8_t res;
    
    /* set modulation mode */
    res = ntag21x_set_modulation_mode(&gs_handle, mode);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set protect start page
 * @param[in] page is the start page
 * @return    status code
 *            - 0 success
 *            - 1 set protect start page failed
 * @note      none
 */
uint8_t ntag21x_basic_set_protect_start_page(uint8_t page)
{
    uint8_t res;
    
    /* set protect start page */
    res = ntag21x_set_protect_start_page(&gs_handle, page);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set the authenticate limitation
 * @param[in] limit is the authenticate limitation
 * @return    status code
 *            - 0 success
 *            - 1 set authenticate limitation failed
 * @note      none
 */
uint8_t ntag21x_basic_set_authenticate_limitation(uint8_t limit)
{
    uint8_t res;
    
    /* set authenticate limitation */
    res = ntag21x_set_authenticate_limitation(&gs_handle, limit);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example set access
 * @param[in] access is the set access
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set access failed
 * @note      none
 */
uint8_t ntag21x_basic_set_access(ntag21x_access_t access, ntag21x_bool_t enable)
{
    uint8_t res;
    
    /* set access */
    res = ntag21x_set_access(&gs_handle, access, enable);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
