#ifndef MONGOOSEINTF_HEADER_INCLUDED
#define  MONGOOSEINTF_HEADER_INCLUDED

// #define USE_LUA // Optional - delete to miss out LUA

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int start_mongoose(char *argv[]);
int stop_mongoose();
const char *mongoose_get_option(const char *name);

char *sdup(const char *str);
char* urldecode(const char *string);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
