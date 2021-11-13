/**
    \file glshell.h
*/

#ifndef GL_SHELL_H_
#define GL_SHELL_H_

#include <GL/glew.h>

// link with glew32.lib (compiler-specific cheat)
#if _WIN32
#  pragma comment (lib, "glew32")
#endif

#include <GL/freeglut.h>

// gl math
#include <glm/glm.hpp>                      // glm::vec3, glm::vec4, glm::ivec4, glm::mat4, ...
#include <glm/gtc/matrix_transform.hpp>     // glm::translate, glm::rotate, glm::scale, glm::perspective, ...
#include <glm/gtc/type_ptr.hpp>             // glm::value_ptr

// undefine some Windows-specific #defines from windows.h and its cronies, included by freeglut.h
#if _WIN32
#  ifdef min
#    undef min
#  endif
#  ifdef max
#    undef max
#  endif
#  ifdef near
#    undef near
#  endif
#  ifdef far
#    undef far
#  endif
#  ifdef CreateWindow
#    undef CreateWindow
#  endif
#endif

#include <string>
#include <vector>
#include <exception>

class GLShell;
class GLApp;


/**
    \enum KeyCode
    \brief Enumeration of common key codes.
*/
// Yikes... http://en.wikipedia.org/wiki/Keyboard_layout
enum KeyCode {
    KC_UNKNOWN,

    // common ASCII keys
    KC_SPACE, KC_TAB, KC_RETURN, KC_ESCAPE, KC_BACKSPACE, KC_DELETE,
    KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0,
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M,
    KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S, KC_T, KC_U, KC_V, KC_W, KC_X, KC_Y, KC_Z,

    // common special keys
    KC_UP, KC_DOWN, KC_RIGHT, KC_LEFT, // arrows
    KC_PAGE_UP, KC_PAGE_DOWN, KC_HOME, KC_END, KC_INSERT, // misc navigation
    KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, // function keys
    
    // modifier keys
    KC_SHIFT, KC_SHIFT_L, KC_SHIFT_R, 
    KC_CTRL, KC_CTRL_L, KC_CTRL_R,
    KC_ALT, KC_ALT_L, KC_ALT_R,

    KC_NUM_KEYS
};


/**
    \enum MouseButton
    \brief Enumeration of common mouse buttons.
*/
enum MouseButton {
    MOUSE_BUTTON_UNKNOWN,

    MOUSE_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_RIGHT,

    MOUSE_NUM_BUTTONS
};


/** \brief A class that encapsulates the current keyboard state.

    A Keyboard object is used by the GLShell to communicate the keyboard status and events
    to a GLApp.

    The GLShell is responsible for creating, initializing, and updating the Keyboard for
    every frame.  Thus, the GLShell has read-write access to the Keyboard.

    The GLApp has read-only access to the Keyboard and can only query the Keyboard state
    and check for events.

    A GLApp can get a pointer to its Keyboard by calling GLApp::getKeyboard().
*/
class Keyboard {
public:
    /** @name Application interface.
        Use these methods to check the state of the keyboard from an application.
    */
    //@{

    /** Check if the given key is currently held down.
     */
    bool                isKeyDown(KeyCode key) const;

    /** Check if the given key is currently up (not held down).
     */
    bool                isKeyUp(KeyCode key) const;

    /** Check if the given key got pressed during this frame.
        @return
            This method returns true if the button is currently down,
            but was up during the previous frame.
     */
    bool                keyPressed(KeyCode key) const;

    /** Check if the given key got released during this frame.
        @return
            This method returns true if the button is currently up,
            but was down during the previous frame.
     */
    bool                keyReleased(KeyCode key) const;

    //@}

    /** @name Internal methods.
        These methods should only be used by the mouse handler.  An application should have
        no need to call these methods directly.
     */
    
    //@{

    /** Default constructor.
     */            
                        Keyboard();

    /** Report a key press event.
     */
    void                injectKeyDown(KeyCode kc);

    /** Report a key release event.
     */
    void                injectKeyUp(KeyCode kc);

    /** Call to update internal data structures once per frame.
     */
    void                update();

    /** Call to clear current keyboard state as if all keys were up.
        Useful when current window loses focus.
     */
    void                clear();

    //@}

private:
    std::vector<bool>   mCurrKeyState;
    std::vector<bool>   mPrevKeyState;
};


inline bool Keyboard::isKeyDown(KeyCode key) const
{
    return mCurrKeyState[key];
}

inline bool Keyboard::isKeyUp(KeyCode key) const
{
    return !mCurrKeyState[key];
}

inline bool Keyboard::keyPressed(KeyCode key) const
{
    return mCurrKeyState[key] && !mPrevKeyState[key];
}

inline bool Keyboard::keyReleased(KeyCode key) const
{
    return !mCurrKeyState[key] && mPrevKeyState[key];
}


