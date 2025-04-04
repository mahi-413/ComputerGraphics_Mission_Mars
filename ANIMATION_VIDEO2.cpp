#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <math.h>
#include <queue>
#define PI 3.14159265

void Bline(int xs, int ys, int xe, int ye, int color);
void PlotCir(int xc, int yc, int x, int y, int color);
void Bcircle(int xc, int yc, int r, int color);
void floodfill4(int x, int y, int fc, int oc);

void drawRocket(int x, int y) ;
////////////////////////////////////////////////////////////////////////////////User Defined Functions
// Bresenham's Line Drawing Algorithm
void Bline(int xs, int ys, int xe, int ye, int color) 
{
    int x, y, dx, dy, p, signx, signy;
    
    dx = abs(xe - xs); // Calculate horizontal distance
    dy = abs(ye - ys); // Calculate vertical distance
    x = xs;
    y = ys;
    
    signx = (xe >= xs) ? 1 : -1; // Determine x-direction
    signy = (ye >= ys) ? 1 : -1; // Determine y-direction
    
    putpixel(x, y, color);  // Set initial pixel with given color
    
    if (dx >= dy) // Case: Slope <= 1
    {
        p = 2 * dy - dx;
        for (int i = 0; i < dx; i++) 
        {
            x += signx;
            if (p >= 0) 
            {
                y += signy;
                p += 2 * (dy - dx);
            }
            else 
            {
                p += 2 * dy;
            }
            putpixel(x, y, color);  // Use the same color consistently
        }
    } 
    else // Case: Slope > 1
    {
        p = 2 * dx - dy;
        for (int i = 0; i < dy; i++) 
        {
            y += signy;
            if (p >= 0) 
            {
                x += signx;
                p += 2 * (dx - dy);
            }
            else 
            {
                p += 2 * dx;
            }
            putpixel(x, y, color);  // Use the same color consistently
        }
    }
}

// Plot 8 Symmetric Points for Circle
void PlotCir(int xc, int yc, int x, int y, int color)
{
    putpixel(xc + x, yc - y, color);
    putpixel(xc - x, yc - y, color);
    putpixel(xc + x, yc + y, color);
    putpixel(xc - x, yc + y, color);
    putpixel(xc + y, yc - x, color);
    putpixel(xc - y, yc - x, color);
    putpixel(xc + y, yc + x, color);
    putpixel(xc - y, yc + x, color);
}

// Bresenham's Circle Drawing Algorithm
void Bcircle(int xc, int yc, int r, int color)
{
    int x = 0, y = r;
    int d = 3 - (2 * r); // Initial decision parameter

    PlotCir(xc, yc, x, y, color); // Draw initial points

    while (x < y)
    {
        x++;

        if (d < 0) // Choose right pixel
            d += 4 * x + 6;
        else // Choose diagonal pixel
        {
            y--;
            d += 4 * (x - y) + 10;
        }

        PlotCir(xc, yc, x, y, color); // Draw symmetric points
    }
}

//Function to draw rectangle
void DrawRectangle(int x1, int y1, int x2, int y2, int color) 
{
    // Draw the four sides using Bline()
    Bline(x1, y1, x2, y1, color); // Top side
    Bline(x2, y1, x2, y2, color); // Right side
    Bline(x2, y2, x1, y2, color); // Bottom side
    Bline(x1, y2, x1, y1, color); // Left side
}

//Flood Fill 4 coordinate
struct Pixel 
{ 
    int x, y; 
};

// flood fill using BFS (Queue-based)
void floodfill4(int x, int y, int fc, int oc) 
{
    if (getpixel(x, y) != oc || getpixel(x, y) == fc) 
        return; 

    std::queue<Pixel> pixelQueue;
    pixelQueue.push({x, y});  // Push the initial pixel

    while (!pixelQueue.empty()) 
    {
        Pixel pixel = pixelQueue.front();
        pixelQueue.pop();
        
        int px = pixel.x, py = pixel.y;

        if (getpixel(px, py) == oc) 
        {
            putpixel(px, py, fc);
            // Push neighboring pixels (4-directional)
            pixelQueue.push({px + 1, py}); // Right
            pixelQueue.push({px - 1, py}); // Left
            pixelQueue.push({px, py + 1}); // Down
            pixelQueue.push({px, py - 1}); // Up
        }
    }
}

////////////////////////////////////////////////////////////////////////0.1  CONTROL ROOM SCENE BEFORE ROCKET LAUNCH

