#include <rtl8139/rtl8139.h>


#define RX_BUFFER_SIZE 8192 + 16 + 1500

// RTL8139 registers offsets (I/O ports)
#define MAC0    0x00
#define MAR0    0x08
#define TX_STATUS0 0x10
#define TX_ADDR0   0x20
#define RX_BUF     0x30
#define CMD        0x37
#define RX_BUF_ADDR 0x30
#define IMR        0x3C
#define ISR        0x3E
#define CAPR       0x38
#define CBR        0x3A

static uint16_t io_base;

// --- RX buffer (circular) ---
static uint8_t rx_buffer[RX_BUFFER_SIZE] __attribute__((aligned(16)));
static uint16_t rx_offset = 0;

// --- MAC address ---
uint8_t rtl8139_mac[6];

// --- IRQ handler (private) ---
static void rtl8139_irq_handler(void) {
    uint16_t isr = inw(io_base + ISR);
    // Clear interrupt flags
    outw(io_base + ISR, isr);

    // For now, do nothing; reception is polled from buffer
}

// --- Initialize RTL8139 NIC ---
void rtl8139_init(uint16_t base) {
    io_base = base;

    // Reset NIC
    outb(io_base + CMD, 0x10);
    while (inb(io_base + CMD) & 0x10);

    // Initialize RX buffer
    outl(io_base + RX_BUF_ADDR, (uint32_t)rx_buffer);
    rx_offset = 0;

    // Read MAC address
    for (int i = 0; i < 6; i++) {
        rtl8139_mac[i] = inb(io_base + MAC0 + i);
    }

    // Enable RX and TX
    outb(io_base + CMD, 0x0C);

    // Enable all RX interrupts
    outw(io_base + IMR, 0x0005); // RX OK + RX ERR

    // Register IRQ handler (implementation depends on your kernel)
    // Example: register_interrupt_handler(nic_irq, rtl8139_irq_handler);
}

// --- Send a packet ---
void rtl8139_send(uint8_t* data, uint16_t length) {
    if (length > 1518) return; // Ethernet max frame size

    // For simplicity, using only TX0
    outl(io_base + TX_ADDR0, (uint32_t)data);
    outl(io_base + TX_STATUS0, length);
}

// --- Receive a packet (polling) ---
uint8_t* rtl8139_receive(uint16_t* length) {
    if ((rx_offset + 4) >= RX_BUFFER_SIZE) rx_offset = 0;

    uint16_t pkt_status = *(uint16_t*)&rx_buffer[rx_offset];
    uint16_t pkt_len    = *(uint16_t*)&rx_buffer[rx_offset + 2];

    if (!(pkt_status & 0x01)) return 0; // no packet ready

    uint8_t* data = &rx_buffer[rx_offset + 4]; // skip status + length
    *length = pkt_len;

    rx_offset += pkt_len + 4; // advance offset
    if (rx_offset >= RX_BUFFER_SIZE) rx_offset = 0;

    return data;
}
