#include <GL/glut.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

bool moveMode = false;

// Window properties
int winWidth = 800, winHeight = 600;
std::string winTitle = "SimpleDraw (Your Name)";

// Drawing state
enum Shape { NONE, RECTANGLE, CIRCLE };
Shape currentShape = NONE;

struct Style {
    float fillColor[3]   = {0, 0, 0}; // black
    float strokeColor[3] = {0, 0, 0}; // black
    int strokeWidth      = 1;
} style;

struct Drawable {
    Shape type;
    float x, y, w, h;
    Style style;
    Drawable* next = nullptr;  // pointer to next object
};

// Head pointer for the linked list
Drawable* head = nullptr;
std::vector<Drawable> objects; // using vector for simplicity

// --- New Global Variables ---
int selectedIndex = -1;     // -1 means nothing selected
bool dragging = false;      // for "Move Around"
int dragOffsetX, dragOffsetY;

// --- Rubber band drawing variables ---
bool isDrawing = false;
int startX, startY;
int currentX, currentY;

// --------------------- Midpoint Circle Algorithm ------------------------
void plotCirclePoints(int cx, int cy, int x, int y, std::vector<std::pair<int,int>>& points) {
    points.push_back({cx + x, cy + y});
    points.push_back({cx - x, cy + y});
    points.push_back({cx + x, cy - y});
    points.push_back({cx - x, cy - y});
    points.push_back({cx + y, cy + x});
    points.push_back({cx - y, cy + x});
    points.push_back({cx + y, cy - x});
    points.push_back({cx - y, cy - x});
}

std::vector<std::pair<int,int>> midpointCircle(int cx, int cy, int radius) {
    std::vector<std::pair<int,int>> points;
    int x = 0;
    int y = radius;
    int p = 1 - radius;
    
    plotCirclePoints(cx, cy, x, y, points);
    
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        plotCirclePoints(cx, cy, x, y, points);
    }
    
    return points;
}

void drawMidpointCircle(int cx, int cy, int radius, bool fill, float* fillColor, float* strokeColor, int strokeWidth) {
    if (radius <= 0) return;
    
    // Fill the circle
    if (fill) {
        glColor3fv(fillColor);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int angle = 0; angle <= 360; angle++) {
            float rad = angle * 3.14159f / 180.0f;
            glVertex2f(cx + radius * cos(rad), cy + radius * sin(rad));
        }
        glEnd();
    }
    
    // Draw outline using Midpoint algorithm
    std::vector<std::pair<int,int>> circlePoints = midpointCircle(cx, cy, radius);
    
    glColor3fv(strokeColor);
    glPointSize(strokeWidth);
    glBegin(GL_POINTS);
    for (const auto& point : circlePoints) {
        glVertex2i(point.first, point.second);
    }
    glEnd();
}

// --------------------- Select Shape ------------------------
int findShapeAt(int x, int y) {
    int idx = 0, found = -1;
    Drawable* curr = head;
    while (curr) {
        if (curr->type == RECTANGLE) {
            if (x >= curr->x && x <= curr->x + curr->w &&
                y >= curr->y && y <= curr->y + curr->h)
                found = idx;
        } else if (curr->type == CIRCLE) {
            float dx = x - curr->x, dy = y - curr->y;
            if (dx*dx + dy*dy <= curr->w * curr->w) found = idx;
        }
        curr = curr->next;
        idx++;
    }
    return found;
}


// --------------------- Utility Functions ------------------------
void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

void resetCanvas() {
    // Clear linked list
    while (head) {
        Drawable* temp = head;
        head = head->next;
        delete temp;
    }
    objects.clear();
    style = Style();
    currentShape = NONE;
    selectedIndex = -1;
    glutPostRedisplay();
}

