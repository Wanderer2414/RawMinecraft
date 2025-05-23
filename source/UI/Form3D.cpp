#include "Form3D.h"
#include "Container3D.h"
#include "Controller.h"
#include "Global.h"
#include "spriv_extended.h"

extern Vector2f WindowSize;

Form3D::Form3D(const int& index): pCamera(0), sCamera(pCamera) {
    form_index = index;
    return_value = INT_MIN;

    gladLoadGL();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    pShader = glCreateProgram();
    GLuint vshader = createShaderFromSPIRV(GL_VERTEX_SHADER, "assets/shaders/shader.vert.spv");
    GLuint fshader = createShaderFromSPIRV(GL_FRAGMENT_SHADER, "assets/shaders/shader.frag.spv");
    glAttachShader(pShader,vshader);
    glAttachShader(pShader,fshader);
    glLinkProgram(pShader);
    glDeleteShader(vshader);
    glDeleteShader(fshader);


    glm::mat4 model(1);
    
    glm::mat4 view = glm::lookAt(glm::vec3(1, 1, 3), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), WindowSize.x/WindowSize.y, 0.1f, 100.0f);

    glGenBuffers(1, &pCamera);
    glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, pCamera);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &model);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &proj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
Form3D::~Form3D() {
    glDeleteProgram(pShader);
    glDeleteBuffers(sizeof(glm::mat4)*3, &pCamera);
}
Vector2f Form3D::getSize() const {
    return WindowSize;
}
_catch_function(Form3D, CatchEvent) {
    bool ans = false;
    if (event.type == Event::Resized) {
        FloatRect rect = {0, 0, 1.0f*event.size.width, 1.0f*event.size.height};
        window.setView(View(rect));
        ans = true;
    }
    ans = Container::CatchEvent(window, event, state) || ans;
    ans = Container3D::CatchEvent(window, event, state) || ans;
    ans = sCamera.CatchEvent(window, event, state);
    return ans;
}
bool Form3D::contains(const Ray3f& position) const {
    return true;
}
int Form3D::run(RenderWindow& window) {
    Event event;
    bool is_changed = true, is_catched = false;
    while (window.isOpen()) {
        Container::reset();
        Container3D::reset();
        while (window.pollEvent(event)) {
            if (!is_catched) {
                is_catched = true;
                is_changed = BeforeCatch(window, event) || is_changed;
                is_changed = Container::setHover(static_cast<Vector2f>(Mouse::getPosition(window))) || is_changed;
                is_changed = Container3D::setHover(sCamera.getSight()) || is_changed;
                is_changed = sCamera.setHover(sCamera.getSight()) || is_changed;
            }
            is_changed = CatchEvent(window, event) || is_changed;
            if (event.type == Event::Closed) window.close();
        }
        if (is_catched) {
            is_changed = AfterCatch(window, event) || is_changed;
            is_catched = false;
        }
        is_changed = handle(window) || is_changed;
        if (is_changed) {
            // window.clear();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0, 0, 0, 0);
            glUseProgram(pShader);
            glBindBuffer(GL_UNIFORM_BUFFER, pCamera);
            Container3D::glDraw();
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            // glFlush();
            // window.pushGLStates();
            // draw(window);
            // window.popGLStates();
            window.display();
        }
        if (return_value!=INT_MIN) return return_value;
        is_changed = 0;
    }
    return form_index;
}

_catch_function(Form3D, BeforeCatch) {
    bool is_changed = Container::BeforeCatch(window, event);
    is_changed = Container3D::BeforeCatch(window, event) || is_changed;
    is_changed = sCamera.BeforeCatch(window, event) || is_changed;
    return is_changed;
}
_catch_function(Form3D, AfterCatch) {
    bool is_changed = Container::AfterCatch(window, event);
    is_changed = Container3D::AfterCatch(window, event) || is_changed;
    is_changed = sCamera.AfterCatch(window, event) || is_changed;
    return is_changed;
}
_handle_function(Form3D, handle) {
    bool is_changed = Container::handle(window, state);
    is_changed = Container3D::handle(window, state) || is_changed;
    is_changed = sCamera.handle(window, state) || is_changed;
    return is_changed;
}
void Form3D::insert(Controller* controller, const int& layer) {
    Container::insert(controller, layer);
}
void Form3D::erase(Controller* controller) {
    Container::erase(controller);
}
void Form3D::insert(Controller3D* controller, const int& layer) {
    Container3D::insert(controller, layer);
}
void Form3D::erase(Controller3D* controller) {
    Container3D::insert(controller);
}
void Form3D::draw(RenderTarget& target, RenderStates state) const {
    Container3D::draw(target, state);
    sCamera.draw(target, state);
    Container::draw(target, state);
}