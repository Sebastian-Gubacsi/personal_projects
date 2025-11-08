# Makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = 

# Platform-specific settings
UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
    LDFLAGS += -lglut -lGLU -lGL
endif

ifeq ($(UNAME), Darwin)
    LDFLAGS += -framework GLUT -framework OpenGL
endif

ifeq ($(OS), Windows_NT)
    LDFLAGS += -lfreeglut -lopengl32 -lglu32
    TARGET = tournament_manager.exe
else
    TARGET = tournament_manager
endif

SOURCES = main.cpp SheetManager.cpp UserManager.cpp UIRenderer.cpp InputHandler.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf sheets/

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

# ============================================================================
# README.md
# 
# Tournament Scoring Sheet Manager
# 
# A comprehensive C++ GLUT application for managing tournament scoring sheets
# with real-time updates, user management, and Excel export capabilities.
# 
# ## Features
# 
# - **User Management**: Register and login with email addresses
# - **Sheet Creation**: Create custom tournament scoring sheets
# - **Real-time Editing**: Edit cells in a spreadsheet-like interface
# - **Sharing**: Share sheets with other registered users
# - **Auto-save**: Automatic saving every 30 seconds
# - **Excel Export**: Export to CSV format (Excel-compatible)
# - **Multi-section Support**: Organize tags into visible and hidden sections
# - **Player Management**: Add/remove players dynamically
# - **Point Tracking**: Automatic calculation of section and total scores
# 
# ## File Structure
# 
# ```
# tournament-manager/
# ├── main.cpp              # Entry point
# ├── SheetManager.h/cpp    # Sheet data management
# ├── UserManager.h/cpp     # User authentication
# ├── UIRenderer.h/cpp      # OpenGL rendering
# ├── InputHandler.h/cpp    # Input processing
# ├── Makefile             # Build configuration
# └── README.md            # This file
# ```
# 
# ## Building
# 
# ### Linux
# ```bash
# sudo apt-get install freeglut3-dev
# make
# ```
# 
# ### macOS
# ```bash
# # GLUT is included with macOS
# make
# ```
# 
# ### Windows
# ```bash
# # Install freeglut first
# # Using MinGW:
# make
# ```
# 
# ## Usage
# 
# ### First Time Setup
# 
# 1. **Launch the application**
#    ```bash
#    ./tournament_manager
#    ```
# 
# 2. **Login/Register**
#    - Press `F1`
#    - Enter your email address
#    - If email doesn't exist, it will register you automatically
# 
# 3. **Create a Sheet**
#    - Press `F2` in the main menu
#    - Enter a sheet name
#    - Sheet is created with default sections
# 
# ### Keyboard Controls
# 
# **Main Menu:**
# - `F1` - Login/Register
# - `F2` - Create New Sheet
# - `F3` - Load Sheet (not fully implemented in basic version)
# - `F4` - Share Sheet
# - `F5` - Export to Excel
# - `Enter` - Open current sheet in editor
# 
# **Sheet Editor:**
# - `Arrow Keys` - Navigate between cells
# - `Enter` - Edit selected cell
# - `+` - Add new player
# - `Page Up/Down` - Scroll through sections
# - `ESC` - Return to main menu
# 
# **Dialog:**
# - `Type` - Enter text
# - `Backspace` - Delete character
# - `Enter` - Confirm input
# - `ESC` - Cancel
# 
# ## Data Format
# 
# ### Sheet Structure
# 
# Each sheet contains:
# - **Players**: List of participant names/Discord names
# - **Sections**: Groups of tags (e.g., "Visible Tags", "Hidden Tags")
# - **Tags**: Individual challenges with point values
# - **Cell Data**: Player-specific values for each tag
# 
# ### File Storage
# 
# - **Users**: `users.dat` - User account information
# - **Sheets**: `sheets/*.tscore` - Individual sheet files
# - **Exports**: `*.csv` - Excel-compatible exports
# 
# ### Sheet File Format (.tscore)
# 
# ```
# SHEET_ID:sheet_1234567890_123
# NAME:My Tournament
# OWNER:user_1234567890_456
# MODIFIED:1234567890
# SHARED:user_123,user_456
# PLAYERS:Player1,Player2,Player3
# SECTIONS:2
# SECTION:Visible Tags:16
# TAGS:8
# TAG:1:1:Fight 'Fatty' or 'Crying Thief'
# DATA:1
# Player1=Yes
# ...
# ```
# 
# ## Customization
# 
# ### Adding Default Tags
# 
# Modify `SheetManager::createNewSheet()` to add default tags:
# 
# ```cpp
# bool SheetManager::createNewSheet(const std::string& sheetName) {
#     // ... existing code ...
#     
#     addSection("Visible Tags", 16);
#     addTag("Visible Tags", "Fight 'Fatty' or 'Crying Thief'", 1);
#     addTag("Visible Tags", "Recruit a max of 1 Operator", 1);
#     // ... more tags ...
#     
#     addSection("Hidden Tags", 16);
#     addTag("Hidden Tags", "Get a 'tactical prop'", 1);
#     // ... more tags ...
#     
#     return saveSheet();
# }
# ```
# 
# ### Changing Cell Size
# 
# In `UIRenderer.cpp`:
# 
# ```cpp
# int cellWidth = 100;  // Adjust width
# float y += 35;        // Adjust row height
# ```
# 
# ### Auto-save Interval
# 
# In `main.cpp`:
# 
# ```cpp
# glutTimerFunc(30000, timer, 0);  // 30000ms = 30 seconds
# ```
# 
# ## Features Explained
# 
# ### User Management
# 
# - Users are identified by email addresses
# - Each user gets a unique ID
# - Users can own and share sheets
# - Simple login without passwords (for local use)
# 
# ### Sheet Sharing
# 
# - Owner can share with other registered users
# - Shared users can view and edit sheets
# - Access control prevents unauthorized viewing
# 
# ### Real-time Updates
# 
# - Changes are immediately reflected in UI
# - Auto-save prevents data loss
# - Unsaved changes indicator
# 
# ### Excel Export
# 
# - Exports to CSV format
# - Includes all sections and tags
# - Automatic score calculation
# - Compatible with Excel, Google Sheets, etc.
# 
# ## Troubleshooting
# 
# ### Build Errors
# 
# **"Cannot find -lglut"**
# - Install GLUT library: `sudo apt-get install freeglut3-dev`
# 
# **"GL/glut.h not found"**
# - Check GLUT installation
# - On macOS: Use `#include <GLUT/glut.h>`
# 
# ### Runtime Issues
# 
# **"Failed to create file"**
# - Check write permissions
# - Ensure `sheets/` directory exists
# 
# **"User not found"**
# - Register first with F1
# - Check `users.dat` file exists
# 
# **Black screen**
# - Check OpenGL driver installation
# - Try updating graphics drivers
# 
# ## Extending the Application
# 
# ### Network Sync (Future Enhancement)
# 
# To add real-time synchronization:
# 
# 1. Add a `NetworkManager` class
# 2. Implement WebSocket or REST API
# 3. Sync on each sheet modification
# 4. Handle conflict resolution
# 
# ### Database Support
# 
# Replace file storage with SQLite:
# 
# 1. Add SQLite library
# 2. Create database schema
# 3. Replace file I/O with SQL queries
# 4. Add transaction support
# 
# ### Advanced Features
# 
# - Formula support for automatic calculations
# - Conditional formatting
# - Data validation
# - Import from existing Excel files
# - Multiple sheet views/filters
# - Export to PDF
# - History/undo functionality
# 
# ## License
# 
# This is example code for educational purposes.
# Feel free to modify and use as needed.
# 
# ## Support
# 
# For issues and questions:
# 1. Check the README
# 2. Review the source code comments
# 3. Test with provided examples
# 
# ---
# 
# Created with Claude AI
# Version 1.0