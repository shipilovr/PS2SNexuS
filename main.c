#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"


/* 
    ----------------------------------------------------------------------------------------------------
    Mod's states
    ----------------------------------------------------------------------------------------------------
*/
typedef enum {
    STATE_HW_INIT,          // Hardware initialization (Ethernet, GPIO, PIO)
    STATE_NET_DISCOVERY,    // Handshake with the udpfsd server
    STATE_READY,            // Ready for PS2 state 
    STATE_XFER_MMCEMAN,     // Трансляция файловых команд лаунчера (меню, списки ISO)
    STATE_XFER_MMCEDRV,     // Трансляция сырых блоков запущенной игры (стриминг секторов)
    STATE_HALT_ERROR        // Критическая ошибка / Останов
} mod_state_t;

//Current global state
volatile mod_state_t g_current_state = STATE_HW_INIT;

/* 
    ----------------------------------------------------------------------------------------------------
    Core 1 section - PS2 listening
    ----------------------------------------------------------------------------------------------------
*/

// Ядро 1 занимается ИСКЛЮЧИТЕЛЬНО быстрой физикой SPI шины PS2
void core1_entry() {
    // Ждем, пока Ядро 0 поднимем сеть и переведет автомат в рабочий режим
    while (g_current_state != STATE_READY) {
        tight_loop_contents();
    }
    
    // Запуск бесконечного PIO/SPI перехватчика команд MMCE
    ps2_bridge_listen_loop(); 
}


/* 
    ----------------------------------------------------------------------------------------------------
    Main Core 0 section - network & logics
    ----------------------------------------------------------------------------------------------------
*/
int main() {

    stdio_init_all();
    
    while (true) {
        switch (g_current_state) {
            
            case STATE_HW_INIT:
                printf("[INIT] Starting mod's hardware...\n");
                // .....                
                g_current_state = STATE_NET_DISCOVERY;
                break;
                
            case STATE_NET_DISCOVERY:
                printf("[NET] Discovery handshake...\n");
                // .....
                g_current_state = STATE_READY;
                break;

            case STATE_READY:
                printf("[READY] Ready for PS2\n");

            case STATE_HALT_ERROR:
                printf("[ERROR] Something going wrong. Mod's restart...\n");
                // .....
                g_current_state = STATE_HW_INIT; // Пробуем инициализацию заново
                break;
        }
    }
    return 0;
}
