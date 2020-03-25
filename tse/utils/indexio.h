#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <webpage.h>
#include <hash.h>
#include <queue.h>

int32_t indexsave(hashtable_t *word_htable, char *file);

hashtable_t* indexload(char *file);
