#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>

// Window dimensions
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 1500;

// Fish positions
float kiteFishX = 0;   // Kite fish's initial X position, starts from the left
float kiteFishY = 750; // Kite fish's Y position
float ellipseFishX = 1500; // Ellipse fish's initial X position, starts from the right
float sharkX = 800;    // Shark's initial X position
float starfishX = 400; // Starfish's initial X position
float octopusX = 1200; // Octopus's initial X position
float upperFish1X = 200; // Upper fish 1 position
float upperFish2X = 1000; // Upper fish 2 position
bool isMoving = false;

// Bubble structure
struct Bubble {
    float x, y;
    float size;
    float speed;
};

std::vector<Bubble> bubbles;

// Vector to store bubble positions and sizes
std::vector<std::pair<std::pair<float, float>, float>> bubblePositions;
std::vector<std::pair<float, float>> fishBubbles; // Bubbles from fish mouths

// Function declarations
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
void drawFishEye(float x, float y, float radius);
void drawFishBody(float cx, float cy, float width, float height);
void drawFishTail(float bodyLeftX, float cy, float tailWidth, float tailHeight);
void drawGrass();
void drawMoreGrass();
void drawLongGrass();
void drawLongGrass_2();
void drawSand();
void drawBubbles();
void drawBubble(float x, float y, float radius);
void h(int rx, int ry, int cx, int cy);
void t(int x, int y);
void generateBubbles();
void updateFishBubbles();
void drawShipwreck();
void drawUpperFish1(float x, float y);
void drawUpperFish2(float x, float y);

// Function to generate bubbles
void generateBubbles() {
    bubblePositions.clear();
    for (int i = 0; i < 50; i++) {
        float x = rand() % 1400 + 100;
        float y = rand() % 1300 + 200;
        float size = rand() % 10 + 2;
        bubblePositions.push_back({ {x, y}, size });
    }
}

// Update fish bubbles
void updateFishBubbles() {
    // Remove bubbles that have gone too high
    for (int i = 0; i < fishBubbles.size(); i++) {
        fishBubbles[i].second += 2 + rand() % 3; // Move bubbles upward
        
        // Remove if too high
        if (fishBubbles[i].second > WINDOW_HEIGHT) {
            fishBubbles.erase(fishBubbles.begin() + i);
            i--;
        }
    }
    
    // Generate new bubbles from fish mouths
    if (rand() % 10 == 0) {
        fishBubbles.push_back(std::make_pair(kiteFishX + 10, kiteFishY + 10));
    }
    if (rand() % 10 == 0) {
        fishBubbles.push_back(std::make_pair(ellipseFishX - 470, 550));
    }
    if (rand() % 10 == 0) {
        fishBubbles.push_back(std::make_pair(ellipseFishX - 370, 450));
    }
    if (rand() % 15 == 0) {
        fishBubbles.push_back(std::make_pair(sharkX + 20, 600));
    }
    if (rand() % 10 == 0) {
        fishBubbles.push_back(std::make_pair(upperFish1X + 15, 1000));
    }
    if (rand() % 10 == 0) {
        fishBubbles.push_back(std::make_pair(upperFish2X + 15, 1200));
    }
}

// Function to draw a bubble
void drawBubble(float x, float y, float radius) {
    glColor3f(0.94, 1.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float theta = i * M_PI / 180;
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
}

void init() {
    srand(time(NULL)); // Initialize random seed
    glClearColor(0.0, 0.41, 0.58, 1.0);
    gluOrtho2D(0.0, 1500.0, 0.0, 1500.0);
    generateBubbles();
}

void drawFish(float x, float y) {
    // Body
    glColor3f(1.0f, 0.6f, 0.0f); // Orange color
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x - 30, y - 20);
    glVertex2f(x - 30, y + 20);
    glEnd();
    
    // Tail
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 30, y);
    glVertex2f(x - 60, y - 30);
    glVertex2f(x - 60, y + 30);
    glEnd();
    
    // Eye
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 5, y + 5);
    for(int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180;
        glVertex2f(x + 5 + 3 * cos(angle), y + 5 + 3 * sin(angle));
    }
    glEnd();
    
    // Pupil
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 5, y + 5);
    for(int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180;
        glVertex2f(x + 5 + 1.5 * cos(angle), y + 5 + 1.5 * sin(angle));
    }
    glEnd();
}

