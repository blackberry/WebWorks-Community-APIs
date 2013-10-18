// Copyright (c) 2004-2013 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _XOPEN_SOURCE 600  // For PATH_MAX on linux

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

#include "mongoose.h"
#include "mongooseintf.h"

#include <sys/wait.h>
#include <unistd.h>
#define DIRSEP '/'

#define MAX_OPTIONS 53
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

static char server_name[40];        // Set by init_server_name()
static struct mg_context *mgctx;      // Set by start_mongoose()

#define ISXDIGIT(x) (isxdigit((int) ((unsigned char)x)))

char* urldecode(const char *string) {
  size_t alloc = strlen(string)+1;
  char *ns = malloc(alloc);
  unsigned char in;
  size_t strindex=0;
  unsigned long hex;
  bool isquote=false;

  if(!ns)
    return NULL;

  while(--alloc > 0) {
    isquote = false;
    in = *string;
    if(('%' == in) && (alloc > 2) &&
       ISXDIGIT(string[1]) && ISXDIGIT(string[2])) {
      char hexstr[3];
      char *ptr;
      hexstr[0] = string[1];
      hexstr[1] = string[2];
      hexstr[2] = 0;
      if(strcmp(hexstr, "22")==0) {
    	  isquote = true;
      }
      hex = strtoul(hexstr, &ptr, 16);

      in = (unsigned char)(hex & (unsigned long) 0xFF);
      string+=2;
      alloc-=2;
    }

  if(!isquote) {
	  ns[strindex++] = in;
  }
  string++;
  }
  ns[strindex]=0;

  return ns;
}

int verify_document_root(const char *root) {
  const char *p, *path;
  char buf[PATH_MAX];
  struct stat st;

  path = root;
  if ((p = strchr(root, ',')) != NULL && (size_t) (p - root) < sizeof(buf)) {
    memcpy(buf, root, p - root);
    buf[p - root] = '\0';
    path = buf;
  }

  if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
	return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

char *sdup(const char *str) {
  char *p;
  if ((p = (char *) malloc(strlen(str) + 1)) != NULL) {
    strcpy(p, str);
  }
  return p;
}

int set_option(char **options, const char *name, const char *value) {
  int i;

  if (!strcmp(name, "document_root") || !(strcmp(name, "r"))) {
    if(verify_document_root(value)==EXIT_FAILURE) {
		return EXIT_FAILURE;
		}
  }

  for (i = 0; i < MAX_OPTIONS - 3; i++) {
    if (options[i] == NULL) {
      options[i] = sdup(name);
      options[i + 1] = sdup(value);
      options[i + 2] = NULL;
      break;
    }
  }

  if (i == MAX_OPTIONS - 3) {
	return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int process_command_line_arguments(char *argv[], char **options) {
  char line[MAX_CONF_FILE_LINE_SIZE], opt[sizeof(line)], val[sizeof(line)], *p;
  FILE *fp = NULL;
  size_t i, cmd_line_opts_start = 0, line_no = 0;
  int errs = 0;

  options[0] = NULL;

  // Handle command line flags.
  for (i = cmd_line_opts_start; argv[i] != NULL; i += 2) {
    if(set_option(options, &argv[i][0], &argv[i + 1][0]) == EXIT_FAILURE) {
	  errs++;
	}
  }
return errs;
}

static void init_server_name(void) {
  snprintf(server_name, sizeof(server_name), "Mongoose web server v. %s",
           mg_version());
}

int start_mongoose(char *argv[]) {
  struct mg_callbacks callbacks;
  char *options[MAX_OPTIONS];
  int i;
  int command_err_cnt = 0; // Returns 0 on success
  	  	  	  	  	  	   // <error count> if any commands were bad
  	  	  	  	  	  	   // 0 - (<error count> + 1) on error


  for(i=0; i<MAX_OPTIONS;i++)
  {
	  options[i] = NULL;
  }

  init_server_name();

  /* Update config based on command line arguments */
  command_err_cnt = process_command_line_arguments(argv, options);

  /* Start Mongoose */
  memset(&callbacks, 0, sizeof(callbacks));

//  callbacks.begin_request = begin_request_handler;

  mgctx = mg_start(&callbacks, NULL, (const char **) options);
  for (i = 0; options[i] != NULL; i++) {
    free(options[i]);
  }

  if (mgctx == NULL) {
	  command_err_cnt++;
	  command_err_cnt = 0 - command_err_cnt;
  }
  return command_err_cnt;
}

int stop_mongoose() {
  mg_stop(mgctx);
  return EXIT_SUCCESS;
}

const char *mongoose_get_option(const char *name) {
	return mg_get_option(mgctx, name);
}