// --------------------- Drawing ------------------------
void drawRectangle(Drawable &obj) {
    // Fill
    glColor3fv(obj.style.fillColor);
    glBegin(GL_QUADS);
        glVertex2f(obj.x, obj.y);
        glVertex2f(obj.x + obj.w, obj.y);
        glVertex2f(obj.x + obj.w, obj.y + obj.h);
        glVertex2f(obj.x, obj.y + obj.h);
    glEnd();

    // Stroke
    glLineWidth(obj.style.strokeWidth);
    glColor3fv(obj.style.strokeColor);
    glBegin(GL_LINE_LOOP);
        glVertex2f(obj.x, obj.y);
        glVertex2f(obj.x + obj.w, obj.y);
        glVertex2f(obj.x + obj.w, obj.y + obj.h);
        glVertex2f(obj.x, obj.y + obj.h);
    glEnd();
}

void drawCircle(Drawable &obj) {
    int radius = (int)obj.w;
    int cx = (int)obj.x;
    int cy = (int)obj.y;
    
    drawMidpointCircle(cx, cy, radius, true, obj.style.fillColor, obj.style.strokeColor, obj.style.strokeWidth);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    Drawable* curr = head;
    int idx = 0;
    while (curr) {
        if (curr->type == RECTANGLE) drawRectangle(*curr);
        else if (curr->type == CIRCLE) drawCircle(*curr);
        
        // Highlight selected object with yellow outline
        if (idx == selectedIndex) {
            glColor3f(1.0, 1.0, 0.0); // Yellow
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            if (curr->type == RECTANGLE) {
                glVertex2f(curr->x - 2, curr->y - 2);
                glVertex2f(curr->x + curr->w + 2, curr->y - 2);
                glVertex2f(curr->x + curr->w + 2, curr->y + curr->h + 2);
                glVertex2f(curr->x - 2, curr->y + curr->h + 2);
            } else if (curr->type == CIRCLE) {
                int segs = 100;
                float highlightRadius = curr->w + 3;
                for (int i = 0; i < segs; i++) {
                    float theta = 2.0f * 3.14159f * i / segs;
                    float dx = highlightRadius * cosf(theta);
                    float dy = highlightRadius * sinf(theta);
                    glVertex2f(curr->x + dx, curr->y + dy);
                }
            }
            glEnd();
        }
        
        curr = curr->next;
        idx++;
    }

    // Draw rubber band rectangle
    if (isDrawing && currentShape == RECTANGLE) {
        float minX = std::min(startX, currentX);
        float maxX = std::max(startX, currentX);
        float minY = std::min(startY, currentY);
        float maxY = std::max(startY, currentY);
        
        glLineWidth(style.strokeWidth);
        glColor3fv(style.strokeColor);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(2, 0xAAAA);
        glBegin(GL_LINE_LOOP);
            glVertex2f(minX, minY);
            glVertex2f(maxX, minY);
            glVertex2f(maxX, maxY);
            glVertex2f(minX, maxY);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }
    
    // Draw rubber band circle
    if (isDrawing && currentShape == CIRCLE) {
        int dx = currentX - startX;
        int dy = currentY - startY;
        int radius = (int)sqrt(dx * dx + dy * dy);
        
        if (radius > 0) {
            glColor3fv(style.strokeColor);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0xAAAA);
            
            // Draw dashed circle outline
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 100; i++) {
                float theta = 2.0f * 3.14159f * i / 100.0f;
                float x = startX + radius * cosf(theta);
                float y = startY + radius * sinf(theta);
                glVertex2f(x, y);
            }
            glEnd();
            
            glDisable(GL_LINE_STIPPLE);
        }
    }

    glutSwapBuffers();
}

// --------------------- Menu Functions ------------------------
void menuNew(int) {
    resetCanvas();
}

void menuDraw(int option) {
    if (option == 1) {
        currentShape = RECTANGLE;
        moveMode = false;
    }
    else if (option == 2) {
        currentShape = CIRCLE;
        moveMode = false;
    }
}