void drawAquarium() {
    // Bottom sand
    glColor3f(0.8f, 0.7f, 0.5f); // Sand color
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, 100);
    glVertex2f(0, 100);
    glEnd();
    
    // Plants
    glColor3f(0.0f, 0.8f, 0.0f); // Green color
    for(int i = 0; i < 5; i++) {
        float x = 100 + i * 150;
        glBegin(GL_TRIANGLES);
        glVertex2f(x, 100);
        glVertex2f(x - 20, 200);
        glVertex2f(x + 20, 200);
        glEnd();
    }
}

void drawShark(float x, float y) {
    // Shark body - larger diamond shape
    glColor3f(0.0, 0.0, 0.0); // Black color for shark
    glBegin(GL_POLYGON);
    glVertex2f(x + 50, y);
    glVertex2f(x - 150, y + 80);
    glVertex2f(x - 300, y);
    glVertex2f(x - 150, y - 80);
    glEnd();
    
    // Shark tail
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 280, y);
    glVertex2f(x - 380, y + 100);
    glVertex2f(x - 380, y - 100);
    glEnd();
    
    // Shark dorsal fin
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 150, y + 80);
    glVertex2f(x - 160, y + 140);
    glVertex2f(x - 200, y + 70);
    glEnd();
    
    // Shark side fins
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 150, y - 70);
    glVertex2f(x - 200, y - 130);
    glVertex2f(x - 100, y - 80);
    glEnd();
    
    // Shark eye
    drawFishEye(x, y + 20, 12);
    
    // Shark mouth
    glColor3f(0.7, 0.0, 0.0); // Red
    glBegin(GL_LINES);
    glVertex2f(x + 50, y);
    glVertex2f(x, y - 10);
    glEnd();
    
    // Draw small fish being eaten (if shark is on screen)
    if (x > 0 && x < WINDOW_WIDTH) {
        // Fish body (half visible, being eaten)
        glColor3f(1.0, 0.7, 0.3); // Orange-gold color
        glBegin(GL_TRIANGLES);
        glVertex2f(x + 40, y - 5);
        glVertex2f(x + 60, y - 15);
        glVertex2f(x + 60, y + 5);
        glEnd();
        
        // Fish tail (sticking out of shark's mouth)
        glColor3f(0.9, 0.6, 0.2);
        glBegin(GL_TRIANGLES);
        glVertex2f(x + 60, y - 5);
        glVertex2f(x + 80, y - 15);
        glVertex2f(x + 80, y + 5);
        glEnd();
        
        // Blood effect
        glColor3f(0.8, 0.0, 0.0);
        glPointSize(3.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < 8; i++) {
            glVertex2f(x + 45 + rand() % 10, y - 5 + rand() % 10);
        }
        glEnd();
        glPointSize(1.0);
    }
}

void drawStarfish(float x, float y) {
    // Double the size and make it more star-like
    glColor3f(1.0, 0.5, 0.0); // Orange color
    
    // Draw a 5-pointed star
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center
    
    for (int i = 0; i <= 5; i++) {
        float angle1 = i * 72 * M_PI / 180;
        float angle2 = (i * 72 + 36) * M_PI / 180;
        
        float outerRadius = 60; // Doubled from 30
        float innerRadius = 30; // Doubled from 15
        
        glVertex2f(x + outerRadius * cos(angle1), y + outerRadius * sin(angle1));
        glVertex2f(x + innerRadius * cos(angle2), y + innerRadius * sin(angle2));
    }
    glEnd();
    
    // Starfish center
    glColor3f(0.8, 0.4, 0.0); // Darker orange
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * M_PI / 180;
        glVertex2f(x + 30 * cos(theta), y + 30 * sin(theta));
    }
    glEnd();
    
    // Add an eye in the center
    drawFishEye(x, y, 8);
}

