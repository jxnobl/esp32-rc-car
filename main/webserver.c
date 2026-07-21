#include "webserver.h"
#include "webpage.h"
#include "motor.h"
#include "battery.h"

#include "esp_http_server.h"
#include "esp_log.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *TAG = "WEBSERVER";

static uint8_t currentSpeed = 50;

/* ===========================
   Handlers
   =========================== */

static esp_err_t index_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, webpage, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t forward_handler(httpd_req_t *req)
{
    rear_forward(currentSpeed);

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Forward");

    return ESP_OK;
}

static esp_err_t backward_handler(httpd_req_t *req)
{
    rear_backward(currentSpeed);

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Backward");

    return ESP_OK;
}

static esp_err_t left_handler(httpd_req_t *req)
{
    steer_left();

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Left");

    return ESP_OK;
}

static esp_err_t right_handler(httpd_req_t *req)
{
    steer_right();

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Right");

    return ESP_OK;
}

static esp_err_t stop_handler(httpd_req_t *req)
{
    rear_stop();
    steer_stop();

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Stop");

    return ESP_OK;
}

static esp_err_t steerstop_handler(httpd_req_t *req)
{
    steer_stop();

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Steering Stop");

    return ESP_OK;
}

static esp_err_t speed_handler(httpd_req_t *req)
{
    char query[64];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK)
    {
        char value[16];

        if (httpd_query_key_value(query, "value", value, sizeof(value)) == ESP_OK)
        {
            int speed = atoi(value);

            if (speed < 0)
                speed = 0;

            if (speed > 100)
                speed = 100;

            currentSpeed = speed;

            ESP_LOGI(TAG, "Speed = %d%%", currentSpeed);
        }
    }

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t battery_handler(httpd_req_t *req)
{
    char response[64];

    sprintf(response,
            "{\"voltage\":%.2f,\"percent\":%d}",
            battery_voltage(),
            battery_percent());

    httpd_resp_set_type(req, "application/json");

    httpd_resp_send(req,
                    response,
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

/* ===========================
   Webserver
   =========================== */

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	config.server_port = 80;
	config.max_uri_handlers = 16;

    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP Server Started");

        httpd_uri_t index = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = index_handler
        };

        httpd_uri_t forward = {
            .uri = "/forward",
            .method = HTTP_GET,
            .handler = forward_handler
        };

        httpd_uri_t backward = {
            .uri = "/backward",
            .method = HTTP_GET,
            .handler = backward_handler
        };

        httpd_uri_t left = {
            .uri = "/left",
            .method = HTTP_GET,
            .handler = left_handler
        };

        httpd_uri_t right = {
            .uri = "/right",
            .method = HTTP_GET,
            .handler = right_handler
        };

        httpd_uri_t stop = {
            .uri = "/stop",
            .method = HTTP_GET,
            .handler = stop_handler
        };

        httpd_uri_t steerstop = {
            .uri = "/steerstop",
            .method = HTTP_GET,
            .handler = steerstop_handler
        };

        httpd_uri_t speed = {
            .uri = "/speed",
            .method = HTTP_GET,
            .handler = speed_handler
        };

        httpd_uri_t battery = {
            .uri = "/battery",
            .method = HTTP_GET,
            .handler = battery_handler
        };

		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &index));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &forward));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &backward));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &left));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &right));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &stop));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &steerstop));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &speed));
		ESP_ERROR_CHECK(httpd_register_uri_handler(server, &battery));

        ESP_LOGI(TAG, "Routes Registered");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to start HTTP Server");
    }
}