void menuFillColor(int option) {
    switch(option) {
        case 1: style.fillColor[0]=1; style.fillColor[1]=0; style.fillColor[2]=0; break; // Red
        case 2: style.fillColor[0]=0; style.fillColor[1]=1; style.fillColor[2]=0; break; // Green
        case 3: style.fillColor[0]=0; style.fillColor[1]=0; style.fillColor[2]=1; break; // Blue
        case 4: style.fillColor[0]=0; style.fillColor[1]=0; style.fillColor[2]=0; break; // Black
        case 5: style.fillColor[0]=1; style.fillColor[1]=1; style.fillColor[2]=1; break; // White
    }
}

void menuStrokeColor(int option) {
    switch(option) {
        case 1: style.strokeColor[0]=1; style.strokeColor[1]=0; style.strokeColor[2]=0; break;
        case 2: style.strokeColor[0]=0; style.strokeColor[1]=1; style.strokeColor[2]=0; break;
        case 3: style.strokeColor[0]=0; style.strokeColor[1]=0; style.strokeColor[2]=1; break;
        case 4: style.strokeColor[0]=0; style.strokeColor[1]=0; style.strokeColor[2]=0; break;
        case 5: style.strokeColor[0]=1; style.strokeColor[1]=1; style.strokeColor[2]=1; break;
    }
}

void menuStrokeWidth(int option) {
    style.strokeWidth = option;
}

void saveSVG(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Error: Could not save to " << filename << "\n";
        return;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << winWidth
        << "\" height=\"" << winHeight << "\" version=\"1.1\">\n";

    // Store objects in reverse order (from tail to head) to maintain visual order
    std::vector<Drawable*> objList;
    Drawable* curr = head;
    while (curr) {
        objList.push_back(curr);
        curr = curr->next;
    }

    // Write in reverse order so bottom objects are first
    for (int i = objList.size() - 1; i >= 0; i--) {
        Drawable* obj = objList[i];
        char fill[32], stroke[32];
        sprintf(fill, "rgb(%d,%d,%d)",
                int(obj->style.fillColor[0] * 255),
                int(obj->style.fillColor[1] * 255),
                int(obj->style.fillColor[2] * 255));
        sprintf(stroke, "rgb(%d,%d,%d)",
                int(obj->style.strokeColor[0] * 255),
                int(obj->style.strokeColor[1] * 255),
                int(obj->style.strokeColor[2] * 255));

        if (obj->type == RECTANGLE) {
            out << "  <rect x=\"" << obj->x << "\" y=\"" << (winHeight - obj->y - obj->h)
                << "\" width=\"" << obj->w << "\" height=\"" << obj->h
                << "\" fill=\"" << fill
                << "\" stroke=\"" << stroke
                << "\" stroke-width=\"" << obj->style.strokeWidth << "\" />\n";
        } else if (obj->type == CIRCLE) {
            out << "  <circle cx=\"" << obj->x << "\" cy=\"" << (winHeight - obj->y)
                << "\" r=\"" << obj->w
                << "\" fill=\"" << fill
                << "\" stroke=\"" << stroke
                << "\" stroke-width=\"" << obj->style.strokeWidth << "\" />\n";
        }
    }

    out << "</svg>\n";
    out.close();
    std::cout << "Successfully saved to " << filename << "\n";
}