void drawOctopus(float x, float y) {
    // Larger octopus body
    glColor3f(0.8, 0.4, 0.8); // Purple color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * M_PI / 180;
        glVertex2f(x + 60 * cos(theta), y + 60 * sin(theta)); // Increased from 40
    }
    glEnd();
    
    // Octopus tentacles - longer and more detailed
    for (int i = 0; i < 8; i++) {
        float angle = i * 45 * M_PI / 180;
        glBegin(GL_TRIANGLE_STRIP);
        float tentacleLength = 100; // Increased from 60
        for (float j = 0; j <= 1; j += 0.05) {
            float width = 15 * (1 - j); // Increased width
            float curveFactor = 20 * sin(j * 5); // Add wavy motion
            
            float xPos = x + (60 + tentacleLength * j) * cos(angle) - width * sin(angle) + curveFactor * cos(angle + M_PI/2);
            float yPos = y + (60 + tentacleLength * j) * sin(angle) + width * cos(angle) + curveFactor * sin(angle + M_PI/2);
            
            glVertex2f(xPos, yPos);
            
            xPos = x + (60 + tentacleLength * j) * cos(angle) + width * sin(angle) + curveFactor * cos(angle + M_PI/2);
            yPos = y + (60 + tentacleLength * j) * sin(angle) - width * cos(angle) + curveFactor * sin(angle + M_PI/2);
            
            glVertex2f(xPos, yPos);
        }
        glEnd();
    }
    
    // Octopus eyes (two eyes for more character)
    drawFishEye(x + 25, y + 20, 10); // Right eye, larger
    drawFishEye(x - 25, y + 20, 10); // Left eye, larger
}

// Function to draw grass
void drawGrass() {
    glColor3f(0.0, 0.8, 0.0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 1500; i += 30) {
        glVertex2f(i, 150);
        glVertex2f(i + 15, 230);
        glVertex2f(i + 30, 150);
    }
    glEnd();
}

// Function to draw dark green grass
void drawMoreGrass() {
    glColor3f(0.0, 0.5, 0.0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 1500; i += 20) {
        glVertex2f(i, 150);
        glVertex2f(i + 10, 220);
        glVertex2f(i + 20, 150);
    }
    glEnd();
}

// Function to draw long grass
void drawLongGrass() {
    glColor3f(0.0, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 1500; i += 30) {
        glVertex2f(i, 150);
        glVertex2f(i + 15, 300);
        glVertex2f(i + 30, 150);
    }
    glEnd();
}

void drawLongGrass_2() {
    glColor3f(0.0, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 1500; i += 35) {
        glVertex2f(i, 100);
        glVertex2f(i + 30, 300);
        glVertex2f(i + 40, 100);
    }
    glEnd();
}

// Function to draw sand
void drawSand() {
    glColor3f(0.8, 0.7, 0.5);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 1500; i += 10) {
        glVertex2f(i, 200 + 15 * sin(i * M_PI / 180));
        glVertex2f(i, 0);
    }
    glEnd();
}

// Function to draw all bubbles
void drawBubbles() {
    for (auto& pos : bubblePositions) {
        drawBubble(pos.first.first, pos.first.second, pos.second);
    }
}

