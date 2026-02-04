#include "Turbo-Base64/turbob64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) return -1;
    if (!*lineptr) {
        *n = 128;
        *lineptr = malloc(*n);
    }
    size_t pos = 0;
    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            *n *= 2;
            char *tmp = realloc(*lineptr, *n);
            if (!tmp) return -1;
            *lineptr = tmp;
        }
        (*lineptr)[pos++] = c;
        if (c == '\n') break;
    }
    if (pos == 0) return -1;
    (*lineptr)[pos] = '\0';
    return pos;
}
#endif

static const char thumb_begin[] = "; thumbnail begin";
static const char thumb_end[] = "; thumbnail end\n";

void gcodeimg_to_file(char *input, char *output) {
  FILE *fp = fopen(input, "r");
  if (fp == NULL) {
    perror("Failed to open input file");
    exit(EXIT_FAILURE);
  }

  unsigned char *b64 = NULL;
  char *line = NULL;
  size_t len = 0;
  int thumbnail_begin = 0;
  int total_len = 0;

  while ((getline(&line, &len, fp)) != -1) {
    // Skip everything until the start marker
    if (thumbnail_begin == 0) {
      if (strncmp(line, thumb_begin, sizeof(thumb_begin) - 1) == 0) {
        thumbnail_begin = 1;
      }
      continue;
    }

    // Stop at the end marker
    if (strcmp(line, thumb_end) == 0) {
      break;
    }

    size_t current_line_len = strlen(line);

    // Ensure line is long enough to strip prefix/suffix ("; " and "\n")
    // Adjust '3' based on your specific G-code comment format
    if (current_line_len < 4)
      continue;
    int data_len = current_line_len - 3;

    // Safe realloc pattern
    unsigned char *tmp = realloc(b64, total_len + data_len);
    if (tmp == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      free(b64);
      free(line);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    b64 = tmp;

    // Copy data (skipping the leading "; ")
    memcpy(b64 + total_len, &line[2], data_len);
    total_len += data_len;
  }

  if (total_len > 0) {
    size_t out_capacity = (total_len * 3 / 4) + 8;
    unsigned char *out = malloc(out_capacity);

    if (out == NULL) {
      fprintf(stderr, "Failed to allocate output buffer\n");
    } else {
      // Decode and write
      tb64xdec(b64, total_len, out);

      FILE *fptr = fopen(output, "wb");
      if (fptr) {
        fwrite(out, out_capacity, 1, fptr);
        fclose(fptr);
      } else {
        perror("Failed to open output file");
      }
      free(out);
    }
  }

  // Cleanup
  if (line)
    free(line);
  if (b64)
    free(b64);
  fclose(fp);
}

static void usage(void) {
  puts("usage: gcode-preview [-i inputfile] [-o outputfile]");
}

int main(int argc, char *argv[]) {
  char *input = NULL;
  char *output = NULL;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-v")) {
      puts("gcode-preview-0.2");
      return EXIT_SUCCESS;
    } else if (!strcmp(argv[i], "-h")) {
      usage();
      return EXIT_SUCCESS;
    } else if (!strcmp(argv[i], "-i") && i + 1 < argc) {
      input = argv[++i];
    } else if (!strcmp(argv[i], "-o") && i + 1 < argc) {
      output = argv[++i];
    }
  }

  if (!input || !output) {
    usage();
    return EXIT_FAILURE;
  }

  gcodeimg_to_file(input, output);
  return EXIT_SUCCESS;
}
