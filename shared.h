#ifndef SHARED_H
#define SHARED_H

// Define the interface between main.cpp and overlay.cpp

// Function to set the overlay rectangle from main.cpp
void setOverlayRect(int width, int height);

// Function to draw the overlay from overlay.cpp
void drawOverlay();

#endif // SHARED_H
