#pragma once
#include <stdint.h>
#include <io/io.h>
#include <string.h>
// Initialize RTL8139 NIC
// Registers the IRQ and sets up RX buffer
void rtl8139_init(uint16_t io_base);

// Send a single Ethernet packet
// 'data' points to full Ethernet frame, 'length' <= 1518 bytes
void rtl8139_send(uint8_t* data, uint16_t length);

// Check if a packet is available in RX buffer and return pointer to data
// Returns 0 if no packet is ready
uint8_t* rtl8139_receive(uint16_t* length);

// MAC address of the NIC (read during init)
extern uint8_t rtl8139_mac[6];
