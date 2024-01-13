#include <stdio.h>
#include <stdlib.h>

// Structure to represent a point in 2D space
typedef struct {
    double x;
    double y;
} Point;

#define SPECTRALBANDSMAX 200

// Function to calculate the orientation of three points (p, q, r)
double orientation(Point p, Point q, Point r) {
    return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
}

// Function to find the convex hull of a set of points
void convexHull(Point *points, int n, Point *hull, int *hullSize) {
    // Find the leftmost point
    int l = 0;

    // Find the point with maximum x-coordinate
    int maxXIndex = 0;
    for (int i = 1; i < n; ++i) {
        if (points[i].x > points[maxXIndex].x) {
            maxXIndex = i;
        }
    }

    // Storage for the convex hull
    *hullSize = 0;

    // Start from the leftmost point and move clockwise until the point with maximum x-coordinate
    int p = l;
    int q;
    while (p != maxXIndex) {
        // Add the current point to the result
        hull[(*hullSize)++] = points[p];

        // Search for a point 'q' such that orientation(p, q, x) is clockwise for all points 'x'
        q = (p + 1) % n;
        for (int i = 0; i < n; ++i) {
            if (orientation(points[p], points[i], points[q]) > 0) {
                q = i;
            }
        }

        // Now q is the most clockwise with respect to p
        // Set p as q for the next iteration
        p = q;
    }

    // Add the point with maximum x-coordinate to the result
    hull[(*hullSize)++] = points[maxXIndex];
}

// Comparison function for qsort
int comparePoints(const void *a, const void *b) {
    return ((Point *)a)->x > ((Point *)b)->x ? 1 : -1;
}

// Function to perform linear interpolation
double interpolate(double x, Point p1, Point p2) {
    double denominator = p2.x - p1.x;
    if (denominator == 0.0) {
        // Avoid division by zero, return a default value (e.g., p1.y)
        if (p1.y > 0) return p1.y;
        else if (p2.y > 0) return p2.y;
        else return 0;
    } else { 
        return p1.y + (x - p1.x) * (p2.y - p1.y) / denominator;
    }
}


// Function to interpolate and fill y-coordinates
void interpolateAndFill(Point *originalData, int originalSize, Point *convexHull, int hullSize) {
    // Sort convex hull points based on x-coordinates
    qsort(convexHull, hullSize, sizeof(Point), comparePoints);

    // Interpolate linearly to fill y-coordinates of original data
    int j = 0;
    for (int i = 0; i < originalSize; ++i) {
        double x = originalData[i].x;

        // Find the convex hull points within the x-range [x1, x2]
        while (j < hullSize - 1 && convexHull[j + 1].x <= x) {
            j++;
        }

        // Interpolate and update the y-coordinate
        double outval = interpolate(x, convexHull[j], convexHull[j + 1]);
        if (outval > 0){
            originalData[i].y = outval;
        }
    }
}

int main() {
    // Read input data from CSV file
    FILE *file = fopen("input.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    // Skip the header line
    char buffer[100];
    fgets(buffer, sizeof(buffer), file);
    // Read data into an array of points
    Point cr[SPECTRALBANDSMAX];
    Point orig[SPECTRALBANDSMAX];
    Point points[SPECTRALBANDSMAX];
    int numPoints = 0;
    while (fscanf(file, "%lf,%lf", &points[numPoints].x, &points[numPoints].y) == 2) {
        numPoints++;
    }
    // Close the CSV input file
    fclose(file);

    // Make a backup for later
    for (int i = 0; i < numPoints; ++i) {
        orig[i].x = points[i].x;
        orig[i].y = points[i].y;
    }

    // CONVEX HULL COMPUTATION

    // Create arrays to store the convex hull points
    Point hull[numPoints];
    int hullSize;

    // STRESS TEST 1M
    for (int i = 0; i < 1000000; i++){

    // Call the convexHull function
    convexHull(points, numPoints, hull, &hullSize);

    // INTERPOLATION OF CONVEX HULL OUTPUT TO INPUT X-COORD LENGTH

    // Call the interpolateAndFill function
    interpolateAndFill(points, numPoints, hull, hullSize);

    // Removal of continuum = original / convex hull
    for (int i = 0; i < numPoints; ++i) {
        cr[i].x = orig[i].x;
        cr[i].y = orig[i].y / points[i].y;
    }

    }
    // Write the continuum removed points to a file in gnuplot .dat format
    FILE *outFile = fopen("cr_output.dat", "w");
    if (outFile == NULL) {
        perror("Error creating output file");
        return 1;
    }
    for (int i = 0; i < numPoints; ++i) {
        fprintf(outFile, "%lf\t%lf\n", cr[i].x, cr[i].y);
    }
    // Close the output file
    fclose(outFile);

    return 0;
}

