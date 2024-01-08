#include "Turbo-Base64/turbob64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char thumb_begin[] = "; thumbnail begin";
static const char thumb_end[] = "; thumbnail end\n";

void gcodeimg_to_file(char *input, char *output) {
  // open input file for reading
  FILE *fp = fopen(input, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file\n");
    exit(EXIT_FAILURE);
  }

  unsigned char *b64;
  char *line = NULL;
  size_t len = 0;
  int thumbnail_begin = 0;
  int line_len;
  int total_len = 0;

  // read line
  while ((getline(&line, &len, fp)) != -1) {
    line_len = strlen(line) - 3;

    // skip everything until we find the magic string
    if (thumbnail_begin == 0) {
      thumbnail_begin =
          strncmp(line, thumb_begin, sizeof(thumb_begin) - 1) == 0;
      continue;
    }

    // stop processing when we find the magic string
    if (!strcmp(line, thumb_end))
      break;

    // copy base64 string into b64
    b64 = realloc(b64, (total_len + line_len) * sizeof(unsigned char));
    memcpy(b64 + total_len, &line[2], line_len);
    total_len += line_len;
  }

  // allocate 3/4th the memory for the output
  unsigned char out[(total_len * 3 / 4) + 8];

  // decode b64 to out
  tb64xdec(b64, total_len, out);

  // write output
  FILE *fptr = fopen(output, "w");
  fwrite(out, sizeof(out), 1, fptr);

  // cleanup
  fclose(fp);
  if (line)
    free(line);
  if (b64)
    free(b64);
}

static void usage(void) {
  puts("usage: gcode-preview [-i inputfile] [-o outputfile]");
}

int main(int argc, char *argv[]) {
  int i;
  char *input;
  char *output;
  for (i = 1; i < argc; i++)
    /* these options take no arguments */
    if (!strcmp(argv[i], "-v")) { /* prints version information */
      puts("gcode-preview-0.1");
      exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[i], "-h")) { /* prints version information */
      usage();
      exit(EXIT_SUCCESS);
    }
    /* these options take one argument */
    else if (!strcmp(argv[i], "-i")) /* input filename */
      input = argv[++i];
    else if (!strcmp(argv[i], "-o")) /* input filename */
      output = argv[++i];

  if (!input) {
    puts("input file not specified");
    exit(EXIT_FAILURE);
  }
  if (!output) {
    puts("output file not specified");
    exit(EXIT_FAILURE);
  }

  gcodeimg_to_file(input, output);
  exit(EXIT_SUCCESS);
}
