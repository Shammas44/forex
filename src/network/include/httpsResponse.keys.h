#ifndef HTTPSRESPONSE_KEYS_H
#define HTTPSRESPONSE_KEYS_H

typedef enum {
  Protocol_Name,
  Protocol_Version,
  Status_Code,
  Status_Message,
  Headers,
  Body,
  Content_Type,
  Content_Length,
  Method_Name,
  Path,
  Connection,
  Upgrade,
  Sec_WebSocket_Key,
  Sec_WebSocket_Version,
} HttpsResponseKey;


static char *httpsresponsekeys[]={
"protocol_name",
"protocol_version",
"status_code",
"status_message",
"headers",
"body",
"Content-Type",
"Content-Length",
"method_name",
"path",
"Connection",
"Upgrade",
"Sec-WebSocket-Key",
"Sec-WebSocket-Version",
};

#define KEY(index) \
({ \
  char* _key = httpsresponsekeys[index]; \
  _key; \
})

#endif
