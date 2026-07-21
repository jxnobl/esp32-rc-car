#include "webserver.h"
#include "webpage.h"

#include "esp_http_server.h"
#include "esp_log.h"

static const char *TAG = "webserver";

static esp_err_t index_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, webpage, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.server_port = 80;

    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP Server Started");

        httpd_uri_t index = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = index_handler,
            .user_ctx  = NULL
        };

        httpd_register_uri_handler(server, &index);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to start HTTP Server");
    }
}