// Function to draw a person at (x, y) in sitting or standing position
void drawPerson(int x, int y, bool sitting) 
{
    setcolor(WHITE);
    
    // Head
    Bcircle(x, y, 10, WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill4(x, y, WHITE, getpixel(x,y));

    // Body
    DrawRectangle(x - 7, y + 10, x + 7, y + 30, WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(x, y + 20, WHITE);

    // Legs (bent if sitting, straight if standing)
    if (sitting) 
    {
        Bline(x - 7, y + 30, x - 10, y + 35, WHITE); // Bent legs
        Bline(x + 7, y + 30, x + 10, y + 35, WHITE);
    } 
    else 
    {
        Bline(x - 7, y + 30, x - 10, y + 50, WHITE); // Standing legs
        Bline(x + 7, y + 30, x + 10, y + 50, WHITE);
    }

    // Arms
    Bline(x - 7, y + 15, x - 15, y + 20, WHITE);
    Bline(x + 7, y + 15, x + 15, y + 20, WHITE);
}

// Function to draw a telemetry screen at (x, y)
void drawTelemetryScreen(int x, int y) 
{
    setcolor(CYAN);
    
    // Draw screen boundary
    DrawRectangle(x, y, x + 160, y + 80, CYAN); 
    
	// Fill screen with a pattern
	setfillstyle(HATCH_FILL, CYAN); 
    floodfill(x + 1, y + 1, CYAN);

	// Display telemetry data
    setcolor(WHITE);
    outtextxy(x + 10, y + 10, "Telemetry");
    outtextxy(x + 10, y + 30, "Speed: 7000m/s");
    outtextxy(x + 10, y + 50, "Altitude: 350km");
}

// Function to draw the main mission screen
void drawMainScreen() 
{
    setcolor(YELLOW);
    
    // Draw and fill the large display screen
    DrawRectangle(250, 50, 550, 240, YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    floodfill(251, 51, YELLOW);

	// Display live feed text
    setcolor(WHITE);
    outtextxy(300, 70, "Live Feed: Rocket Launch");

	// Draw the rocket
    drawRocket(400, 130);
}


// Function to draw the control room setup
void drawControlRoom() 
{
  
    // Display heading
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3); // Larger font
    outtextxy(250, 10, "MISSION CONTROL CENTER");

    // Main live feed screen
    drawMainScreen();

    // Telemetry screens
    drawTelemetryScreen(30, 50);
    drawTelemetryScreen(570, 50);
    drawTelemetryScreen(30, 150);
    drawTelemetryScreen(570, 150);

    // Engineers and personnel in two rows
    drawPerson(130, 270, false); // Standing
    drawPerson(180, 280, true); // Sitting

    drawPerson(230, 270, false);
    drawPerson(280, 280, true);

    drawPerson(330, 270, false);
    drawPerson(380, 280, true);

	drawPerson(130, 350, false);
    drawPerson(180, 360, true);

    drawPerson(230, 350, false);
    drawPerson(280, 360, true);

    drawPerson(330, 350, false);
    drawPerson(380, 360, true);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////1 COUNTDOWN OF ROCKET LAUNCH
void drawRocket(int x, int y) 
{
    setcolor(BLACK); // Set Boundary Color to BLACK

    // Rocket Body (Added Extra Outer Layer)
    DrawRectangle(x - 20, y, x + 20, y + 100, BLACK);
    DrawRectangle(x - 21, y - 1, x + 21, y + 101, BLACK);  // Extra boundary

    setfillstyle(SOLID_FILL, LIGHTGRAY);  
    floodfill(x, y + 50, BLACK); // Fill inside boundary

    // Rocket Nose
    Bline(x - 20, y, x, y - 30, BLACK);
    Bline(x, y - 30, x + 20, y, BLACK);
    Bline(x - 21, y, x, y - 31, BLACK); // Extra boundary
    Bline(x, y - 31, x + 21, y, BLACK); // Extra boundary

    setfillstyle(SOLID_FILL, RED);
    floodfill(x, y - 20, BLACK); // Use boundary color (BLACK)

    // Rocket Fins (Extra Boundary Lines)
    Bline(x - 20, y + 80, x - 40, y + 100, BLACK);
    Bline(x + 20, y + 80, x + 40, y + 100, BLACK);
    Bline(x - 21, y + 80, x - 41, y + 101, BLACK); // Extra boundary
    Bline(x + 21, y + 80, x + 41, y + 101, BLACK); // Extra boundary

    setfillstyle(SOLID_FILL, RED);
    floodfill(x - 30, y + 90, BLACK);
    floodfill(x + 30, y + 90, BLACK);
}

void drawLaunchPad(int x, int y) 
{
    // Set Boundary Color to BLACK
    setcolor(BLACK);

    // Support Structure (Extra Boundaries)
    Bline(x - 50, y + 100, x - 50, y - 50, BLACK);
    Bline(x + 50, y + 100, x + 50, y - 50, BLACK);
    Bline(x - 50, y - 50, x + 50, y - 50, BLACK);
    Bline(x - 51, y + 100, x - 51, y - 51, BLACK); // Extra boundary
    Bline(x + 51, y + 100, x + 51, y - 51, BLACK);

    // Base (Double Boundary)
    DrawRectangle(x - 60, y + 100, x + 60, y + 120, BLACK);
    DrawRectangle(x - 61, y + 99, x + 61, y + 121, BLACK);  // Extra boundary

    setfillstyle(SOLID_FILL, DARKGRAY);
    floodfill(x, y + 110, BLACK); // Fill inside boundary
}
void drawBackground() 
{
    // Set Background Color (Sky Blue)
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    floodfill(1, 1, WHITE); 
}

void drawCountdown(int count) 
{
    setcolor(YELLOW); // Countdown text color
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    char countdown[2];
    sprintf(countdown, "%d", count);
    outtextxy(300, 70, countdown); // Display countdown
}

void drawSmoke(int x, int y, int frame) 
{
    for (int i = 0; i < 10; i++) 
	{
        int dx = rand() % 40 - 20; // Random horizontal spread
        int dy = rand() % 20; // Random vertical spread
        setcolor(LIGHTGRAY);
        Bcircle(x + dx, y + 100 + dy + frame * 2, 5, LIGHTGRAY); // Draw smoke puff
    }
}

void drawFlames(int x, int y, int frame) 
{
    setcolor(RED);
    // Draw red flames with random variations
    for (int i = 0; i < 5; i++) 
	{
        int dx = rand() % 20 - 10;
        int dy = rand() % 10;
        Bline(x + dx, y + 100 + frame * 2, x + dx, y + 120 + dy + frame * 2, RED);
    }
    setcolor(YELLOW);
    // Draw yellow flames for extra effect
    for (int i = 0; i < 3; i++) 
	{
        int dx = rand() % 10 - 5;
        int dy = rand() % 5;
        Bline(x + dx, y + 100 + frame * 2, x + dx, y + 115 + dy + frame * 2, YELLOW);
    }
}

void Birdstranslate(int *x, int *y, int tx, int ty)
{
    *x += tx;
    *y += ty;
}

void drawBirds(int frame) 
{
    for (int i = 0; i < 6; i++)  // Draw multiple birds
    {
        int x = 50 + i * 100; // Initial position of birds
        int y = 100 - (i * 10);

        // Apply translation transformation (Moving birds)
        int tx = frame % 500; // Controls horizontal movement
        int ty = (i % 2 == 0) ? sin(frame * 0.1) * 5 : -sin(frame * 0.1) * 5; // Simulate flapping effect
        
        Birdstranslate(&x, &y, tx, ty);

        // Draw Bird Wings
        Bline(x, y, x + 10, y - 5, BLACK);
        Bline(x + 10, y - 5, x + 20, y, BLACK);

        // Extra wings for animation effect
        Bline(x + 5, y, x + 15, y - 7, BLACK);
        Bline(x + 15, y - 7, x + 25, y, BLACK);
    }
}


void drawClouds(int frame)
{
    int cloudX = (frame % 600); // Continuous cloud movement
    int screenWidth = getmaxx(); // Get screen width
    int screenHeight = getmaxy(); // Get screen height
    int cloudSpacing = 150; // Horizontal spacing between clouds

    for (int i = 0; i < screenWidth / cloudSpacing + 2; i++) 
	{ // Cover the screen width
        for (int j = 0; j < 1; j++) 
		{ // Multiple cloud rows
            int x = cloudX + i * cloudSpacing;
            int y = 50 + j * 80 + (i % 2) * 20; // Staggered height for a natural look

            setcolor(LIGHTGRAY);
            setfillstyle(SOLID_FILL, LIGHTGRAY);

            // Cloud Body (Using Filled Ellipses for Proper Filling)
            fillellipse(x, y, 20, 15);
            fillellipse(x + 20, y + 5, 18, 12);
            fillellipse(x - 20, y + 5, 18, 12);
            fillellipse(x + 10, y - 10, 15, 10);
            fillellipse(x - 10, y - 10, 15, 10);
        }
    }
}


void drawSun()
{
    // Sun Circle
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    Bcircle(550, 50, 30, YELLOW);
    floodfill(550, 50, YELLOW);

    // Sun Rays
    for (int i = 0; i < 360; i += 45)
    {
        int x1 = 550 + 35 * cos(i * 3.14 / 180);
        int y1 = 50 + 35 * sin(i * 3.14 / 180);
        Bline(550, 50, x1, y1, YELLOW);
    }
}


void drawSky(int frame)
{
    drawSun();
    drawClouds(frame);
    drawBirds(frame);   
}

/////////////////////////////////////////////////////////////////////////////////////////////////////2  ROCKET STARTED TO GO UPWARDS
void cameraShake(int *x, int *y, int intensity)
{
    *x += (rand() % (intensity * 2 + 1)) - intensity; // Random horizontal jitter
    *y += (rand() % (intensity + 1)) - intensity / 2; // Random vertical jitter
}

// Function to draw flames at the bottom of the rocket
void drawFlamesUp(int x, int y, int intensity)
{
    setcolor(RED);
    for (int i = 0; i < intensity * 2; i++)
    {
        int fx = x + (rand() % 30 - 15);  // Randomize flame position
        int fy = y + 100 + (rand() % 20); // Extend flames downward
        Bline(x, y + 100, fx, fy, RED);
    }
}

// Function to draw thick smoke at the bottom of the rocket
void drawSmokeUp(int x, int y, int intensity)
{
    setcolor(LIGHTGRAY);
    for (int i = 0; i < intensity * 3; i++)
    {
        int sx = x + (rand() % 40 - 20);  // Spread out the smoke
        int sy = y + 120 + (rand() % 30); // Make smoke rise
        Bcircle(sx, sy, rand() % 10 + 5, LIGHTGRAY); // Random smoke puff size  
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        floodfill(sx, sy, LIGHTGRAY);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////3  ROCKET IN HIGHER ALTITUTE IN CLOUDS

// Function to make the rocket partially disappear in clouds
void drawRocketWithCloudEffect(int x, int y, int altitude)
{
    int rocketColor = DARKGRAY; // Default rocket color 

    setcolor(BLACK); // Rocket outline color  

    // Rocket Body (Added Extra Outer Layer)
    DrawRectangle(x - 20, y, x + 20, y + 100, BLACK);
    DrawRectangle(x - 21, y - 1, x + 21, y + 101, BLACK); // Extra boundary

    setfillstyle(SOLID_FILL, rocketColor);
    floodfill(x, y + 50, BLACK); // Fill inside boundary

    // Rocket Nose
    Bline(x - 20, y, x, y - 30, BLACK);
    Bline(x, y - 30, x + 20, y, BLACK);
    Bline(x - 21, y, x, y - 31, BLACK); // Extra boundary
    Bline(x, y - 31, x + 21, y, BLACK);

    setfillstyle(SOLID_FILL, RED);
    floodfill(x, y - 20, BLACK); // Use boundary color (BLACK)

    // Rocket Fins (Extra Boundary Lines)
    Bline(x - 20, y + 80, x - 40, y + 100, BLACK);
    Bline(x + 20, y + 80, x + 40, y + 100, BLACK);
    Bline(x - 21, y + 80, x - 41, y + 101, BLACK); // Extra boundary
    Bline(x + 21, y + 80, x + 41, y + 101, BLACK);

    setfillstyle(SOLID_FILL, RED);
    floodfill(x - 30, y + 90, BLACK);
    floodfill(x + 30, y + 90, BLACK);

	drawSmokeUp(x, y, 5); 
}

void drawClouds3(int frame) 
{
    int cloudX = (frame % 600); // Continuous cloud movement
    int screenWidth = getmaxx(); // Get screen width
    int screenHeight = getmaxy(); // Get screen height
    int cloudSpacing = 160; // Horizontal spacing between clouds
    int rowSpacing = 90; // Adjusted vertical spacing for better density

    for (int i = 0; i < screenWidth / cloudSpacing + 5; i++) 
    { // More columns for full coverage
        for (int j = 0; j < screenHeight / rowSpacing + 5; j++) 
        { // More rows for full height coverage
            int x = cloudX + i * cloudSpacing;
            int y = 50 + j * rowSpacing + (i % 2) * 20; // Staggered height for a natural look

            setcolor(WHITE);
            setfillstyle(SOLID_FILL, WHITE);

            // Cloud Body (Using Filled Ellipses for Proper Filling)
            fillellipse(x, y, 25, 18);
            fillellipse(x + 22, y + 6, 20, 14);
            fillellipse(x - 22, y + 6, 20, 14);
            fillellipse(x + 12, y - 12, 18, 12);
            fillellipse(x - 12, y - 12, 18, 12);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////4 ROCKET ENTERING IN SPACE 
void drawEarth(int shrinkFactor) 
{
    int earthX = getmaxx() / 2;
    int earthY = getmaxy() - 50; // Stay fixed at bottom

    int width = 200 - shrinkFactor / 5;
    int height = 100 - shrinkFactor / 10;

    // Draw Blue Ocean (Base of Earth)
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(earthX, earthY, width, height); 

    // Draw Green Landmasses (Random Shapes)
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(earthX - width / 3, earthY, width / 6, height / 4);
    fillellipse(earthX + width / 4, earthY - height / 6, width / 7, height / 5);
    fillellipse(earthX - width / 5, earthY + height / 10, width / 8, height / 6);

    // Draw White Clouds (Scattered Ellipses)
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(earthX - width / 3, earthY - height / 5, width / 10, height / 15);
    fillellipse(earthX + width / 4, earthY + height / 6, width / 12, height / 15);
    fillellipse(earthX, earthY - height / 4, width / 14, height / 20);
}


// Function to draw stars across full screen
void drawStars(int count) 
{
    setcolor(WHITE);
    for (int i = 0; i < count; i++) 
	{
        int x = rand() % getmaxx(); // Random X
        int y = rand() % getmaxy(); // Random Y
        putpixel(x, y, WHITE);
    }
}

// Function to draw Moon and planets
void drawPlanets(int shrinkFactor) 
{
    // Moon
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(getmaxx() - 100, 100 - shrinkFactor / 5, 40, 40);

    // Mars
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(getmaxx() - 250, 200 - shrinkFactor / 4, 30, 30);

    // Venus
    setcolor(LIGHTRED);
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(getmaxx() - 400, 150 - shrinkFactor / 6, 50, 50);
}

// Draws the rocket
void drawRocket4(int x, int y) 
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, DARKGRAY);

    // Rocket Body
    DrawRectangle(x - 20, y, x + 20, y + 100, WHITE);
    floodfill(x, y + 50, WHITE);

    // Nose Cone
    Bline(x - 20, y, x, y - 30, WHITE);
    Bline(x, y - 30, x + 20, y, WHITE);
    setfillstyle(SOLID_FILL, RED);
    floodfill(x, y - 20, WHITE);

    // Fins
    Bline(x - 20, y + 80, x - 40, y + 100, WHITE);
    Bline(x + 20, y + 80, x + 40, y + 100, WHITE);
    setfillstyle(SOLID_FILL, RED);
    floodfill(x - 30, y + 90, WHITE);
    floodfill(x + 30, y + 90, WHITE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////4.1 ROCKET MOVING IN ORBIT ALONG WITH OTHER PLANETS

// Function to draw the Sun  
void drawSun(int x, int y) 
{
    setcolor(YELLOW);
    Bcircle(x, y, 30, YELLOW); // Outer circle for Sun  
    setfillstyle(SOLID_FILL, YELLOW);
    floodfill(x, y, YELLOW); // Fill Sun with yellow color
}

// Function to draw a planet with given position, size, and color  
void drawPlanet(int x, int y, int radius, int color) 
{
    setcolor(color);
    Bcircle(x, y, radius, color); // Outer boundary of the planet  
    setfillstyle(SOLID_FILL, color);
    floodfill(x, y, color); // Fill planet with given color  
}

// Function to draw a small rocket  
void drawRocket41(int x, int y) 
{
    setcolor(WHITE);
    
    // Rocket Body
    DrawRectangle(x - 10, y, x + 10, y + 40, WHITE);  // Main rocket structure  
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    floodfill(x, y + 20, WHITE); // Fill rocket body  
    
    // Rocket Nose
    Bline(x - 10, y, x, y - 15, WHITE); // Left side of nose  
    Bline(x, y - 15, x + 10, y, WHITE); // Right side of nose  
    setfillstyle(SOLID_FILL, RED);
    floodfill(x, y - 10, WHITE); // Fill nose cone 
    
    // Rocket Fins
    Bline(x - 10, y + 30, x - 20, y + 40, WHITE); // Left fin 
    Bline(x + 10, y + 30, x + 20, y + 40, WHITE); // Right fin 
    setfillstyle(SOLID_FILL, RED);
    floodfill(x - 15, y + 35, WHITE); // Fill left fin  
    floodfill(x + 15, y + 35, WHITE); // Fill right fin 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////4.2 ROCKET MOVING AROUND EARTH

// Function to draw the background (black space) 
void drawBackground42() 
{
    setcolor(BLACK);

    // Draw the boundary of the screen using four lines (rectangle)
    Bline(0, 0, getmaxx(), 0, BLACK);         // Top boundary
    Bline(getmaxx(), 0, getmaxx(), getmaxy(), BLACK);  // Right boundary
    Bline(getmaxx(), getmaxy(), 0, getmaxy(), BLACK);  // Bottom boundary
    Bline(0, getmaxy(), 0, 0, BLACK);         // Left boundary

    // Fill the entire screen with black using floodfill
    floodfill(getmaxx() / 2, getmaxy() / 2, BLACK); 
}


// Function to draw Earth with landmasses and ice caps  
void drawEarth42(int x, int y) 
{
    // Draw Earth's boundary
    setcolor(WHITE);
    Bcircle(x, y, 100, WHITE);  

    // Fill Earth with Blue (Oceans)
    setfillstyle(SOLID_FILL, BLUE);
    floodfill(x, y, WHITE);

    // Add Green Landmasses (Continents)
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);

    // Main landmasses
    fillellipse(x - 30, y, 25, 15); 
    fillellipse(x + 40, y - 10, 20, 10); 
    fillellipse(x - 50, y - 20, 15, 10); 
    fillellipse(x + 30, y + 30, 20, 12); 
    fillellipse(x, y - 40, 18, 14);  
    fillellipse(x - 20, y + 45, 22, 12); 

    // Add White Polar Ice Caps
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y - 90, 40, 10); // Top ice cap
    fillellipse(x, y + 90, 40, 10); // Bottom ice cap
}


// Function to draw a rotating rocket with scaling  
void drawRocket42(int x, int y, float scale, float angle) 
{
    setcolor(WHITE);

    int bodyWidth = 20 * scale;
    int bodyHeight = 50 * scale;

    // Rotate rocket around its center using trigonometry
    int rotatedX = x + cos(angle) * bodyHeight;
    int rotatedY = y + sin(angle) * bodyHeight;

	// Draw rocket body  
    DrawRectangle(rotatedX - bodyWidth / 2, rotatedY, rotatedX + bodyWidth / 2, rotatedY - bodyHeight, WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    floodfill(rotatedX, rotatedY - bodyHeight / 2, WHITE);

    // Rocket Nose
    int noseX = rotatedX;
    int noseY = rotatedY - bodyHeight - 10 * scale;

    Bline(rotatedX - bodyWidth / 2, rotatedY - bodyHeight, noseX, noseY, WHITE);
    Bline(noseX, noseY, rotatedX + bodyWidth / 2, rotatedY - bodyHeight, WHITE);

    setfillstyle(SOLID_FILL, BLACK);
    floodfill(noseX, noseY + 5, WHITE); 

    // Rocket Fins
    int finOffset = 10 * scale;
    int finX1 = rotatedX - bodyWidth / 2;
    int finY1 = rotatedY - 10 * scale;
    int finX2 = finX1 - finOffset;
    int finY2 = rotatedY + 10 * scale;

    int finX3 = rotatedX + bodyWidth / 2;
    int finY3 = rotatedY - 10 * scale;
    int finX4 = finX3 + finOffset;
    int finY4 = rotatedY + 10 * scale;

    Bline(finX1, finY1, finX2, finY2, WHITE);
    Bline(finX2, finY2, rotatedX - bodyWidth / 2, rotatedY, WHITE);

    Bline(finX3, finY3, finX4, finY4, WHITE);
    Bline(finX4, finY4, rotatedX + bodyWidth / 2, rotatedY, WHITE);

    setfillstyle(SOLID_FILL, BLACK);
    floodfill(finX1 - 2, finY1 + 2, WHITE);
    floodfill(finX3 + 2, finY3 + 2, WHITE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////5 ROCKET MOVING AMONG ASTEROIDS

// Function to draw the Sun with animated rays
void drawSun5(int centerX, int centerY, int frame) 
{
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(centerX, centerY, 50, 50); // Sun body

    // Animated sun rays
    for (int i = 0; i < 360; i += 30) 
    {
        int length = 70 + (frame % 5); // Rays slightly change size for animation
        int x1 = centerX + cos(i * M_PI / 180) * 50;
        int y1 = centerY + sin(i * M_PI / 180) * 50;
        int x2 = centerX + cos(i * M_PI / 180) * length;
        int y2 = centerY + sin(i * M_PI / 180) * length;
        Bline(x1, y1, x2, y2, YELLOW); // Draw rays
    }
}

// Function to draw planets
void drawPlanet5(int x, int y, int radius, int color) 
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, radius, radius);
}

// Function to draw asteroids moving randomly
void drawAsteroids5(int numAsteroids, int asteroidPositions[][2]) 
{
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);

    for (int i = 0; i < numAsteroids; i++) 
    {
        int x = asteroidPositions[i][0];
        int y = asteroidPositions[i][1];
        fillellipse(x, y, 5, 5); // Small asteroid
    }
}

// Function to move asteroids randomly
void moveAsteroids5(int numAsteroids, int asteroidPositions[][2], int asteroidDirections[][2]) 
{
    for (int i = 0; i < numAsteroids; i++) 
    {
        // If falling asteroid, move downward diagonally
        if (asteroidDirections[i][1] == 1) 
        {  
            asteroidPositions[i][0] += (rand() % 3 + 1); // Rightward movement
            asteroidPositions[i][1] += (rand() % 3 + 1); // Downward movement
        } 
        else 
        { 
            // Normal random movement for space-like floating asteroids
            asteroidPositions[i][0] += (rand() % 5 - 2);
            asteroidPositions[i][1] += (rand() % 5 - 2);
        }

        // Reset position if asteroid moves out of bounds
        if (asteroidPositions[i][0] < 0) 
			asteroidPositions[i][0] = getmaxx();
        if (asteroidPositions[i][0] > getmaxx()) 
			asteroidPositions[i][0] = 0;
        if (asteroidPositions[i][1] > getmaxy()) 
        {
            asteroidPositions[i][1] = 0; // Reset falling asteroid to top
            asteroidPositions[i][0] = rand() % getmaxx(); // Assign new random x-position
        }
    }
}

// Function to draw the rocket
void drawRocket5(int x, int y) 
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);

    // Rocket Body
    DrawRectangle(x - 10, y, x + 10, y + 40, WHITE);
    floodfill(x, y + 20, WHITE);

    // Rocket Nose
    Bline(x - 10, y, x, y - 15, WHITE);
    Bline(x, y - 15, x + 10, y, WHITE);
    floodfill(x, y - 10, WHITE);

    // Rocket Fins
    Bline(x - 10, y + 40, x - 20, y + 50, WHITE);
    Bline(x + 10, y + 40, x + 20, y + 50, WHITE);
    floodfill(x - 15, y + 45, WHITE);
    floodfill(x + 15, y + 45, WHITE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////5.1 ROCKET MOVING AROUND THE MARS

// Function to draw the background with stars
void drawBackground51() 
{
    setbkcolor(BLACK);
    cleardevice();

    // Generate random stars
    for (int i = 0; i < 200; i++) 
	{
        int starX = rand() % getmaxx();
        int starY = rand() % getmaxy();
        putpixel(starX, starY, WHITE); // Draw stars at random positions
    }
}

// Function to draw Mars with surface details
void drawMars51(int x, int y, int radius) 
{
    // Mars Surface - Single Outer Circle
    setcolor(RED);
    Bcircle(x, y, radius, RED); 

    // Fill Mars with red color
    setfillstyle(SOLID_FILL, RED);
    floodfill(x, y, RED);
}

// Function to draw a rocket orbiting Mars
void drawRocket51(int orbitX, int orbitY, float angle) 
{
    int rocketSize = 20;

    // Clear previous frame (important for smooth motion)
    cleardevice();
    drawBackground51();  // Redraw stars
    drawMars51(orbitX, orbitY, 100);  // Redraw Mars

    // Calculate rocket position based on orbital motion
    int rocketX = orbitX + 180 * cos(angle * PI / 180);
    int rocketY = orbitY + 180 * sin(angle * PI / 180);

    // Rocket Body
    setcolor(WHITE);
    DrawRectangle(rocketX - 10, rocketY, rocketX + 10, rocketY - 40, WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    floodfill(rocketX, rocketY - 20, WHITE);

    // Rocket Nose
    Bline(rocketX - 10, rocketY - 40, rocketX, rocketY - 60, WHITE);
    Bline(rocketX, rocketY - 60, rocketX + 10, rocketY - 40, WHITE);
    setfillstyle(SOLID_FILL, RED);
    floodfill(rocketX, rocketY - 50, WHITE);

    // Rocket Fins
    Bline(rocketX - 10, rocketY, rocketX - 20, rocketY + 10, WHITE);
    Bline(rocketX + 10, rocketY, rocketX + 20, rocketY + 10, WHITE);
    setfillstyle(SOLID_FILL, RED);
    floodfill(rocketX - 15, rocketY + 5, WHITE);
    floodfill(rocketX + 15, rocketY + 5, WHITE);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////6 ROCKET LANDING ON THE MARS SURFACE

// Function to draw the Mars surface with terrain details
void drawMarsSurface6()
{
    int maxX = getmaxx();
    int maxY = getmaxy();
    
    // Set Mars Surface Color
    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    DrawRectangle(0, maxY - 100, maxX, maxY, BROWN);
    floodfill(10, maxY - 50, BROWN); // Fill surface

    // Draw Mountains
    setcolor(DARKGRAY);

	// First Mountain (Mountain 1)
	int mountain[] = {50, maxY - 100, 150, maxY - 200, 250, maxY - 100, 50, maxY - 100};
	
	// Drawing the outline of the mountain using Bline (Bresenham's line drawing)
	for (int i = 0; i < 6; i += 2) 
	{
	    Bline(mountain[i], mountain[i + 1], mountain[i + 2], mountain[i + 3], DARKGRAY);
	}
	
	// Filling the mountain using floodfill (inside the polygon)
	setfillstyle(SOLID_FILL, DARKGRAY);
	floodfill((mountain[0] + mountain[2] + mountain[4]) / 3, (mountain[1] + mountain[3] + mountain[5]) / 3, DARKGRAY);
	
	// Second Mountain (Mountain 2)
	int mountain2[] = {200, maxY - 100, 300, maxY - 180, 400, maxY - 100, 200, maxY - 100};
	
	// Drawing the outline of the second mountain using Bline
	for (int i = 0; i < 6; i += 2) 
	{
	    Bline(mountain2[i], mountain2[i + 1], mountain2[i + 2], mountain2[i + 3], DARKGRAY);
	}
	
	// Filling the second mountain using floodfill
	setfillstyle(SOLID_FILL, DARKGRAY);
	floodfill((mountain2[0] + mountain2[2] + mountain2[4]) / 3, (mountain2[1] + mountain2[3] + mountain2[5]) / 3, DARKGRAY);


    // Draw Craters
    setcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(100, maxY - 50, 20, 10);
    fillellipse(300, maxY - 60, 25, 12);
    fillellipse(500, maxY - 40, 15, 8);
}

// Function to draw the rocket with landing gear
void drawRocket6(int x, int y, bool landingGear) 
{
    setcolor(WHITE);

    // Rocket Body
    DrawRectangle(x - 20, y, x + 20, y + 80, WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(x, y + 40, WHITE); // Fill inside rocket

    // Rocket Nose
	setcolor(RED);
	// Draw the nose using Bline (Bresenham's line drawing)
	Bline(x - 20, y, x, y - 30, RED);
	Bline(x, y - 30, x + 20, y, RED);
	Bline(x + 20, y, x - 20, y, RED);
	
	// Fill the nose with red color using floodfill
	setfillstyle(SOLID_FILL, RED);
	floodfill(x, y - 15, RED); // Point inside the nose for filling
	
	// Rocket Fins
	setcolor(RED);
	
	// Left Fin
	Bline(x - 20, y + 60, x - 40, y + 80, RED);
	Bline(x - 40, y + 80, x - 20, y + 80, RED);
	Bline(x - 20, y + 80, x - 20, y + 60, RED);
	
	// Fill the left fin with red color using floodfill
	setfillstyle(SOLID_FILL, RED);
	floodfill(x - 30, y + 70, RED); // Point inside the left fin for filling
	
	// Right Fin
	Bline(x + 20, y + 60, x + 40, y + 80, RED);
	Bline(x + 40, y + 80, x + 20, y + 80, RED);
	Bline(x + 20, y + 80, x + 20, y + 60, RED);
	
	// Fill the right fin with red color using floodfill
	setfillstyle(SOLID_FILL, RED);
	floodfill(x + 30, y + 70, RED); // Point inside the right fin for filling


    // Landing Gear (Only when near Mars)
    if (landingGear) 
    {
        setcolor(YELLOW);

        // Left Leg
        Bline(x - 15, y + 80, x - 30, y + 100, YELLOW);
        Bline(x - 30, y + 100, x - 25, y + 100, YELLOW);

        // Right Leg
        Bline(x + 15, y + 80, x + 30, y + 100, YELLOW);
        Bline(x + 30, y + 100, x + 25, y + 100, YELLOW);
    }
}

// Function to animate the rocket descending towards Mars
void landRocket6() 
{
    int rocketX = getmaxx() / 2;
    int rocketY = 50; // Start from the top
    int groundY = getmaxy() - 130; // Mars surface level

	// Rocket descending animation
    for (int i = 0; rocketY < groundY; i++) 
	{
        cleardevice();
        setbkcolor(BLACK); // Space background

        drawMarsSurface6(); // Mars' terrain
        drawRocket6(rocketX, rocketY, (rocketY > groundY - 40)); // Deploy landing gear near surface

        rocketY += 1; // Slow descent
        delay(20);
    }

    // Final touch down
    for (int i = 0; i < 5; i++) 
	{
        setcolor(YELLOW);
        Bcircle(rocketX, groundY + 20, i * 3, YELLOW); // Small shockwave effect
        delay(100);
    }

	// Display landing message
    outtextxy(rocketX - 40, groundY + 30, "Landing Successful!");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////7 ASTRONAUT COMING OUT OF THE ROCKET ON MARS

// Function to draw an astronaut at position (x, y)
void drawAstronaut7(int x, int y)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    
    // Helmet
    Bcircle(x, y, 10, WHITE);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    floodfill(x, y, WHITE);

    // Body
    DrawRectangle(x - 5, y + 10, x + 5, y + 30, WHITE);
    floodfill(x, y + 20, WHITE);

    // Legs
    Bline(x - 5, y + 30, x - 10, y + 40, WHITE);
    Bline(x + 5, y + 30, x + 10, y + 40, WHITE);

    // Arms
    Bline(x - 5, y + 15, x - 15, y + 20, WHITE);
    Bline(x + 5, y + 15, x + 15, y + 20, WHITE);
}

// Function to draw a flag at position (x, y)
void drawFlag7(int x, int y)
{
    setcolor(WHITE);
    
    // Flag Pole (Aligning with astronaut's right hand)
    Bline(x, y - 10, x, y - 60, WHITE);

    // Flag Rectangle (Attached at astronaut's hand level)
    setfillstyle(SOLID_FILL, GREEN);
    DrawRectangle(x, y - 60, x + 40, y - 40, WHITE);
    floodfill(x + 10, y - 50, WHITE);

    // Display "MISSION ACCOMPLISHED" above the flag
    setcolor(YELLOW);
    outtextxy(x - 110 , y - 160, "MISSION");
    outtextxy(x - 110, y - 130, "ACCOMPLISHED");
}

////////////////////////////////////////////////////////////////////////////////////////////////////8 CONTROL SCREEN AFTER THE ROCKET LANDING

// Function to update the main screen after success
void drawMissionAccomplishedScreen() 
{
    setcolor(YELLOW);
    DrawRectangle(250, 50, 550, 240, YELLOW); // Message Screen
    setfillstyle(SOLID_FILL, YELLOW);
    floodfill(251, 51, YELLOW);

    setcolor(WHITE);
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2); 
    outtextxy(280, 100, "MISSION ACCOMPLISHED!");
}

// Function to draw a dancing person at (x, y) with movement
void drawDancingPerson(int x, int y, int frame) 
{
    setcolor(WHITE);
    
    // Head
    Bcircle(x, y, 10, WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(x, y, WHITE);

    // Body
    DrawRectangle(x - 7, y + 10, x + 7, y + 30, WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(x, y + 20, WHITE);

    // Random jumping motion
    int jumpOffset = (frame % 10 < 5) ? 5 : -5; // Moves up and down

    // Random arm movement
    int armOffset = (frame % 4 < 2) ? 5 : -5; 

    // Moving Arms
    Bline(x - 7, y + 15, x - 15, y + 10 + armOffset, WHITE);
    Bline(x + 7, y + 15, x + 15, y + 10 - armOffset, WHITE);

    // Dancing Legs
    Bline(x - 7, y + 30, x - 10, y + 35 + jumpOffset, WHITE);
    Bline(x + 7, y + 30, x + 10, y + 35 - jumpOffset, WHITE);
}

// Function to draw a telemetry screen at (x, y)
void drawTelemetryScreen8(int x, int y) 
{
    setcolor(WHITE);
    DrawRectangle(x, y, x + 200, y + 100, WHITE); // Screen box
    setfillstyle(HATCH_FILL, LIGHTBLUE);
    floodfill(x + 1, y + 1, WHITE);

    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2); 
    outtextxy(x + 10, y + 20, "Landed On Mars");
}

// Function to show the celebration scene with movement
void drawCelebrationScene() 
{
    setbkcolor(BLACK); // Set background color to black
    cleardevice();
    
    for (int frame = 0; frame < 50; frame++) // Loop to animate
    {
        cleardevice();
        
        // Display heading
	    setcolor(WHITE);
	    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3); // Larger font
	    outtextxy(250, 10, "MISSION CONTROL CENTER");
        
        // Display the mission accomplished message
        drawMissionAccomplishedScreen();
        
        // Enlarged Telemetry screens with flashing colors
        setcolor(rand() % 15); drawTelemetryScreen8(30, 50);
        setcolor(rand() % 15); drawTelemetryScreen8(600, 50);
        setcolor(rand() % 15); drawTelemetryScreen8(30, 180);
        setcolor(rand() % 15); drawTelemetryScreen8(600, 180);

        // Engineers dancing and celebrating
        drawDancingPerson(130, 300, frame);
        drawDancingPerson(180, 310, frame);
        drawDancingPerson(230, 300, frame);
        drawDancingPerson(280, 310, frame);
        drawDancingPerson(330, 300, frame);
        drawDancingPerson(380, 310, frame);

        drawDancingPerson(130, 370, frame);
        drawDancingPerson(180, 380, frame);
        drawDancingPerson(230, 370, frame);
        drawDancingPerson(280, 380, frame);
        drawDancingPerson(330, 370, frame);
        drawDancingPerson(380, 380, frame);

        delay(100); // Small delay for smooth animation
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////// MAIN()
int main() 
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    int x = 300, y = 300;
    
    ///////////////////////////0.1  CONTROL ROOM SCENE BEFORE ROCKET LAUNCH
    // Display the control room scene with engineers monitoring the launch
	for(int i=0; i<20; i++)
	{

		drawControlRoom(); 
		delay(100);
	}
       
    ///////////////////////////1  COUNTDOWN OF ROCKET LAUNCH
    // Display countdown and prepare rocket on the launch pad
	for (int i = 5; i >= 0; i--) 
	{
        cleardevice(); // Clear the screen for each countdown step
        drawBackground(); // Draw sky and surroundings	    
	    drawLaunchPad(x,y); // Draw the launch pad	    
	    drawRocket(x, y); // Draw the rocket
        drawCountdown(i); // Display countdown number
        drawSky(i); // Adjust sky based on countdown stage
        drawSmoke(x, y, 10 - i); // Generate smoke gradually
        drawFlames(x, y, 10 - i); // Show flames at the base of the rocket
        delay(1000); // Wait for 1 second per countdown step
    }
    
    ///////////////////////////2  ROCKET STARTED TO GO UPWARDS
    // Move the rocket upwards after launch
	for (int j = 0; j < 400; j += 12) 
    {
        cleardevice(); // Clear screen for smooth movement
        drawBackground(); // Maintain background visuals
		drawLaunchPad(300, 300); // Keep launch pad visible
		drawSky(1); // Show the sky during ascent
        y -= 12; // Move rocket upwards

        // Apply camera shake in early stages
        if (j < 50)
        {
            cameraShake(&x, &y, 3);
        }
        
        drawRocket(x, y); // Redraw rocket at new position
        // Redraw flames and smoke while launching
        drawSmokeUp(x, y, 5); // Generate smoke trail upwards
        drawFlamesUp(x, y, 5); // Show flames beneath the rocket
        delay(50); // Short delay for smooth animation
    }
    
	///////////////////////////3  ROCKET IN HIGHER ALTITUTE IN CLOUDS
	x = 300, y = 400; 
    for (int j = 0; j < 500; j += 10) 
    {
        cleardevice(); // Clear screen for smooth transition
        drawBackground(); // Maintain sky background
        drawRocketWithCloudEffect(x, y - j, j); // Rocket passing through clouds
        drawClouds3(j); // Display moving clouds effect
        delay(50); // Small delay for smooth animation
    }
    
	///////////////////////////4  ROCKET ENTERING IN SPACE 
	int rocketX = getmaxx() / 2; // Center the rocket
    int rocketY = getmaxy() - 200; // Set initial position
    int shrinkFactor = 0; // Controls Earth shrinking effect
    int starsCount = 0; // Number of stars in space

    srand(time(0)); // Seed random for stars effect

    while (rocketY > -100) // Continue until rocket exits screen
	{ 
        cleardevice(); // Clear screen for each frame
        setbkcolor(BLACK); // Set background to space
        cleardevice(); // Ensure a clean black background
        drawEarth(shrinkFactor); // Gradually shrink Earth
        drawStars(starsCount); // Increase star count over time
        drawPlanets(shrinkFactor); // Display planets in space
        drawRocket4(rocketX, rocketY); // Draw rocket moving upwards

        shrinkFactor += 3; // Earth shrinks slowly
        rocketY -= 3; // Move rocket up
        if (starsCount < 500) 
			starsCount += 3; // Gradually increase stars for depth effect

        delay(20); // Smooth animation delay
    }
    
    ///////////////////////////4.1  ROCKET MOVING IN ORBIT ALONG WITH OTHER PLANETS
    int sunX = 320, sunY = 240; // Sun position at center
    int earthX = sunX + 150, earthY = sunY; // Initial Earth position
    float angleRocket = 0, angleMercury = 0, angleVenus = 0, angleEarth = 0, angleMars = 0;

    for (int t = 0; t < 90; t++) 
	{
        cleardevice(); // Clear screen for smooth animation

        drawSun(sunX, sunY); // Draw Sun at the center

        // Draw planetary orbits
        setcolor(DARKGRAY);
        ellipse(sunX, sunY, 0, 360, 80, 50);   // Mercury Orbit
        ellipse(sunX, sunY, 0, 360, 130, 80);  // Venus Orbit
        ellipse(sunX, sunY, 0, 360, 180, 110); // Earth Orbit
        ellipse(sunX, sunY, 0, 360, 230, 140); // Mars Orbit 

        // Calculate planet positions 
        int mercuryX = sunX + 80 * cos(angleMercury);
        int mercuryY = sunY + 50 * sin(angleMercury);
        int venusX = sunX + 130 * cos(angleVenus);
        int venusY = sunY + 80 * sin(angleVenus);
        int earthX = sunX + 180 * cos(angleEarth);
        int earthY = sunY + 110 * sin(angleEarth);
        int marsX = sunX + 230 * cos(angleMars);  
        int marsY = sunY + 140 * sin(angleMars);

        // Draw planets
        drawPlanet(mercuryX, mercuryY, 5, LIGHTCYAN);
        drawPlanet(venusX, venusY, 8, LIGHTGREEN);
        drawPlanet(earthX, earthY, 10, BLUE);
        drawPlanet(marsX, marsY, 7, RED); 

        // Calculate Rocket Position orbiting Earth
        int rocketX = earthX + 40 * cos(angleRocket);
        int rocketY = earthY + 40 * sin(angleRocket);

        // Draw the Rocket orbiting Earth
        drawRocket41(rocketX, rocketY);

        // Update angles for movement
        angleRocket += 0.1;
        angleMercury += 0.15;
        angleVenus += 0.1;
        angleEarth += 0.08;  // Earth moves slightly slower
        angleMars += 0.05;   // Mars moves even slower

        delay(50); // Delay for animation effect
    }
    
    ///////////////////////////4.2  ROCKET MOVING AROUND EARTH
    earthX = getmaxx() / 2;
    earthY = getmaxy() / 2;
    int orbitRadius = 150; // Orbit radius around Earth
	starsCount = 0;
    float angle = 0;

    for (int t=0; t<70; t++) 
	{
        cleardevice(); // Clear screen
        drawBackground42(); // Space background
        drawEarth42(earthX, earthY); // Draw Earth
        drawStars(starsCount); // Draw stars in background

        // Compute rocket position in orbit
        int rocketX = earthX + orbitRadius * cos(angle);
        int rocketY = earthY + orbitRadius * sin(angle);

        // Scale the rocket based on depth (simulating 3D effect)
        float scale = 0.7 + 0.3 * cos(angle);

        drawRocket42(rocketX, rocketY, scale, angle); // Draw orbiting rocket

        if (starsCount < 500) 	
			starsCount += 2; // Gradually increase stars

        delay(50); // Delay for smooth animation
        angle += 0.1; // Update angle for rotation
    }
 
    ///////////////////////////5  ROCKET MOVING AMONG ASTEROIDS
    rocketX = getmaxx() / 2, rocketY = getmaxy() - 100; // Initial rocket position
    sunX = getmaxx() - 100, sunY = 80; // Sun position
    int frame = 0; // Animation frame counter

    // Asteroid initialization
    const int numAsteroids = 20;
    int asteroidPositions[numAsteroids][2]; // Store asteroid positions
    int asteroidDirections[numAsteroids][2]; // Store movement directions
    srand(time(0));
    for (int i = 0; i < numAsteroids; i++) 
    {
        asteroidPositions[i][0] = rand() % getmaxx(); // Random x position
        asteroidPositions[i][1] = rand() % getmaxy(); // Random y position
        asteroidDirections[i][1] = (rand() % 2); // Random movement direction
    }

    while (rocketY > -100) // Loop until rocket exits the screen
    { 
        cleardevice();
        setbkcolor(BLACK); // Set black background
        cleardevice();
        drawSun5(sunX, sunY, frame); // Draw the Sun with animated rays

        // Draw planets (Mercury, Venus, Mars)
        drawPlanet5(getmaxx() - 150, 120, 10, LIGHTGRAY); // Mercury
        drawPlanet5(getmaxx() - 200, 180, 15, BROWN);     // Venus
        drawPlanet5(getmaxx() - 300, 250, 20, RED);       // Mars

        drawAsteroids5(numAsteroids, asteroidPositions); // Draw asteroids
        moveAsteroids5(numAsteroids, asteroidPositions, asteroidDirections); // Move asteroids
     
        drawRocket5(rocketX, rocketY); // Draw Rocket

        rocketY -= 5; // Move rocket upwards slowly
        frame++; // Animation frame counter

        delay(30); // Delay for smooth animation
    }
    
    ///////////////////////////5.1  ROCKET MOVING AROUND THE MARS
    int marsX = getmaxx() / 2;
    int marsY = getmaxy() / 2;
    int marsRadius = 100; // Mars orbit radius
    
    angle = 0; // Initial rocket angle
    
    for( int i=0; i<80; i++) 
	{
        cleardevice();
        drawBackground51(); // Draw background space
        drawMars51(marsX, marsY, marsRadius); // Draw Mars
        drawRocket51(marsX, marsY, angle); // Draw rocket orbiting Mars

        angle += 5; // Increment angle to move rocket in orbit
        if (angle > 360) // Reset angle after full orbit
			angle = 0;

        delay(25); // Delay for smooth animation
    }
     
    ///////////////////////////6  ROCKET LANDING ON THE MARS SURFACE
	rocketX = getmaxx() / 2;
    rocketY = 50; // Start high in space
    bool landingGear = false; // Landing gear initially retracted

    while (rocketY < getmaxy() - 140) // Stop slightly above Mars surface
    { 
        cleardevice(); // Clear screen before drawing new frame

        setbkcolor(BLACK); // Space background
        drawMarsSurface6(); // Draw Mars Surface
        drawRocket6(rocketX, rocketY, landingGear); // Draw rocket

        rocketY += 5; // Smooth descent
        if (rocketY > getmaxy() - 200) 
        {
            landingGear = true; // Deploy landing gear near Mars
        }

        delay(5); // Control animation speed
    }
    
    ///////////////////////////7  ASTRONAUT COMING OUT OF THE ROCKET ON MARS
    int maxY = getmaxy();
    int astronautX = 320; // Start position of astronaut
    int astronautY = maxY - 120; // Ground level
    int flagX = astronautX + 15;  // Align with astronaut's right hand
	int flagY = astronautY + 20;  // Match astronaut's arm height

    // Draw Mars Surface & Rocket
    cleardevice(); // Clear screen for next scene
    drawMarsSurface6(); // Draw Mars surface
    drawRocket6(rocketX, rocketY, landingGear); // Draw landed rocket
    delay(1000); // Pause before astronaut exits

    // Astronaut Walks Forward
    for (int i = 0; i < 100; i=i+5)
    {
        cleardevice();
        drawMarsSurface6();
        drawRocket6(rocketX, rocketY, landingGear);
        drawAstronaut7(astronautX + i, astronautY); // Move astronaut step by step
        delay(10);
    }
  
    drawFlag7(flagX + 100, flagY); // Plant the Flag
    delay(1500);
	
	//////////////////////////8  CONTROL SCREEN AFTER THE ROCKET LANDING
	for(int i=0; i<200; i++)
	{
		setbkcolor(BLACK);  
	    cleardevice();
	
	    drawCelebrationScene(); // Display control room celebration

	}
    
    getch();
    closegraph();
    return 0;
}