/** \brief A class that encapsulates the current mouse state.

    A Mouse object is used by the GLShell to communicate the mouse status and events
    to a GLApp.

    The GLShell is responsible for creating, initializing, and updating the Mouse for
    every frame.  Thus, the GLShell has read-write access to the Mouse.

    The GLApp has read-only access to the Mouse and can only query the Mouse state
    and check for events.

    A GLApp can get a pointer to its Mouse by calling GLApp::getMouse().
*/
class Mouse {
public:
    /** @name Application interface.
        Use these methods to check the state of the mouse from an application.
    */
    //@{

    /** Check whether the given mouse button is currently down.
     */
    bool                isButtonDown(MouseButton button) const;

    /** Check whether the given mouse button is currently up.
     */
    bool                isButtonUp(MouseButton button) const;

    /** Check whether the given mouse button got pressed during this frame.
        @return
            This method returns true if the mouse button is currently down,
            but was up during the previous frame.
    */
    bool                buttonPressed(MouseButton button) const;

    /** Check whether the given mouse button got released during this frame.
        @return
            This method returns true if the mouse button is currently up,
            but was down during the previous frame.
    */
    bool                buttonReleased(MouseButton button) const;

    /** Get the current x-coordinate of the mouse position in screen space.
        @remarks
            The top-left corner of the screen is (0, 0).
        @return
            A return value of 0 means that the mouse is at the left edge
            of the screen.  A return value greater than 0 means that the mouse
            is that many pixels to the right of the left edge of the screen.
     */
    int                 getX() const;

    /** Get the current y-coordinate of the mouse position in screen space.
        @remarks
            The top-left corner of the screen is (0, 0).
        @return
            A return value of 0 means that the mouse is at the top edge
            of the screen.  A return value greater than 0 means that the mouse
            is that many pixels below the top edge of the screen.
     */
    int                 getY() const;

    /** Check whether the mouse has moved since last frame.
     */
    bool                isMoving() const;

    /** Check whether the given button was held down while the mouse moved.
     */
    bool                isDragging(MouseButton button) const;

    /** Get the amount of horizontal mouse coursor displacement since the last frame.
        @return
            A return value of 0 means that the mouse has not moved horizontally.
            A return value greater than 0 means that the mouse has moved that many
            pixels to the right.
            A return value less than 0 means that the mouse has moved that many
            pixels to the left.
     */
    int                 getDeltaX() const;

    /** Get the amount of vertical mouse cursor displacement since the last frame.
        @return
            A return value of 0 means that the mouse has not moved vertically.
            A return value greater than 0 means that the mouse has moved upward
            that many pixels.
            A return value less than 0 means that the mouse has moved downward
            that many pixels.
     */
    int                 getDeltaY() const;

    /** Get the amount of scroll wheel displacement since the last frame.
        @return
            A return value of 0 means that the scroll wheel has not moved.
            A return value greater than 0 means that the scroll wheel has
            moved "forward" that many "steps".
            A return value less than 0 means that the scroll wheel has
            moved "backward" that many "steps".
     */
    int                 getWheelDelta() const;

    void                freezeCursor(bool freeze) const;  // FIXME: const

    //@}

    /** @name Internal methods.
        These methods should only be used by the mouse handler.  An application should have
        no need to call these methods directly.
     */
    
    //@{

    /** Default constructor.
     */            
                        Mouse();

    /** Report a mouse button press event.
     */
    void                injectButtonDown(MouseButton button);

    /** Report a mouse button release event.
     */
    void                injectButtonUp(MouseButton button);

    /** Report current mouse cursor coordinate in screen space.
     */
    void                injectMousePos(int x, int y);

    /** Report a mouse scroll event.
     */
    void                injectWheelDelta(int dir);

    void                injectMouseEnter();
    void                injectMouseLeave();

    /** Call to update internal data structures once per frame.
     */
    void                update();

    /** Call to clear current button state as if all buttons were up.
        Useful when current window loses focus.
     */
    void                clear();

    //@}

private:
    std::vector<bool>   mCurrButtonState;
    std::vector<bool>   mPrevButtonState;

    mutable int         mCurrX, mCurrY;
    mutable int         mPrevX, mPrevY;

    int                 mDeltaX, mDeltaY;

    int                 mWheelDelta;

    bool                mHaveMousePos;
    mutable bool        mFreezeCursor;
    mutable bool        mWarpedCursor;
};


inline bool Mouse::isButtonDown(MouseButton button) const
{
    return mCurrButtonState[button];
}

inline bool Mouse::isButtonUp(MouseButton button) const
{
    return !mCurrButtonState[button];
}

inline bool Mouse::buttonPressed(MouseButton button) const
{
    return mCurrButtonState[button] && !mPrevButtonState[button];
}

inline bool Mouse::buttonReleased(MouseButton button) const
{
    return !mCurrButtonState[button] && mPrevButtonState[button];
}

inline int Mouse::getX() const
{
    return mCurrX;
}

inline int Mouse::getY() const
{
    return mCurrY;
}

inline bool Mouse::isMoving() const
{
    return mCurrX != mPrevX || mCurrY != mPrevY;
}

inline bool Mouse::isDragging(MouseButton button) const
{
    return isMoving() && isButtonDown(button);
}

inline int Mouse::getDeltaX() const
{
    return mDeltaX;
}

inline int Mouse::getDeltaY() const
{
    return mDeltaY;
}

