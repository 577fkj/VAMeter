/*
 * INA228 - TI Current/Voltage/Power Monitor Code
 * Copyright (C) 2021 Craig Peacock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef MAIN_INA228_H_
#define MAIN_INA228_H_

#include <stdint.h>

#define INA228_SLAVE_ADDRESS 0x40

#define INA228_CONFIG 0x00
#define INA228_ADC_CONFIG 0x01
#define INA228_SHUNT_CAL 0x02
#define INA228_SHUNT_TEMPCO 0x03
#define INA228_VSHUNT 0x04
#define INA228_VBUS 0x05
#define INA228_DIETEMP 0x06
#define INA228_CURRENT 0x07
#define INA228_POWER 0x08
#define INA228_ENERGY 0x09
#define INA228_CHARGE 0x0A
#define INA228_DIAG_ALRT 0x0B
#define INA228_SOVL 0x0C
#define INA228_SUVL 0x0D
#define INA228_BOVL 0x0E
#define INA228_BUVL 0x0F
#define INA228_TEMP_LIMIT 0x10
#define INA228_PWR_LIMIT 0x11
#define INA228_MANUFACTURER_ID 0x3E
#define INA228_DEVICE_ID 0x3F

typedef struct
{
    // clang-format off
    uint16_t RST      : 1; // 第 15 位
    uint16_t RSTACC   : 1; // 第 14 位
    uint16_t CONVDLY  : 8; // 第 13-6 位
    uint16_t TEMPCOMP : 1; // 第 5 位
    uint16_t ADCRANGE : 1; // 第 4 位
    uint16_t RESERVED : 4; // 第 3-0 位 (保留)
} ina228_config_reg_t;


typedef struct
{
    uint8_t i2c_master_port;
    uint8_t address;
    uint16_t max_current;
    double r_shunt;

    // private
    uint16_t shunt_cal;
    double current_lsb;
    ina228_config_reg_t config_reg;
} ina228_config_t;

void ina228_read_config(ina228_config_t *config, ina228_config_reg_t *config_reg);
void ina228_write_config(ina228_config_t *config, ina228_config_reg_t *config_reg);

double calculate_current_lsb(uint16_t max_current);
uint16_t calculate_shunt_cal(uint16_t max_current, double r_shunt);

void ina228_init(ina228_config_t *config);
float ina228_voltage(ina228_config_t *config);
float ina228_dietemp(ina228_config_t *config);
float ina228_shuntvoltage(ina228_config_t *config);
float ina228_current(ina228_config_t *config);
float ina228_power(ina228_config_t *config);
float ina228_energy(ina228_config_t *config);
float ina228_charge(ina228_config_t *config);

// void ina228_set_alert(ina228_config_t *config, uint16_t alert); // TODO

void set_shunt_cal(ina228_config_t *config, uint16_t new_shunt_cal);
void set_current_lsb(ina228_config_t *config, double new_current_lsb);

#endif