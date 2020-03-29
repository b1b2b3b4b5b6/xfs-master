#include "serial.h"

#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG
#define EX_UART_NUM UART_NUM_1


#define RD_BUF_SIZE (1024)
#define RECV_QUEUE_SIZE 8
#define EVENT_QUENE_LENGTH 10
static xQueueHandle g_uart_queue;

void serial_send(char *data, int len)
{
    uart_write_bytes(EX_UART_NUM, data, len);
}

static void uart_event_task(void *pvParameters)
{
    STD_LOGD("uart event task start");
    uart_event_t event;
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(g_uart_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    STD_LOGD( "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    STD_LOGD( "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    STD_LOGD( "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    STD_LOGD( "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    STD_LOGD( "uart frame error");
                    break;
                //UART_PATTERN_DET 
                case UART_PATTERN_DET:
                    break;
                //Othersp
                default:
                    STD_LOGD( "uart event type: %d", event.type);
                    break;
            }
        }
    }

    vTaskDelete(NULL);
}

int serial_init()
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = XFS_BAND_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(EX_UART_NUM, &uart_config));

    //Set UART pins (using UART0 default pins ie no changes.)
    ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, XFS_TX, XFS_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    //Install UART driver, and get the queue.
    ESP_ERROR_CHECK(uart_driver_install(EX_UART_NUM, RD_BUF_SIZE, 0, EVENT_QUENE_LENGTH, &g_uart_queue, 0));

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, RECV_QUEUE_SIZE, NULL);

    STD_LOGI("xfs serial init success");

    return 0;
}
