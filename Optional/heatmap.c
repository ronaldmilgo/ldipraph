/**
 * Main driver for heatmap program.
 *
 * @author CPSC 223 Staff and you
 */

// see the header file for suggestions on how the suggested functions behave
// (but this and heatmap.c are in Optional, so you can do whatever you want
// or need to do with any of this code -- that goes for track.c as well)
#include "heatmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#include "track.h"
#include "trackpoint.h"
#include "location.h"

int main(int argc, char **argv)
{
  command_line args;
  if (!validate_command_line(argc, argv, &args))
    {
      fprintf(stderr, "%s: invalid parameter\n", argv[0]);
      return 1;
    }

  track *trk = track_create();
  if (trk == NULL)
    {
      fprintf(stderr, "%s: could not create track\n", argv[0]);
      return 1;
    }

  if (!read_input(stdin, trk))
    {
      track_destroy(trk);
      return 1;
    }

  // print the heatmap
  if (!show_heatmap(stdout, trk, &args))
    {
      track_destroy(trk);
      return 1;
    }

  track_destroy(trk);
  return 0;
}

// ./Heatmap 0.010 0.010 .:xX@# 1 < /c/cs223/hw3/Tests/example.in
// typedef struct {
//   double cell_width;
//   double cell_height;
//   const char *colors; // or "colors" -- no scare quotes in C identifiers!
//   size_t num_colors;
//   size_t range_width;
// } command_line;

bool validate_command_line(int argc, char **argv, command_line *args)
{
  // TO DO: set the fields of the struct args points to according to
  // the contents of the command line, checking for validity along the
  // way
  if (argc != 5){
    return false;
  }
  // parse width, height, colors, num_colors, range_width
  args->cell_width = atof(argv[1]);
  args->cell_height = atof(argv[2]);
  args->colors = argv[3];
  args->num_colors = strlen(argv[3]);
  args->range_width = atoi(argv[4]);

  if (args->cell_width <= 0.0 || args->cell_width > 360.0 
  || args->cell_height <= 0.0 || args->cell_height > 180.0
  || args->num_colors <= 0 || args->range_width <= 0){
    return false;
  }
  return true;
}
 
bool read_input(FILE *in, track *trk)
{
  // THIS WORKS FOR VALID INPUTS WITH ONLY ONE SEGMENT
  // TO DO: checking for blank lines that separate segments and making sure
  // no crashes or infinite loops on invalid input
  
  double lat;
  double lon;
  long time;

  char *line = NULL;
  size_t length = 0;
  ssize_t read;

  bool valid_point_found = false;

  while ((read = getline(&line, &length, in)) != -1){//read up until we hit a blank line- returns -1 if EOF

    while(read > 0 && line[read - 1] == '\n'){
      line[--read] = '\0'; //replace with this null-terminater
    }
    if(strlen(line) == 0){
      track_start_segment(trk);
      // printf("NEW SEGMENT CREATED!\n");
      continue;
    }

    if (sscanf(line, "%lf %lf %ld", &lat, &lon, &time) != 3){
      fprintf(stderr, "Invalid input line\n");
      continue;
    }

    trackpoint *pt = trackpoint_create(lat, lon, time);
    if (pt == NULL)
    {
      fprintf(stderr, "could not create point %f %f %ld\n", lat, lon, time);
      free(line);
      return false;
    }
    track_add_point(trk, pt);
    // printf("POINT ADDED: %lf %lf %ld\n", lat, lon, time);

      // track made its own copy, so we destroy ours
    trackpoint_destroy(pt);
    valid_point_found = true;
    }
  free(line);
  // printf("DONE!\n");
  if (!valid_point_found){
    return false;
  }
  return true;
}

bool show_heatmap(FILE *out, const track *trk, const command_line *args)
{
  // TO DO: ask the track for a heatmap and print it!
  size_t **heatmap;
  size_t rows;
  size_t cols;
  track_heatmap(trk, args->cell_width, args->cell_height, &heatmap, &rows, &cols);

  if (heatmap == NULL){
    fprintf(stderr, "Heatmap not created/n");
    return false;
  }

  for (size_t i = 0; i < rows; i++){
    for (size_t j = 0; j < cols; j++){
      size_t trkpoint_count = heatmap[i][j];
      size_t color_index = (trkpoint_count >= ((args->num_colors - 1) * args->range_width) ? 
      (args->num_colors - 1) : (trkpoint_count/args->range_width));
      fputc(args->colors[color_index], out);
    }
    fputc('\n', out);
  }
  //free heatmap memory
  for (size_t i = 0; i < rows; i++){
    free(heatmap[i]);
  }
  free(heatmap);
  return true;
}