// Function to draw shipwreck (modern cargo/cruise ship style)
void drawShipwreck() {
    // Ship position and scale
    float shipX = 600;
    float shipY = 400;
    float shipScale = 1.0; // Make it larger
    
    glPushMatrix();
    glTranslatef(shipX, shipY, 0);
    glScalef(shipScale, shipScale, 1.0);
    
    // Main hull - dark blue
    glColor3f(0.2, 0.3, 0.5);
    glBegin(GL_POLYGON);
    // Hull shape
    glVertex2f(-300, -200); // Bottom left
    glVertex2f(300, -200);  // Bottom right
    glVertex2f(350, -150);  // Front slope bottom
    glVertex2f(400, -50);   // Front tip
    glVertex2f(350, 0);     // Top front
    glVertex2f(-350, 0);    // Top back
    glVertex2f(-400, -50);  // Back slope
    glVertex2f(-350, -150); // Back bottom
    glEnd();
    
    // Red bottom part of hull
    glColor3f(0.8, 0.2, 0.2);
    glBegin(GL_POLYGON);
    glVertex2f(-300, -200); // Bottom left
    glVertex2f(300, -200);  // Bottom right
    glVertex2f(350, -150);  // Front slope bottom
    glVertex2f(-350, -150); // Back bottom
    glEnd();
    
    // Main deck - white
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
    glVertex2f(-350, 0);    // Bottom back
    glVertex2f(350, 0);     // Bottom front
    glVertex2f(320, 100);   // Top front
    glVertex2f(-320, 100);  // Top back
    glEnd();
    
    // Upper deck 1 - white
    glColor3f(0.95, 0.95, 0.95);
    glBegin(GL_POLYGON);
    glVertex2f(-250, 100);  // Bottom back
    glVertex2f(250, 100);   // Bottom front
    glVertex2f(230, 160);   // Top front
    glVertex2f(-230, 160);  // Top back
    glEnd();
    
    // Upper deck 2 - white
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-200, 160);  // Bottom back
    glVertex2f(200, 160);   // Bottom front
    glVertex2f(180, 210);   // Top front
    glVertex2f(-180, 210);  // Top back
    glEnd();
    
    // Command bridge - white with gray top
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_POLYGON);
    glVertex2f(-100, 210);  // Bottom back
    glVertex2f(100, 210);   // Bottom front
    glVertex2f(80, 250);    // Top front
    glVertex2f(-80, 250);   // Top back
    glEnd();
    
    // Windows on main deck (blue glass)
    glColor3f(0.4, 0.7, 0.9);
    for (int i = -300; i < 300; i += 40) {
        glBegin(GL_QUADS);
        glVertex2f(i, 20);
        glVertex2f(i + 30, 20);
        glVertex2f(i + 30, 60);
        glVertex2f(i, 60);
        glEnd();
        
        // Window frames (dark gray)
        glColor3f(0.3, 0.3, 0.3);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(i, 20);
        glVertex2f(i + 30, 20);
        glVertex2f(i + 30, 60);
        glVertex2f(i, 60);
        glEnd();
        glLineWidth(1.0);
        glColor3f(0.4, 0.7, 0.9);
    }
    
    // Windows on upper deck 1
    for (int i = -230; i < 230; i += 35) {
        glBegin(GL_QUADS);
        glVertex2f(i, 120);
        glVertex2f(i + 25, 120);
        glVertex2f(i + 25, 145);
        glVertex2f(i, 145);
        glEnd();
        
        // Window frames
        glColor3f(0.3, 0.3, 0.3);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(i, 120);
        glVertex2f(i + 25, 120);
        glVertex2f(i + 25, 145);
        glVertex2f(i, 145);
        glEnd();
        glLineWidth(1.0);
        glColor3f(0.4, 0.7, 0.9);
    }
    
    // Windows on upper deck 2
    for (int i = -180; i < 180; i += 30) {
        glBegin(GL_QUADS);
        glVertex2f(i, 175);
        glVertex2f(i + 20, 175);
        glVertex2f(i + 20, 195);
        glVertex2f(i, 195);
        glEnd();
        
        // Window frames
        glColor3f(0.3, 0.3, 0.3);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(i, 175);
        glVertex2f(i + 20, 175);
        glVertex2f(i + 20, 195);
        glVertex2f(i, 195);
        glEnd();
        glLineWidth(1.0);
        glColor3f(0.4, 0.7, 0.9);
    }
    
    // Windows on command bridge
    for (int i = -70; i < 70; i += 25) {
        glBegin(GL_QUADS);
        glVertex2f(i, 220);
        glVertex2f(i + 15, 220);
        glVertex2f(i + 15, 240);
        glVertex2f(i, 240);
        glEnd();
        
        // Window frames
        glColor3f(0.3, 0.3, 0.3);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(i, 220);
        glVertex2f(i + 15, 220);
        glVertex2f(i + 15, 240);
        glVertex2f(i, 240);
        glEnd();
        glLineWidth(1.0);
        glColor3f(0.4, 0.7, 0.9);
    }
    
    // Railings
    glColor3f(0.7, 0.7, 0.7);
    glLineWidth(2.0);
    
    // Deck 1 railings
    glBegin(GL_LINES);
    for (int i = -320; i <= 320; i += 15) {
        glVertex2f(i, 100);
        glVertex2f(i, 115);
    }
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2f(-320, 115);
    glVertex2f(320, 115);
    glEnd();
    
    // Deck 2 railings
    glBegin(GL_LINES);
    for (int i = -230; i <= 230; i += 15) {
        glVertex2f(i, 160);
        glVertex2f(i, 175);
    }
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2f(-230, 175);
    glVertex2f(230, 175);
    glEnd();
    
    // Deck 3 railings
    glBegin(GL_LINES);
    for (int i = -180; i <= 180; i += 15) {
        glVertex2f(i, 210);
        glVertex2f(i, 225);
    }
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2f(-180, 225);
    glVertex2f(180, 225);
    glEnd();
    
    glLineWidth(1.0);
    
    // Smokestacks/funnels
    glColor3f(0.8, 0.3, 0.3); // Red funnels
    
    // Funnel 1
    glBegin(GL_POLYGON);
    glVertex2f(-150, 250);
    glVertex2f(-120, 250);
    glVertex2f(-115, 320);
    glVertex2f(-155, 320);
    glEnd();
    
    // Funnel 2
    glBegin(GL_POLYGON);
    glVertex2f(-90, 250);
    glVertex2f(-60, 250);
    glVertex2f(-55, 320);
    glVertex2f(-95, 320);
    glEnd();
    
    // Black tops of funnels
    glColor3f(0.2, 0.2, 0.2);
    
    // Funnel 1 top
    glBegin(GL_POLYGON);
    glVertex2f(-155, 320);
    glVertex2f(-115, 320);
    glVertex2f(-120, 335);
    glVertex2f(-150, 335);
    glEnd();
    
    // Funnel 2 top
    glBegin(GL_POLYGON);
    glVertex2f(-95, 320);
    glVertex2f(-55, 320);
    glVertex2f(-60, 335);
    glVertex2f(-90, 335);
    glEnd();
    
    // Shadow/water line at waterline
    glColor3f(0.1, 0.2, 0.4);
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-350, -50);  // Back slope
    glVertex2f(-400, -50);  // Back tip
    glVertex2f(-350, -150); // Back bottom
    glVertex2f(-300, -200); // Bottom left
    glVertex2f(300, -200);  // Bottom right
    glVertex2f(350, -150);  // Front slope bottom
    glVertex2f(400, -50);   // Front tip
    glVertex2f(350, 0);     // Top front
    glEnd();
    glLineWidth(1.0);
    
    // White foam/wake around ship
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 20; i++) {
        float angle = i * M_PI / 10;
        float radius = 30 + 10 * sin(i * 1.5);
        glVertex2f(400 + radius * cos(angle), -50 + radius * sin(angle));
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 20; i++) {
        float angle = i * M_PI / 10 + M_PI;
        float radius = 30 + 10 * sin(i * 1.5);
        glVertex2f(-400 + radius * cos(angle), -50 + radius * sin(angle));
    }
    glEnd();
    
    glPopMatrix();
}

