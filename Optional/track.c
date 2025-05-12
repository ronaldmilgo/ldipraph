/**
 * Starter code for the track ADT.
 * You're going to get crashes out of just about every test until you
 * fix track_add_point and track_get_point to at least respect the ownership
 * of the points passed back and forth, so fix that first!
 *
 * @author CPSC 223 Staff and you
 */

#include "track.h"
#include "trackpoint.h"

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

typedef struct
{
  // size_t num_pts;
  // trackpoint *pts[10]; // TO DO: make dynamically allocated to allow > 10 pts
  // TO DO: add whatever else you need here!
  double distance;
  size_t pts_capacity;
  size_t num_pts;
  trackpoint **pts;
} segment;

// TO DO: implement some more segment functions; keep things modular!
/**
 * Adds the given point to the given segment.
 *
 * @param seg a pointer to a segment, non-NULL
 * @param pt a pointer to a track point, non-NULL
 */
void segment_add_point(segment *seg, const trackpoint *pt);

struct track {
  // segment segments[1]; // TO DO: make dynamically allocated to allow > 1 seg
  // TO DO: add whatever other fields you need here!
  segment *segments; //pointer to an array of segments
  size_t num_segments; //no. of segments
  size_t seg_capacity; // capacity of the segment array
};

track *track_create()
{
  // TO DO: much below will need to change as you add fields to the structs
  // and change from static arrays to dynamically allocated arrays
  track *trk = malloc(sizeof(*trk));
  if(trk == NULL){
    return NULL;
  }

  // initialize track
  trk->seg_capacity = 3; //random capacity to set up
  trk->num_segments = 1;
  trk->segments = malloc(sizeof(segment) * trk->seg_capacity);
  if (trk->segments == NULL){
    free(trk);
    return NULL;
  }

  //initialize first segment struct in the array of segments
  trk->segments[0].num_pts = 0; //no points yet
  trk->segments[0].pts_capacity = 15; // creating space for 15 points initially
  trk->segments[0].distance = 0.0;
  trk->segments[0].pts = malloc(sizeof(trackpoint*) * trk->segments[0].pts_capacity);
  if (trk->segments[0].pts == NULL){
    free(trk->segments);
    free(trk);
    return NULL;
  }


  // trk->segments[0].num_pts = 0;
  return trk;
}

void track_destroy(track *trk)
{
  // TO DO: much will need to be added as you add fields to the structs
  // and change from static arrays to dynamically allocated arrays
  if (trk == NULL){
    return;
  }
  // freeing each segment's pts
  for (size_t i = 0; i < trk->num_segments; i++){
    if (trk->segments[i].pts != NULL){
      for (size_t j = 0; j < trk->segments[i].num_pts; j++){
        trackpoint_destroy(trk->segments[i].pts[j]);
        // in case of a dangling pointer
        trk->segments[i].pts[j] = NULL;
      }
      free(trk->segments[i].pts);
      trk->segments[i].pts = NULL;
    }
  }
  //freeing segments
  if (trk->segments != NULL){
    free(trk->segments);
    trk->segments = NULL;
  }
  //free track
  free(trk);
  trk = NULL;
}
                     
size_t track_count_segments(const track *trk)
{
  // TO DO: change this after you implement track_start_segment
  // return 1;
  if (trk == NULL){
    return 0;
  }
  // get number of segments
  size_t number_segments = trk->num_segments;
  return number_segments;
}

size_t track_count_points(const track *trk, size_t i)
{
  // TO DO: change this after you update to allow more than one segment
  return trk->segments[i].num_pts;
}

trackpoint *track_get_point(const track *trk, size_t i, size_t j)
{
  // TO DO: fix; this isn't even correct for one segment with one point
  // (read the documentation carefully)
  if (trk == NULL || i >= trk->num_segments){
    return NULL;
  }

  if(j >= trk->segments[i].num_pts){
    return NULL;
  }
  trackpoint *nw_pt = trackpoint_copy(trk->segments[i].pts[j]);
  return nw_pt;
}

double *track_get_lengths(const track *trk)
{
  // TO DO: implement this; this is just a "stub" to allow everything
  // to compile
  if(trk == NULL || trk->num_segments == 0){//if no segments
    return NULL;
  }

  double *lengths = malloc(sizeof(double) * trk->num_segments);
  if(lengths == NULL){
    return NULL;
  }

  for (size_t i = 0; i < trk->num_segments; i++){
    lengths[i] = trk->segments[i].distance;
  }

  return lengths;
}