void exportBitmap(const std::string& filename) {
    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(3 * winWidth * winHeight);
    glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // BMP file header (14 bytes)
    unsigned char bmpFileHeader[14] = {
        'B', 'M',           // Signature
        0, 0, 0, 0,         // File size (filled later)
        0, 0,               // Reserved
        0, 0,               // Reserved
        54, 0, 0, 0         // Offset to pixel data
    };

    // BMP info header (40 bytes)
    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,        // Info header size
        0, 0, 0, 0,         // Image width (filled later)
        0, 0, 0, 0,         // Image height (filled later)
        1, 0,               // Number of color planes
        24, 0,              // Bits per pixel (24-bit RGB)
        0, 0, 0, 0,         // Compression (0 = uncompressed)
        0, 0, 0, 0,         // Image size (can be 0 for uncompressed)
        0, 0, 0, 0,         // Horizontal resolution
        0, 0, 0, 0,         // Vertical resolution
        0, 0, 0, 0,         // Number of colors in palette
        0, 0, 0, 0          // Important colors
    };

    // Calculate row size (must be multiple of 4)
    int rowSize = (winWidth * 3 + 3) & (~3);
    int imageSize = rowSize * winHeight;
    int fileSize = 54 + imageSize;

    // Fill in file size
    bmpFileHeader[2] = (unsigned char)(fileSize);
    bmpFileHeader[3] = (unsigned char)(fileSize >> 8);
    bmpFileHeader[4] = (unsigned char)(fileSize >> 16);
    bmpFileHeader[5] = (unsigned char)(fileSize >> 24);

    // Fill in image dimensions
    bmpInfoHeader[4] = (unsigned char)(winWidth);
    bmpInfoHeader[5] = (unsigned char)(winWidth >> 8);
    bmpInfoHeader[6] = (unsigned char)(winWidth >> 16);
    bmpInfoHeader[7] = (unsigned char)(winWidth >> 24);
    bmpInfoHeader[8] = (unsigned char)(winHeight);
    bmpInfoHeader[9] = (unsigned char)(winHeight >> 8);
    bmpInfoHeader[10] = (unsigned char)(winHeight >> 16);
    bmpInfoHeader[11] = (unsigned char)(winHeight >> 24);

    // Open file for writing
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cout << "Error: Could not save to " << filename << "\n";
        return;
    }

    // Write headers
    out.write((char*)bmpFileHeader, 14);
    out.write((char*)bmpInfoHeader, 40);

    // Write pixel data (BMP format is BGR, bottom-to-top)
    std::vector<unsigned char> padding(rowSize - winWidth * 3, 0);
    
    for (int y = 0; y < winHeight; y++) {
        for (int x = 0; x < winWidth; x++) {
            int idx = (y * winWidth + x) * 3;
            // Convert RGB to BGR
            unsigned char r = pixels[idx];
            unsigned char g = pixels[idx + 1];
            unsigned char b = pixels[idx + 2];
            out.put(b);
            out.put(g);
            out.put(r);
        }
        // Write padding bytes to make row size multiple of 4
        if (padding.size() > 0) {
            out.write((char*)padding.data(), padding.size());
        }
    }

    out.close();
    std::cout << "Successfully exported bitmap to " << filename << "\n";
}

void openSVG(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Error: Could not open " << filename << "\n";
        return;
    }

    resetCanvas();
    
    std::string line;
    std::vector<Drawable*> loadedObjects;
    
    while (std::getline(in, line)) {
        if (line.find("<rect") != std::string::npos) {
            Drawable* obj = new Drawable();
            obj->type = RECTANGLE;
            
            float x, y, w, h;
            int r, g, b, sr, sg, sb, sw;
            
            // Parse rectangle attributes
            sscanf(line.c_str(),
                   "  <rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"rgb(%d,%d,%d)\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"%d\"",
                   &x, &y, &w, &h, &r, &g, &b, &sr, &sg, &sb, &sw);
            
            obj->x = x;
            obj->y = winHeight - y - h; // Convert from SVG to OpenGL coordinates
            obj->w = w;
            obj->h = h;
            obj->style.fillColor[0] = r / 255.0f;
            obj->style.fillColor[1] = g / 255.0f;
            obj->style.fillColor[2] = b / 255.0f;
            obj->style.strokeColor[0] = sr / 255.0f;
            obj->style.strokeColor[1] = sg / 255.0f;
            obj->style.strokeColor[2] = sb / 255.0f;
            obj->style.strokeWidth = sw;
            
            loadedObjects.push_back(obj);
            
        } else if (line.find("<circle") != std::string::npos) {
            Drawable* obj = new Drawable();
            obj->type = CIRCLE;
            
            float cx, cy, r;
            int fr, fg, fb, sr, sg, sb, sw;
            
            // Parse circle attributes
            sscanf(line.c_str(),
                   "  <circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"rgb(%d,%d,%d)\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"%d\"",
                   &cx, &cy, &r, &fr, &fg, &fb, &sr, &sg, &sb, &sw);
            
            obj->x = cx;
            obj->y = winHeight - cy; // Convert from SVG to OpenGL coordinates
            obj->w = r;
            obj->h = r;
            obj->style.fillColor[0] = fr / 255.0f;
            obj->style.fillColor[1] = fg / 255.0f;
            obj->style.fillColor[2] = fb / 255.0f;
            obj->style.strokeColor[0] = sr / 255.0f;
            obj->style.strokeColor[1] = sg / 255.0f;
            obj->style.strokeColor[2] = sb / 255.0f;
            obj->style.strokeWidth = sw;
            
            loadedObjects.push_back(obj);
        }
    }
    
    in.close();
    
    // Add objects in reverse order to maintain visual layering
    for (int i = loadedObjects.size() - 1; i >= 0; i--) {
        loadedObjects[i]->next = head;
        head = loadedObjects[i];
    }
    
    glutPostRedisplay();
    std::cout << "Successfully opened " << filename << " (" << loadedObjects.size() << " objects loaded)\n";
}