// Function to draw upper fish 1 (colorful tropical fish)
void drawUpperFish1(float x, float y) {
    // Body (oval)
    glColor3f(1.0, 0.2, 0.4); // Pink-red color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * M_PI / 180;
        float rx = 40;
        float ry = 25;
        glVertex2f(x + rx * cos(theta), y + ry * sin(theta));
    }
    glEnd();
    
    // Tail
    glColor3f(1.0, 0.8, 0.2); // Yellow
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 35, y);
    glVertex2f(x - 70, y + 25);
    glVertex2f(x - 70, y - 25);
    glEnd();
    
    // Fins (top and bottom)
    glColor3f(1.0, 0.6, 0.2); // Orange
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 20);
    glVertex2f(x - 10, y + 40);
    glVertex2f(x + 10, y + 40);
    glEnd();
    
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y - 20);
    glVertex2f(x - 10, y - 40);
    glVertex2f(x + 10, y - 40);
    glEnd();
    
    // Eye
    drawFishEye(x + 25, y + 5, 6);
}

// Function to draw upper fish 2 (angel fish with movement fixed)
void drawUpperFish2(float x, float y) {
    // Body (diamond) - now facing left
    glColor3f(0.2, 0.4, 1.0); // Blue color
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + 20, y + 50);
    glVertex2f(x + 40, y);
    glVertex2f(x + 20, y - 50);
    glEnd();
    
    // Tail (facing left)
    glColor3f(0.2, 0.2, 0.8); // Darker blue
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x - 40, y + 30);
    glVertex2f(x - 40, y - 30);
    glEnd();
    
    // Eye (facing left)
    drawFishEye(x + 30, y, 5);
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Background
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.2, 0.4);
    glVertex2f(0, 1500);
    glColor3f(0.0, 0.4, 0.6);
    glVertex2f(1500, 1500);
    glColor3f(0.0, 0.1, 0.3);
    glVertex2f(1500, 0);
    glColor3f(0.0, 0.3, 0.5);
    glVertex2f(0, 0);
    glEnd();

    // Draw sand and grass (environment)
    drawSand();
    drawGrass();
    drawMoreGrass();
    drawLongGrass();
    drawLongGrass_2();
    
    // Draw shipwreck
    drawShipwreck();
    
    // Draw bubbles
    drawBubbles();
    
    // Draw fish bubbles
    for (const auto& bubble : fishBubbles) {
        drawBubble(bubble.first, bubble.second, 3 + rand() % 3);
    }

    // Draw ellipse fish (moving left)
    h(60, 32, ellipseFishX - 500, 550);
    h(70, 42, ellipseFishX - 400, 450);
    t(ellipseFishX - 500, 550);
    t(ellipseFishX - 400, 450);
    drawFishEye(ellipseFishX - 525, 556, 10); // Eye for first ellipse fish
    drawFishEye(ellipseFishX - 430, 460, 10); // Eye for second ellipse fish

    // Draw kite fish (moving right)
    float fishWidth = 200, fishHeight = 100;
    float tailWidth = 80, tailHeight = 120;
    float eyeRadius = 10;

    drawFishBody(kiteFishX, kiteFishY, fishWidth, fishHeight);
    drawFishTail(kiteFishX - fishWidth / 2, kiteFishY, tailWidth, tailHeight);
    drawFishEye(kiteFishX + fishWidth / 4, kiteFishY + fishHeight / 4, eyeRadius);

    // Draw upper fish
    drawUpperFish1(upperFish1X, 1000);
    drawUpperFish2(upperFish2X, 1200);

    // Draw new creatures
    drawShark(sharkX, 600);
    drawStarfish(starfishX, 300);
    drawOctopus(octopusX, 400);

    glFlush();
}