void track_add_point(track *trk, const trackpoint *pt)
{
  // TO DO: update this to work as you change the structs to allow for
  // an unlimited number of points and segments
  if (trk == NULL || pt == NULL){
    return;
  }
  //if there is no segment yet, create one
  if(trk->num_segments == 0){
    trk->segments = malloc(sizeof(segment) * trk->seg_capacity);
    if (trk->segments == NULL){
      return;
    }
    trk->num_segments = 1;
    trk->segments[0].distance = 0.0;
    trk->segments[0].pts_capacity = 15;
    trk->segments[0].num_pts = 0;
    trk->segments[0].pts = malloc(sizeof(trackpoint*) * trk->segments[0].pts_capacity);
    if (trk->segments[0].pts == NULL){
      free(trk->segments);
      return;
    }
  }
  trackpoint *nw_pt = trackpoint_copy(pt);
  //add point
  segment_add_point(&trk->segments[trk->num_segments - 1], nw_pt);
  trackpoint_destroy(nw_pt);
  // segment_add_point(&trk->segments[0], pt);
}

void track_start_segment(track *trk)
{
  // TO DO: replace this stub
  // start a new segment within the track
  if (trk == NULL){
    return;
  }

  //check if array of segments needs resizing
  if (trk->num_segments == trk->seg_capacity){
    trk->seg_capacity *= 2;
    segment *new_segment = realloc(trk->segments, sizeof(segment) * trk->seg_capacity);
    if (new_segment == NULL){
      return;
    }
    trk->segments = new_segment;
  }
  //initializing the new segment
  trk->segments[trk->num_segments].distance = 0.0;
  trk->segments[trk->num_segments].pts_capacity = 15;
  trk->segments[trk->num_segments].num_pts = 0;
  trk->segments[trk->num_segments].pts = malloc(sizeof(trackpoint*) * trk->segments[trk->num_segments].pts_capacity);
  if(trk->segments[trk->num_segments].pts == NULL){
    return;
  }
  trk->num_segments += 1;
}

void track_merge_segments(track *trk, size_t start, size_t end)
{
  // TO DO: replace this stub too!
  if(trk == NULL || start >= end || end > trk->num_segments){
    return;
  }
  //number of points to be merged
  size_t count_points = trk->segments[start].num_pts;
  for (size_t i = start + 1; i < end; i++){
    count_points += trk->segments[i].num_pts;
  }
  //resize array in start
  trackpoint **nw_pts = realloc(trk->segments[start].pts, sizeof(trackpoint*) * count_points);
  if (nw_pts == NULL){
    return;
  }
  trk->segments[start].pts = nw_pts;
  
  //copy over the points from start +1 to end -1 to start
  for (size_t i = start + 1; i < end; i++){
    for(size_t j = 0; j < trk->segments[i].num_pts; j++){
      trk->segments[start].pts[(trk->segments[start].num_pts)++] = trk->segments[i].pts[j];
    }
    //add distance
    trk->segments[start].distance += trk->segments[i].distance;

    free(trk->segments[i].pts);
  }

  //move everything back to replace the merged segments
  for(size_t i = start + 1, j = end; j < trk->num_segments; i++, j++){
      trk->segments[i] = trk->segments[j];
  }
  //update num_segs
  // trk->num_segments -= (end - start - 1);
  trk->num_segments = trk->num_segments - (end - 1 - start); 
}

