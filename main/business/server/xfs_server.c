#include "xfs_server.h"

#define QUEUE_LEN 1
#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG
#define RET_OK "{\"ErrCode\":0}"
#define RET_ERR "{\"ErrCode\":1}"

static xQueueHandle g_recv = NULL;
/* An HTTP POST handler */


static esp_err_t speak_post_handler(httpd_req_t *req)
{
    char *buf = STD_CALLOC(req->content_len + 1, 1);;
    int ret = httpd_req_recv(req, buf, req->content_len + 1);
    httpd_resp_set_type(req, "application/json");
    if(ret > 0)
    {
        httpd_resp_send_chunk(req, RET_OK, strlen(RET_OK));
        xQueueSend(g_recv, &buf, 0);
    }
    else
    {
        STD_FREE(buf);
        STD_LOGE("recv err[%d]", ret);
        httpd_resp_send_chunk(req, RET_ERR, strlen(RET_ERR));
    }
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
static const httpd_uri_t speak_handle = {
    .uri       = SPEAK_PATH,
    .method    = HTTP_POST,
    .handler   = speak_post_handler,
    .user_ctx  = NULL
};

static esp_err_t info_post_handler(httpd_req_t *req)
{
    char *buf = STD_CALLOC(req->content_len + 1, 1);;
    int ret = httpd_req_recv(req, buf, req->content_len + 1);
    httpd_resp_set_type(req, "application/json");
    if(ret > 0)
    {
        char str[100] = {0};
        char *version = std_ota_version();
        json_pack_object(str, "Version", &version);
        int err_code = 0;
        json_pack_object(str, "ErrCode", &err_code);

        char *url = STD_CALLOC(100, 1);
        std_nvs_load(HB_SERVER_KEY, url, 100);
        json_pack_object(str, "Server", &url);
        STD_FREE(url);
        httpd_resp_send_chunk(req, str, strlen(str));
    }
    else
    {
        STD_LOGE("recv err[%d]", ret);
        httpd_resp_send_chunk(req, RET_ERR, strlen(RET_ERR));
    }
    httpd_resp_send_chunk(req, NULL, 0);
    STD_FREE(buf);
    return ESP_OK;
}
static const httpd_uri_t info_handle = {
    .uri       = INFO_PATH,
    .method    = HTTP_POST,
    .handler   = info_post_handler,
    .user_ctx  = NULL
};


static esp_err_t ota_post_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    char *buf = STD_CALLOC(req->content_len + 1, 1);;
    int ret = httpd_req_recv(req, buf, req->content_len + 1);
    char res_buf[100] = {0};
    int err_code = 0;
    char *msg = "no err";
    if(ret <= 0)
    {
        err_code = 1;
        goto FAIL;
    }
    else
    {
        char *path = NULL;
        json_parse_object(buf, "Path", &path);
        if(path == NULL)
        {
            msg = "path parse fail";
            err_code = 2;
            json_pack_object(res_buf, "ErrMessage", &msg);
            goto FAIL;           
        }
        std_ota_set_url(path);
        json_free(path, 0);
        std_ota_update();
        msg = "ota fail, check ota path and version";
        err_code = 3;
    }
FAIL:
    STD_FREE(buf);
    json_pack_object(res_buf, "ErrCode", &err_code);
    json_pack_object(res_buf, "ErrMessage", &msg);
    httpd_resp_send_chunk(req, res_buf, strlen(res_buf));
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t ota_handle = {
    .uri       = OTA_PATH,
    .method    = HTTP_POST,
    .handler   = ota_post_handler,
    .user_ctx  = NULL
};


static esp_err_t change_post_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    char *buf = STD_CALLOC(req->content_len + 1, 1);;
    int ret = httpd_req_recv(req, buf, req->content_len + 1);
    char res_buf[100] = {0};
    int err_code = 0;
    char *msg = "no err";
    if(ret <= 0)
    {
        err_code = 1;
        goto FAIL;
    }
    else
    {
        char *server = NULL;
        json_parse_object(buf, "Server", &server);
        if(server == NULL)
        {
            msg = "server parse fail";
            err_code = 2;
            json_pack_object(res_buf, "ErrMessage", &msg);
            goto FAIL;           
        }
        static char url[100];
        strcpy(url, server);
        json_free(server, 0);
        std_nvs_save(HB_SERVER_KEY, url, 100);
    }
FAIL:
    STD_FREE(buf);
    json_pack_object(res_buf, "ErrCode", &err_code);
    json_pack_object(res_buf, "ErrMessage", &msg);
    httpd_resp_send_chunk(req, res_buf, strlen(res_buf));
    httpd_resp_send_chunk(req, NULL, 0);
    if(err_code == 0)
        std_reboot(500);
    return ESP_OK;
}

static const httpd_uri_t change_handle = {
    .uri       = CHANGE_PATH,
    .method    = HTTP_POST,
    .handler   = change_post_handler,
    .user_ctx  = NULL
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = SERVER_PORT;

    // Start the httpd server
    STD_LOGI("Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        STD_LOGI("Registering URI handlers");
        httpd_register_uri_handler(server, &speak_handle);
        httpd_register_uri_handler(server, &ota_handle);
        httpd_register_uri_handler(server, &info_handle);
        httpd_register_uri_handler(server, &change_handle);
        return server;
    }

    STD_LOGE("Error starting server!");
    return NULL;
}

char *wait_http_req(int time_ms)
{
    char *buf = NULL;
    xQueueReceive(g_recv, &buf, time_ms / portTICK_PERIOD_MS);
    return buf;
}

void http_server_init()
{
    g_recv = xQueueCreate(QUEUE_LEN, sizeof(char *));
    start_webserver();
}

void free_http_req(char *recv)
{
    STD_FREE(recv);
}