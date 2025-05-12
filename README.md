# Heatmap Generator

This project implements a GPS heatmap generator in C. It reads track data from standard input and visualizes activity density across a 2D grid using ASCII characters. The solution uses dynamically allocated memory, an abstract data type (ADT) for tracks, and emphasizes efficient geographic computation.

## 🔥 Objective

- Use and implement an ADT with opaque structs  
- Work with dynamically allocated memory  
- Create a 2D heatmap from structured GPS data  

## 🗺️ Description

A **GPS track** is a time-ordered sequence of GPS points organized into multiple segments. This program allows:

- Adding points to the current segment  
- Starting new segments  
- Merging segments  
- Generating a heatmap by counting how many points fall into each cell of a grid  

The grid is defined by cell width/height in degrees, and each cell's density is mapped to a specific character representing a value range.

## 🧾 Input Format

### 📌 Command-line Arguments

```bash
./Heatmap <width> <height> <chars> <range_size> < input.txt
```

- `<width>` and `<height>`: Cell size in degrees of longitude and latitude  
- `<chars>`: A non-empty string mapping ranges to ASCII characters  
- `<range_size>`: The size of the range each character represents  
- Standard input: GPS data — latitude, longitude, timestamp (in ms), one point per line  
- Blank lines separate segments

### 📌 Example Input

```
-48.0020 121.0260 1500000000000
-48.0080 121.0220 1500010000000
-48.0120 121.0210 1500025000000

-48.0220 121.0000 1500201000000
-48.0225 121.0030 1500210000000
```

### 📌 Example Command

```bash
./Heatmap 0.010 0.010 .:xX@# 1 < example.in
```

### 📌 Example Output

```
.:#xx
#x:x:
xX:::
.::::
```

Each character represents a range of track point counts. For example:

- `.` = 0  
- `:` = 1  
- `x` = 2  
- `X` = 3  
- `@` = 4  
- `#` = 5+

## 🧠 Track ADT Overview

- Tracks are composed of one or more **segments**.  
- Segments contain **track points**: (latitude, longitude, timestamp)  
- Points are added in increasing time order  
- The ADT supports adding points, starting/merging segments, and generating heatmaps

### Key Functions (from `track.h`)

- `track_add_point()`  
- `track_start_segment()`  
- `track_merge_segments()`  
- `track_count_segments()`  
- `track_count_points()`  
- `track_heatmap()`  

## 🔍 Heatmap Construction Logic

- Rows increase southward; columns increase eastward  
- Heatmap bounds are calculated to contain all points within the smallest valid geographic wedge  
- Longitude wraparound (crossing ±180°) is handled correctly  
- Each track point contributes to exactly one cell  
- Output is row-by-row (top to bottom), each character per cell, with no extra whitespace

## ⚙️ Compilation

Use the provided `Makefile` to compile:

```bash
make
```

This builds the executable:

```bash
./Heatmap
```

## 📊 Performance

Operations on the track and heatmap meet strict time and space bounds:

| Function               | Time Complexity  |
|------------------------|------------------|
| `track_create`         | O(1)             |
| `track_destroy`        | O(n)             |
| `track_add_point`      | Amortized O(1)   |
| `track_merge_segments` | O(n)             |
| `track_heatmap`        | O(n + c)`        |

(`n` = number of track points, `c` = number of grid cells)

---

This project demonstrates real-world data handling, memory-efficient ADT design, and geographic visualization using ASCII-art heatmaps.
