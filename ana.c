/*
 * Ana. Simple anagram/dictionnary finder
 * (c) 2016 Joachim Naulet <jnaulet@rdinnovation.fr>
 */

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFLEN 512
#define MAX_LEN 50

static void usage(char *argv) {

  fprintf(stderr, "%s -f dictionnary -s min_size " \
	  "-l max_size <pattern>\n", argv);
}

static int tolcaseascii(int c) {

  static char a[] = { "àâä" };
  static char e[] = { "éèêë" };
  static char i[] = { "îï" };
  static char o[] = { "ôö" };
  static char u[] = { "ùûü" };
  static char y[] = { "ŷÿ" };

  /* Lower */
  c = tolower(c);
  
  if(memchr(a, c, sizeof a)) return 'a';
  if(memchr(e, c, sizeof e)) return 'e';
  if(memchr(i, c, sizeof i)) return 'i';
  if(memchr(o, c, sizeof o)) return 'o';
  if(memchr(u, c, sizeof u)) return 'u';
  if(memchr(y, c, sizeof y)) return 'y';

  return c;
}

static int char_exists(char c, const char *pattern, size_t psize) {

  int i;
  for(i = psize; i--;){
    if(!pattern[i]) continue;
    if(pattern[i] == '*' ||
       tolcaseascii(c) == tolower(pattern[i]))
      return i;
  }
  
  return -1;
}

static int find_pattern(const char *buf, size_t bsize,
			const char *pattern, size_t psize,
			size_t max_len) {
  
  int score = 0, i;
  char lpattern[BUFLEN];
  strncpy(lpattern, pattern, psize);

  if(bsize > max_len)
    goto out;
  
  for(i = bsize; i--;){
    int index = char_exists(buf[i], lpattern, psize);
    if(index != -1){
      lpattern[index] = 0;
      score++;
    }
  }

 out:
  return score;
}

int main(int argc, char **argv) {

  FILE *fp;
  int c, psize;
  char buf[BUFLEN];
  char *filename = NULL, *pattern;
  
  int max_len = -1;
  int min_score = -1;
  
  while((c = getopt(argc, argv, "f:s:l:")) != -1){
    switch(c){
    case 'f' : filename = optarg; break;
    case 's' : min_score = atoi(optarg); break;
    case 'l' : max_len = atoi(optarg); break;
    }
  }
  
  /* Find pattern & set options */
  pattern = argv[optind];

  if(!filename || !pattern){
    usage(argv[0]);
    goto out;
  }  
  
  psize = strlen(pattern);
  max_len = ((max_len < 0) ? MAX_LEN : max_len);
  min_score = ((min_score < 0) ? psize : min_score);
  
  if(!(fp = fopen(filename, "r"))){
    perror(filename);
    goto out;
  }

  /* Main loop */
  
  while(fgets(buf, sizeof(buf), fp)){
    /* Remove terminating carriage return if found */
    char *carriage = strrchr(buf, '\n');
    if(carriage) *carriage = '\0';
    /* Find algorithm here */
    int size = strlen(buf);
    int score = find_pattern(buf, size, pattern, psize, max_len);
    if(score >= min_score)
      printf("[%d/%d] %s\n", score, size, buf);
  }

  fclose(fp);
  return 0;
  
 out:
  return -1;
}