inline int Mouse::getWheelDelta() const
{
    return mWheelDelta;
}

/**

shell = window + input handlers
shell provides an interface between platform (OS/hardware) and GL application
separation of shell and GL application
- shell should not call any GL methods
- GL application should not call any shell methods
  (GL application should not be aware of the existance of shell)

*/
class GLShell {
public:
    const Keyboard&     getKeyboard() const;
    const Mouse&        getMouse() const;

    GLApp&              getApp() const;
    int                 getWindowId() const;

    static void         Run(GLApp& app, const std::string& title, int width, int height);

    static float        GetTime();

private:
                        GLShell(GLApp& app, int windowId);

                        ~GLShell();

                        GLShell(const GLShell&);
                        GLShell& operator= (const GLShell&);

    Keyboard&           getKeyboard();
    Mouse&              getMouse();

    bool                update();

    void                lostFocus();
    void                gainFocus();

    static void         Initialize();
    static void         InitializeKeys();
    static void         InitializeMouseButtons();

    static int          CreateWindow(const std::string& title, int width, int height);

    static void         WindowReshapeCallback(int w, int h);
    static void         WindowCloseCallback();
    static void         WindowEntryCallback(int state);

    static void         WindowVisibilityCallback(int state);
    static void         WindowStatusCallback(int state);

    static void         KeyDownCallback(unsigned char key, int x, int y);
    static void         KeyUpCallback(unsigned char key, int x, int y);
    static void         SpecialDownCallback(int key, int x, int y);
    static void         SpecialUpCallback(int key, int x, int y);

    static void         MouseButtonCallback(int button, int state, int x, int y);
    static void         MouseMotionCallback(int x, int y);
    static void         MouseDragCallback(int x, int y);
    static void         MouseWheelCallback(int wheel, int dir, int x, int y);
    static void         MouseEntryCallback(int state);

    static void         DisplayCallback();

    /** global callback (not associated with any window)
    */
    static void         UpdateCallback();

    // instance data
    //

    GLApp&              mApp;
    int                 mWindowId;

    Keyboard            mKeyboard;
    Mouse               mMouse;

    float               mTime;

    // static data
    //

    static std::vector<GLShell*>    smShells;

    static std::vector<KeyCode>     smAsciiKeys;
    static std::vector<KeyCode>     smSpecialKeys;

    static std::vector<MouseButton> smMouseButtons;
};


inline const Keyboard& GLShell::getKeyboard() const
{
    return GLShell::mKeyboard;
}

inline const Mouse& GLShell::getMouse() const
{
    return mMouse;
}

inline GLApp& GLShell::getApp() const
{
    return mApp;
}

inline int GLShell::getWindowId() const
{
    return mWindowId;
}

inline Keyboard& GLShell::getKeyboard()
{
    return GLShell::mKeyboard;
}

inline Mouse& GLShell::getMouse()
{
    return mMouse;
}

inline float GLShell::GetTime()
{
    return 0.001f * glutGet(GLUT_ELAPSED_TIME);
}


/**
*/
class GLApp {
public:
    virtual             ~GLApp();

    virtual void        initialize();
    virtual void        shutdown();
    virtual void        resize(int w, int h);
    virtual void        draw();
    virtual bool        update(float deltaT);

    void                setShell(const GLShell* shell);

    const GLShell*      getShell() const;
    const Keyboard*     getKeyboard() const;
    const Mouse*        getMouse() const;

    bool                isRunning() const;

protected:
                        GLApp();

private:
                        GLApp(const GLApp&);
                        GLApp& operator= (const GLApp&);

    const GLShell*      mShell;  // the shell instance that is executing this application
};


inline void GLApp::setShell(const GLShell* shell)
{
    mShell = shell;
}

inline const GLShell* GLApp::getShell() const
{
    return mShell;
}

inline const Keyboard* GLApp::getKeyboard() const
{
    return mShell ? &mShell->getKeyboard() : NULL;
}

inline const Mouse* GLApp::getMouse() const
{
    return mShell ? &mShell->getMouse() : NULL;
}

inline bool GLApp::isRunning() const
{
    return mShell != NULL;
}

void CheckGLErrors(const char* label, const char* function, const char* file, long line);
void CheckGLShader(GLuint shaderId, const char* label, const char* function, const char* file, long line);
void CheckGLProgram(GLuint progId, const char* label, const char* function, const char* file, long line);

#define CHECK_GL_ERRORS(label)              CheckGLErrors(label, __FUNCTION__, __FILE__, __LINE__)
#define CHECK_GL_SHADER(shader_id, label)   CheckGLShader(shader_id, label, __FUNCTION__, __FILE__, __LINE__)
#define CHECK_GL_PROGRAM(prog_id, label)    CheckGLProgram(prog_id, label, __FUNCTION__, __FILE__, __LINE__)

/**
*/
class Exception : public std::exception {
    std::string             mMessage;

public:
    explicit                Exception(const std::string& msg);
    virtual                 ~Exception() throw();

    // overrides std::exception::what
    // subclasses, if any, don't need to override it
    //
    virtual const char*     what() const throw();
};

#endif