void menuFile(int option) {
    switch(option) {
        case 1: saveSVG("output.svg"); break;
        case 2: openSVG("output.svg"); break;
        case 3: exportBitmap("output.bmp"); break;
    }
}


void menuQuit(int) {
    exit(0);
}

// --------------------- Mouse Input ------------------------
void mouse(int button, int state, int x, int y) {
    int mx = x;
    int my = winHeight - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentShape == RECTANGLE) {
            isDrawing = true;
            startX = mx;
            startY = my;
            currentX = mx;
            currentY = my;
        } else if (currentShape == CIRCLE) {
            isDrawing = true;
            startX = mx;
            startY = my;
            currentX = mx;
            currentY = my;
        } else if (moveMode) {
            selectedIndex = findShapeAt(mx, my);
            if (selectedIndex != -1) {
                Drawable* sel = head;
                for (int i = 0; i < selectedIndex; i++) sel = sel->next;
                dragOffsetX = mx - sel->x;
                dragOffsetY = my - sel->y;
                dragging = true;
            }
            glutPostRedisplay();
        } else {
            // Selection mode
            int found = findShapeAt(mx, my);
            if (found != -1) {
                selectedIndex = found;
            } else {
                selectedIndex = -1; // Deselect
            }
            glutPostRedisplay();
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (isDrawing && currentShape == RECTANGLE) {
            isDrawing = false;
            
            float minX = std::min(startX, currentX);
            float maxX = std::max(startX, currentX);
            float minY = std::min(startY, currentY);
            float maxY = std::max(startY, currentY);
            
            Drawable* obj = new Drawable();
            obj->type = RECTANGLE;
            obj->x = minX;
            obj->y = minY;
            obj->w = maxX - minX;
            obj->h = maxY - minY;
            obj->style = style;
            obj->next = head;
            head = obj;
            glutPostRedisplay();
        } else if (isDrawing && currentShape == CIRCLE) {
            isDrawing = false;
            
            int dx = currentX - startX;
            int dy = currentY - startY;
            int radius = (int)sqrt(dx * dx + dy * dy);
            
            if (radius > 0) {
                Drawable* obj = new Drawable();
                obj->type = CIRCLE;
                obj->x = startX;
                obj->y = startY;
                obj->w = radius;
                obj->h = radius;
                obj->style = style;
                obj->next = head;
                head = obj;
                glutPostRedisplay();
            }
        }
        dragging = false;
    }
}

