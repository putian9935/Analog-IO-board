#include "init_chips.hpp"
#include <DMAChannel.h>
#define CS2 36

// MOSI, 11, B0_02
// MISO, 12, B0_01
// CS2, 36, B1_02
#define ALT2 2
#define ALT5 5

static void start_fourway()
{

    static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;

    uint32_t tcr  = spi_regs->TCR;
    spi_regs->TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(63) | LPSPI_TCR_RXMSK | LPSPI_TCR_WIDTH(2);

    // set datawidth to 4
    spi_regs->CFGR1 |= (LPSPI_CFGR1_PINCFG(2) | LPSPI_CFGR1_PCSCFG);

    // turn on cs2 36
    pinMode(CS2, OUTPUT);
    IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02 = ALT2;
}

static uint32_t convert_8_32(uint32_t num)
{
    num = ((num & 0x0000000F) | ((num & 0x000000F0) << 12));
    num = ((num & 0x00030003) | ((num & 0x000C000C) << 6));
    num = ((num & 0x01010101) | ((num & 0x02020202) << 3));
    return num;
}

void fourway_write(uint16_t word)
{
    static uint8_t const CS2_offset = 2;
    static IMXRT_LPSPI_t* spi_regs  = &IMXRT_LPSPI4_S;

    uint32_t low  = convert_8_32(word & 0xFF) << CS2_offset,
             high = convert_8_32((word & 0xFF00) >> 8) << CS2_offset;
    while ((spi_regs->FSR & 0xff) > 14)
        ;
    spi_regs->TDR = high;
    spi_regs->TDR = low;
}

static void end_fourway()
{
    // turn off cs2 36
    IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02 = ALT5;
    digitalWrite(CS2, LOW);

    // set datawidth to 2
    static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;

    spi_regs->CFGR1 &= ~LPSPI_CFGR1_PCSCFG;
    spi_regs->CFGR1 |= LPSPI_CFGR1_OUTCFG | LPSPI_CFGR1_SAMPLE | LPSPI_CFGR1_MASTER;

    uint32_t tcr  = spi_regs->TCR;
    spi_regs->TCR = ((tcr & 0xfff00000) | LPSPI_TCR_FRAMESZ(31) | LPSPI_TCR_WIDTH(1));

    // spi_regs->CCR |= LPSPI_CCR_DBT(255) | LPSPI_CCR_PCSSCK(255) | LPSPI_CCR_SCKPCS(255);
    spi_regs->CCR |= LPSPI_CCR_DBT(20) | LPSPI_CCR_PCSSCK(1) | LPSPI_CCR_SCKPCS(1);

    spi_regs->DER = LPSPI_DER_RDDE | LPSPI_DER_TDDE;
}

volatile uint32_t ch0, ch1;

static DMAChannel rx, tx;
static uint32_t txb;

static void initSPIMasterDMA()
{
    static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;
    rx.begin(true);
    rx.source(spi_regs->RDR);
    rx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_RX);
    rx.destination(ch0);
    rx.transferCount(2);
    rx.TCD->DOFF     = 4;
    rx.TCD->DLASTSGA = -8;

    tx.disable();
    tx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_TX);
    tx.destination(spi_regs->TCR);
    tx.source(txb);
    tx.transferCount(1);
    txb = spi_regs->TCR | LPSPI_TCR_TXMSK;

    rx.enable();
    tx.enable();
}

void adc_init()
{
    SPI.begin();
    SPI.beginTransaction(SPISettings(ADC_FCLK, MSBFIRST, SPI_MODE2));
    SPI.setCS(CS);

    set_fastio_pin(CS);
    set_fastio_pin(MOSI);
    set_fastio_pin(MISO);
    set_fastio_pin(SCK);

    start_fourway();

    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG2 | ADC_CFG2_HRST));
    delay(10);
    fourway_write(ADC_NOP);
    fourway_write(ADC_NOP);
    delay(10);

    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG1 | ADC_CFG1_SEQ | (1 << 9) | (3 << 6)));
    delay(1);
    fourway_write(ADC_NOP);
    fourway_write(ADC_NOP);
    delay(10);
    end_fourway();
    initSPIMasterDMA();
}

uint16_t decode_32_16(uint32_t num)
{
    num = (num & 0x11111111) | ((num & 0x44444444) >> 1);
    num = (num & 0x03030303) | ((num & 0x30303030) >> 2);
    num = (num & 0x000F000F) | ((num & 0x0F000F00) >> 4);
    num = (num & 0x000000FF) | ((num & 0x00FF0000) >> 8);
    return num;
}

std::pair<uint16_t, uint16_t> decode(uint32_t data)
{
    return std::pair<uint16_t, uint16_t>(decode_32_16(data), decode_32_16(data >> 1));
}

uint16_t adc_test()
{
    auto res  = decode(ch0);
    // Serial.println(res.second);
    return res.second;
}