// Fish movement function - fix the upper fish direction
void update(int value) {
    if (isMoving) {
        // Kite fish moves right
        kiteFishX += 5;
        if (kiteFishX > 1600) kiteFishX = -100;

        // Ellipse fish moves left
        ellipseFishX -= 5;
        if (ellipseFishX < -100) ellipseFishX = 1600;

        // Shark moves right
        sharkX += 3; // Slower movement for shark
        if (sharkX > 1600) sharkX = -400; // Adjusted for larger size

        // Starfish moves left very slowly
        starfishX -= 1;
        if (starfishX < -100) starfishX = 1600;

        // Octopus moves right
        octopusX += 2; // Slower movement
        if (octopusX > 1600) octopusX = -100;
        
        // Upper fish movement
        upperFish1X += 7; // Fast, moving right
        if (upperFish1X > 1600) upperFish1X = -100;
        
        // Fixed direction for upperFish2 to move left (matching its eye direction)
        upperFish2X += 7; // Now moving left correctly
        if (upperFish2X > 1600) upperFish2X = -100;
        
        // Update fish bubbles
        updateFishBubbles();

        glutPostRedisplay();
    }
    glutTimerFunc(50, update, 0);
}

// Keyboard Input (M = Move, S = Stop)
void keyboard(unsigned char key, int x, int y) {
    if (key == 'm' || key == 'M') isMoving = true;
    if (key == 's' || key == 'S') isMoving = false;
    if (key == 27) exit(0); // ESC key to exit
}

// Function to draw ellipse fish
void h(int rx, int ry, int cx, int cy) {
    glColor3f(0.0, 1.0, 0.0); // Green color for the ellipse fish
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of ellipse
    for (int i = 0; i <= 360; i++) {
        float theta = i * M_PI / 180;
        float x = rx * cos(theta);
        float y = ry * sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Function to draw fish tail
void t(int x, int y) {
    glColor3f(0.0, 0.0, 1.0); // Blue color for the tail
    glBegin(GL_TRIANGLES);
    glVertex2i(x + 70, y);
    glVertex2i(x + 120, y + 70);
    glVertex2i(x + 120, y - 70);
    glEnd();
}

// Function to draw fish eye
void drawFishEye(float x, float y, float radius) {
    glColor3f(1.0, 1.0, 1.0); // White eye
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float theta = i * M_PI / 180;
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0); // Black pupil
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float theta = i * M_PI / 180;
        glVertex2f(x + (radius / 2) * cos(theta), y + (radius / 2) * sin(theta));
    }
    glEnd();
}

// Function to draw fish body
void drawFishBody(float cx, float cy, float width, float height) {
    glColor3f(1.0, 0.6, 0.0); // Orange color for the kite fish
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy + height / 2);
    glVertex2f(cx - width / 2, cy);
    glVertex2f(cx, cy - height / 2);
    glVertex2f(cx + width / 2, cy);
    glEnd();
}

// Function to draw fish tail
void drawFishTail(float bodyLeftX, float cy, float tailWidth, float tailHeight) {
    glColor3f(0.0, 0.0, 1.0); // Blue tail
    glBegin(GL_TRIANGLES);
    glVertex2f(bodyLeftX, cy);
    glVertex2f(bodyLeftX - tailWidth, cy + tailHeight / 2);
    glVertex2f(bodyLeftX - tailWidth, cy - tailHeight / 2);
    glEnd();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Aquarium Scene with Moving Fishes");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, update, 0);
    glutMainLoop();
    return 0;
} 