void motion(int x, int y) {
    int my = winHeight - y;
    
    if (isDrawing && (currentShape == RECTANGLE || currentShape == CIRCLE)) {
        currentX = x;
        currentY = my;
        glutPostRedisplay();
    } else if (dragging && selectedIndex != -1 && moveMode) {
        Drawable* sel = head;
        for (int i = 0; i < selectedIndex; i++) sel = sel->next;
        if (sel) {
            sel->x = x - dragOffsetX;
            sel->y = my - dragOffsetY;
            glutPostRedisplay();
        }
    }
}

void deleteSelected() {
    if (selectedIndex == -1) return;

    Drawable* prev = nullptr;
    Drawable* curr = head;
    for (int i = 0; i < selectedIndex; i++) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) head = curr->next;
    else prev->next = curr->next;

    delete curr;
    selectedIndex = -1;
    glutPostRedisplay();
}

void moveFront() {
    if (selectedIndex == -1) return;

    Drawable* prev = nullptr;
    Drawable* curr = head;
    for (int i = 0; i < selectedIndex; i++) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) return; // Already at front

    prev->next = curr->next;
    
    Drawable* tail = head;
    while (tail->next) tail = tail->next;
    tail->next = curr;
    curr->next = nullptr;
    
    selectedIndex = -1;
    glutPostRedisplay();
}

void moveBack() {
    if (selectedIndex == -1) return;

    Drawable* prev = nullptr;
    Drawable* curr = head;
    for (int i = 0; i < selectedIndex; i++) {
        prev = curr;
        curr = curr->next;
    }

    if (prev) prev->next = curr->next;
    curr->next = head;
    head = curr;
    selectedIndex = 0;
    glutPostRedisplay();
}

void menuEdit(int option) {
    switch(option) {
        case 1: // Select
            currentShape = NONE; 
            moveMode = false;
            selectedIndex = -1;
            glutPostRedisplay();
            break;
        case 2: // Delete
            deleteSelected();
            break;
        case 3: // Move Front
            moveFront();
            break;
        case 4: // Move Back
            moveBack();
            break;
        case 5: // Move Around
            currentShape = NONE;
            moveMode = true;
            break;
    }
}

// --------------------- Init ------------------------
void initMenu() {
    int drawMenu = glutCreateMenu(menuDraw);
    glutAddMenuEntry("Rectangle", 1);
    glutAddMenuEntry("Circle", 2);

    int fillMenu = glutCreateMenu(menuFillColor);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Black", 4);
    glutAddMenuEntry("White", 5);

    int strokeMenu = glutCreateMenu(menuStrokeColor);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Black", 4);
    glutAddMenuEntry("White", 5);

    int strokeWidthMenu = glutCreateMenu(menuStrokeWidth);
    for (int i=1; i<=5; i++) {
        std::string s = "Width " + std::to_string(i);
        glutAddMenuEntry(s.c_str(), i);
    }

    int styleMenu = glutCreateMenu(NULL);
    glutAddSubMenu("Fill Color", fillMenu);
    glutAddSubMenu("Stroke Color", strokeMenu);
    glutAddSubMenu("Stroke Width", strokeWidthMenu);

    int editMenu = glutCreateMenu(menuEdit);
    glutAddMenuEntry("Select", 1);
    glutAddMenuEntry("Delete", 2);
    glutAddMenuEntry("Move Front", 3);
    glutAddMenuEntry("Move Back", 4);
    glutAddMenuEntry("Move Around", 5);

    int fileMenu = glutCreateMenu(menuFile);
    glutAddMenuEntry("Save SVG", 1);
    glutAddMenuEntry("Open SVG", 2);
    glutAddMenuEntry("Save Bitmap", 3);

    glutCreateMenu(NULL);
    glutAddMenuEntry("New", 1);
    glutAddSubMenu("Draw", drawMenu);
    glutAddSubMenu("Style", styleMenu);
    glutAddSubMenu("Edit", editMenu);
    glutAddSubMenu("File", fileMenu);
    glutAddMenuEntry("Quit", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, winWidth, 0, winHeight);
}

// --------------------- Main ------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow(winTitle.c_str());

    initGL();
    initMenu();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}