void track_heatmap(const track *trk, double cell_width, double cell_height,
		    size_t ***map, size_t *rows, size_t *cols)
{
  // TO DO: you got it -- this is yet another stub to replace
  double min_lat = 90.0;// for purposes of knowing the boundaries of our heat map
  double max_lat = -90.0;
  double min_lon = 180.0;
  double max_lon = -180.0;

  for(size_t i = 0; i < trk->num_segments; i++){
    for (size_t j = 0; j < trk->segments[i].num_pts; j++){
      location loc  = trackpoint_location(trk->segments[i].pts[j]);
      if (loc.lat < min_lat){
        min_lat = loc.lat; //update for the boundaries of our heatmap
      }
      if(loc.lat > max_lat){
        max_lat = loc.lat; 
      }

      // double normalized_lon = (loc.lon < 0) ? loc.lon + 360 : loc.lon;
  
      if(loc.lon > max_lon){
        max_lon = loc.lon;
      }
      if(loc.lon < min_lon){
        min_lon = loc.lon;
      }
    }
  }
  // double unnormalized_maxlon = (max_lon > 180) ? max_lon - 360 : max_lon;
  // double unnormalized_minlon = (min_lon > 180) ? min_lon - 360 : min_lon;

  if ((max_lon - min_lon) > 180){
    max_lon = 0.0;
    min_lon = 360;
    for(size_t i = 0; i < trk->num_segments; i++){
      for (size_t j = 0; j < trk->segments[i].num_pts; j++){
        location loc  = trackpoint_location(trk->segments[i].pts[j]);

        double normalized_lon = (loc.lon < 0) ? loc.lon + 360 : loc.lon;
      

        if(normalized_lon > max_lon){
          max_lon = normalized_lon;
        }
        if(normalized_lon < min_lon){
          min_lon = normalized_lon;
        }
      }
    }
  }
  
  // double without_wrapping = max_lon - min_lon;
  // double with_wrapping = (360 + min_lon) - max_lon;


  
  // if (with_wrapping < without_wrapping){
  //   min_lon += 360; 
  // }
  // if (with_wrapping < without_wrapping){
  //   double temp = unnormalized_minlon;
  //   min_lon = unnormalized_maxlon;
  //   max_lon = temp;
  // }

  double min_lon_cpy = min_lon;
  
  //num of rows&columns
  double total_height = max_lat - min_lat;
  double total_width = fabs(max_lon - min_lon);
  double row_num = ceil(total_height/cell_height);
  double column_num = ceil(total_width/cell_width);
  *rows = (size_t)row_num;
  *cols = (size_t)column_num;

  //memory allocation to our 2D-array
  *map = malloc(sizeof(size_t*) * (*rows)); //allocates memory to the array of ponters to the rows
  for(size_t i = 0; i < *rows; i++){
    (*map)[i] = malloc(sizeof(size_t) * (*cols)); //allocate memory for the cells while initializing to 0
    for(size_t j = 0; j < *cols; j++){
      (*map)[i][j] = 0; //initialize cells to 0
    }
  }
  //populate the heatmap
  for(size_t i = 0; i < trk->num_segments; i++){
    for (size_t j = 0; j < trk->segments[i].num_pts; j++){
      location loc = trackpoint_location(trk->segments[i].pts[j]);
      
      double adjusted_lon = loc.lon < min_lon_cpy ? loc.lon + 360.0 : loc.lon;
      //indexing top to bottom and left to right
      double raw_row = ((max_lat - loc.lat)/cell_height);
      size_t row = (size_t)raw_row;
      //if point lies on the boundary lat of our map
      if (row == *rows && !(raw_row > *rows)){
        row = *rows - 1;
      }
      double raw_column = fabs((adjusted_lon - min_lon)/cell_width);
      size_t column = (size_t)raw_column;
      //if point lies on the boundary lon
      if (column == *cols && !(raw_column > *cols)){
        column = *cols - 1;
      }
      //incremnt count
      (*map)[row][column] += 1;
    }
  }
}


void segment_add_point(segment *seg, const trackpoint *pt)
{
  // TO DO: allow for an arbitrary number of points per segment, and fix
  // that compiler warning about const (read the documentation of
  // track_add_point carefully)
  if (seg == NULL || pt == NULL){
    return;
  }

  if (seg->num_pts == seg->pts_capacity){ // if true then we need to resize array
    seg->pts_capacity *= 2; //double capacity
    trackpoint **nw_pts = realloc(seg->pts, sizeof(trackpoint*) * seg->pts_capacity);
    if (nw_pts == NULL){
      return;
    }
    seg->pts = nw_pts; //update array
  }
  trackpoint *nw_pt = trackpoint_copy(pt);
  if (nw_pt == NULL){
    return;
  }

  seg->pts[seg->num_pts] = nw_pt;

  //update distance if point if there was a point prior
  if (seg->num_pts > 0){
    trackpoint *last_point = seg->pts[seg->num_pts - 1];
    location last_loc = trackpoint_location(last_point);
    location nw_loc = trackpoint_location(pt);
    double new_dist = location_distance(&last_loc, &nw_loc);
    seg->distance += new_dist; 
  }
  
  seg->num_pts++;
}  
