#include "v6o.hpp"

#include "esp_http_server.h"

#include "cJSON.h"


namespace v6o {

namespace {

constexpr size_t MAX_BODY_SIZE = 1024;

httpd_handle_t server = nullptr;

// Send a JSON response with the given status and body.
esp_err_t sendJson(httpd_req_t* req, const char* status, const char* json)
{
    httpd_resp_set_status(req, status);
    httpd_resp_set_type(req, "application/json");

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_sendstr(req, json);
}

// Parse the JSON body into a BrewProfile structure.
bool parseProfile(const char* body, BrewProfile& profile)
{
    // Parse the JSON body into a cJSON object
    cJSON* root = cJSON_Parse(body);

    if (!root)
        return false;

    // Parse the pours array
    cJSON* pours = cJSON_GetObjectItemCaseSensitive(root, "pours");

    if (!cJSON_IsArray(pours)) {
        cJSON_Delete(root);
        return false;
    }

    // Get the number of pours
    int count = cJSON_GetArraySize(pours);

    // Validate the number of pours
    if (count <= 0 || count > MAX_POURS) {
        cJSON_Delete(root);
        return false;
    }

    // Initialize the profile
    profile = {};
    profile.pourCount = count;

    // Parse each pour step
    for (int i = 0; i < count; i++) {

        cJSON* pour = cJSON_GetArrayItem(pours, i);

        if (!cJSON_IsObject(pour)) {
            cJSON_Delete(root);
            return false;
        }

        // Get the wait and duration values in seconds
        cJSON* wait = cJSON_GetObjectItemCaseSensitive(pour, "wait_s");
        cJSON* duration = cJSON_GetObjectItemCaseSensitive(pour, "duration_s");

        if (!cJSON_IsNumber(wait) || !cJSON_IsNumber(duration))
        {
            cJSON_Delete(root);
            return false;
        }

        int waitSeconds = wait->valueint;
        int durationSeconds = duration->valueint;


        // Validate the values
        if (
            waitSeconds < 0     ||
            waitSeconds > 60    ||
            durationSeconds < 5 ||
            durationSeconds > 60
        ) {
            cJSON_Delete(root);
            return false;
        }


        // The wait time for the first pour must be 0.
        if (i == 0 && waitSeconds != 0) {
            cJSON_Delete(root);
            return false;
        }

        // Set the values in the profile
        profile.pours[i].waitBeforeMs = static_cast<uint32_t>(waitSeconds * 1000);
        profile.pours[i].durationMs = static_cast<uint32_t>(durationSeconds * 1000);
        
        profile.pours[i].pumpPulse = DEFAULT_PUMP_PULSE;
    }


    cJSON_Delete(root);

    return true;
}


esp_err_t brewHandler(httpd_req_t* req)
{
    if (req->content_len == 0 || req->content_len >= MAX_BODY_SIZE) {
        return sendJson(
            req,
            "413 Payload Too Large",
            R"({"error":"invalid body size"})"
        );
    }


    char body[MAX_BODY_SIZE];

    size_t received = 0;


    while (received < req->content_len) {

        int result = httpd_req_recv(
            req,
            body + received,
            req->content_len - received
        );


        if (result <= 0) {
            return sendJson(
                req,
                "400 Bad Request",
                R"({"error":"failed to read request"})"
            );
        }


        received += result;
    }


    body[received] = '\0';


    BrewProfile profile{};

    if (!parseProfile(body, profile)) {
        return sendJson(
            req,
            "400 Bad Request",
            R"({"error":"invalid profile"})"
        );
    }


    esp_err_t result = sendJson(
        req,
        "202 Accepted",
        R"({"status":"started"})"
    );

    onApiRequest(profile);

    return result;
}


const httpd_uri_t brewUri = {
    .uri = "/api/brew",
    .method = HTTP_POST,
    .handler = brewHandler,
    .user_ctx = nullptr
};

}


bool startApi()
{
    if (server)
        return true;


    httpd_config_t config =
        HTTPD_DEFAULT_CONFIG();


    if (httpd_start(
        &server,
        &config
    ) != ESP_OK) {
        return false;
    }


    if (httpd_register_uri_handler(server, &brewUri) != ESP_OK)
    {
        httpd_stop(server);
        server = nullptr;

        return false;
    }

    return true;
}

}
