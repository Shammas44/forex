#include "connectors.h"
#include "wsframe.old.h"

int connectors_get_auth(char **authorization) {
  char body1[] = "{\"email\":\"Admin.Admin@orif.ch\",\"password\":\"adad\"}";
  char *headers1[] = {
      "Connection: close",
      "Content-Type: application/json",
  };

  HttpRequest req1 = {
      .host = "127.0.0.1",
      .path = "/api/auth",
      .method = "POST",
      .port = "443",
      .body = body1,
      .body_length = sizeof(body1) > 1 ? sizeof(body1) - 1 : 0,
      .headers = headers1,
      .header_count = sizeof(headers1) / sizeof(headers1[0]),
  };

  HttpResponse res1;
  // int status = https_fetch(&req1, &res1);
  // if (status)
  //   return get_error("Failed to fetch");
  // status = http_get_authorization_from_res(&res1, authorization);
  // if (status)
  //   return get_error("failed to get authorization");
  return 0;
}

int connectors_read_websocket_frames(Mtqueue_list *q) {
  int status = 0;

  while (1) {
    char *out_message = NULL;
    status = wsframe_read_single_frame(q->ssl, &out_message);

    if (status != 0) {
      free(out_message);
      break;
    }

    jsmntok_t *tokens = NULL;
    int token_num;
    status = json_parse(out_message, JSMN_OBJECT, &tokens, &token_num);

    if (status != 0) {
      free(out_message);
      if (tokens) {
        free(tokens);
      }
      continue;
    }

    char type[20];
    char status[20];
    for (int i = 1; i < token_num; i++) {
      if (json_cmp_token_to_string(out_message, &tokens[i], "type") == 0) {
        json_extract_token(tokens, i, out_message, type, to_char);
        i++;
      }
      if (json_cmp_token_to_string(out_message, &tokens[i], "status") == 0) {
        json_extract_token(tokens, i, out_message, status, to_char);
        i++;
      }
    }

    if (strcmp(type, "TICKER") == 0) {
      Tick *tick = tick_create(out_message, &tokens, token_num);
      mtqueue_enqueue(q->ticks, tick);
    } else if (strcmp(type, "Market") == 0) {
      Order order = order_create(out_message, &tokens, token_num);
      mtqueue_enqueue(q->orders, &order);
    }

    free(out_message);
    free(tokens);
  }
  return status;
}



int connectors_establish_ws_handshake(SSL **ssl) {
  char *authorization = NULL;
  int status = connectors_get_auth(&authorization);
  if (status) {
    return get_error("failed to get authorization");
  }
  char body[] = "";
  char *headers[] = {
      "Upgrade: websocket",
      "Connection: Upgrade",
      "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==",
      "Sec-WebSocket-Version: 13",
      authorization,
  };

  HttpRequest postrequest = {
      .host = "127.0.0.1",
      .path = "/",
      .method = "GET",
      .port = "443",
      .body = "",
      .body_length = sizeof(body) > 1 ? sizeof(body) - 1 : 0,
      .headers = headers,
      .header_count = sizeof(headers) / sizeof(headers[0]),
  };

  HttpResponse res;
  status = ws_fetch(ssl, &postrequest, &res);
  if (status) {
    return get_error("failed to fetch");
  }
  return 0;
}
