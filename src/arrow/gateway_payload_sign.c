#include "arrow/gateway_payload_sign.h"
#include "arrow/sign.h"
#include "crypt/crypt.h"
#include <arrow/mem.h>

#define INIT_CANONICAL_REQUEST

#if defined(DYNAMIC_SIGN_MEMORY)
#define GET_ADDR (char *)malloc(512);
#else
static char canonical_request_memory[512];
#define GET_ADDR canonical_request_memory;
#endif

int gateway_payload_sign(char *signature,
                         const char *hid,
                         const char *name,
                         int encrypted,
                         const char *canParString,
                         const char *signatureVersion) {
  // step 1
  char *canonicalRequest = GET_ADDR;
  strcpy(canonicalRequest, hid);
  strcat(canonicalRequest, "\n");
  strcat(canonicalRequest, name);
  strcat(canonicalRequest, "\n");
  if ( encrypted ) strcat(canonicalRequest, "true\n");
  else strcat(canonicalRequest, "false\n");
  strcat(canonicalRequest, canParString);
  strcat(canonicalRequest, "\n");
  char hex_hash_canonical_req[66];
  char hash_canonical_req[34];
  sha256(hash_canonical_req, canonicalRequest, (int)strlen(canonicalRequest));
  hex_encode(hex_hash_canonical_req, hash_canonical_req, 32);
  hex_hash_canonical_req[64] = '\0';
  printf("can: %s\r\n", hex_hash_canonical_req);

  // step 2
  char *stringtoSign = GET_ADDR;
  strcpy(stringtoSign, hex_hash_canonical_req);
  strcat(stringtoSign, "\n");
  strcat(stringtoSign, get_api_key());
  strcat(stringtoSign, "\n");
  strcat(stringtoSign, signatureVersion);

  // step 3
  char tmp[128];
  char hex_tmp[128];

  printf("api: %s\r\n", get_api_key());
  printf("sec: %s\r\n", get_secret_key());
  hmac256(tmp, get_api_key(), (int)strlen(get_api_key()), get_secret_key(), (int)strlen(get_secret_key()));
  hex_encode(hex_tmp, tmp, 32);
  printf("hex1: %s\r\n", hex_tmp);
  memset(tmp, 0, 128);
  hmac256(tmp, signatureVersion, (int)strlen(signatureVersion), hex_tmp, (int)strlen(hex_tmp));
  hex_encode(hex_tmp, tmp, 32);
  printf("hex2: [%d]%s\r\n", strlen(hex_tmp), hex_tmp);

  printf("sig str: [%d][\n%s\n]\r\n", strlen(stringtoSign), stringtoSign);

  hmac256(tmp, hex_tmp, strlen(hex_tmp), stringtoSign, strlen(stringtoSign));
  hex_encode(signature, tmp, 32);
  printf("sig: [%d]%s\r\n", strlen(signature), signature);

  return 0;
}