#include "show_ccm_registers.hpp"

void show_ipg_clk() {
    static const uint32_t pre_periph_clk[] = {
        528000, // PLL2
        396000, // PLL2 PFD2
        352000, // PLL2 PFD0
        F_CPU // PLL1
    };
    PRREG(CCM_ANALOG_PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK);
    PRREG(CCM_CACRR & CCM_CACRR_ARM_PODF_MASK);
    PRREG((CCM_CBCMR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK) >> 18);
    PRREG((CCM_CBCDR & CCM_CBCDR_PERIPH_CLK_SEL) >> 25); 
    PRREG((CCM_CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> 10);
    PRREG((CCM_CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> 8); 
}

void show_lpspi_clk() {
    static const uint32_t pre_periph_clk[] = {
        664620, // PLL3
        720000, // PLL3 PFD0
        528000, // PLL2
        396000 // PLL2 PFD2
    };
    PRREG((CCM_CBCMR & CCM_CBCMR_LPSPI_CLK_SEL_MASK) >> 4);
    PRREG((CCM_CBCMR & CCM_CBCMR_LPSPI_PODF_MASK) >> 26);
}


void set_arm_clk_div(uint8_t new_div) {
    CCM_CBCMR &= (~CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK); // switch to PLL2(528MHz)
    CCM_ANALOG_PLL_ARM = (CCM_ANALOG_PLL_ARM & ~CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) | CCM_ANALOG_PLL_ARM_DIV_SELECT(new_div);
    CCM_CACRR = (CCM_CACRR & (~CCM_CACRR_ARM_PODF_MASK)) | CCM_CACRR_ARM_PODF(0);
    CCM_CBCMR |= CCM_CBCMR_PRE_PERIPH_CLK_SEL(3); 
}